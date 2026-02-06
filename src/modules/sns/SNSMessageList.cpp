
#include <modules/sns/SNSMessageList.h>

SNSMessageList::SNSMessageList(const QString &title, QWidget *parent) : BasePage(parent) {

    // Connect service
    _snsService = new SNSService();
    connect(_snsService, &SNSService::ListMessagesSignal, this, &SNSMessageList::HandleListMessageSignal);
    connect(_snsService, &SNSService::ReloadMessagesSignal, this, &SNSMessageList::HandleReloadMessageSignal);

    // Toolbar
    const auto toolBar = new QHBoxLayout();
    const auto spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar back action
    const auto backButton = new QPushButton(IconUtils::GetIcon("back"), "");
    backButton->setIconSize(QSize(16, 16));
    backButton->setToolTip("Go back to the topic list");
    connect(backButton, &QPushButton::clicked, [this]() {
        emit EventBus::instance().RouteChanged("SNS");
    });

    // Toolbar label
    const auto titleLabel = new QLabel(title);

    // Toolbar add action
    const auto addButton = new QPushButton(IconUtils::GetIcon("add"), "");
    addButton->setIconSize(QSize(16, 16));
    addButton->setToolTip("Add a new topic");
    connect(addButton, &QPushButton::clicked, [this]() {
        SNSMessageAddDialog dialog(_topicArn);
        dialog.exec();
    });

    // Toolbar add action
    const auto purgeAllButton = new QPushButton(IconUtils::GetIcon("purge"), "");
    purgeAllButton->setIconSize(QSize(16, 16));
    purgeAllButton->setToolTip("Purge all messages");
    connect(purgeAllButton, &QPushButton::clicked, [this]() {
        _snsService->PurgeMessages(_topicArn);
    });

    // Toolbar refresh action
    const auto refreshButton = new QPushButton(IconUtils::GetIcon("refresh"), "");
    refreshButton->setIconSize(QSize(16, 16));
    refreshButton->setToolTip("Refresh the queue list");
    connect(refreshButton, &QPushButton::clicked, [this]() {
        LoadContent();
    });

    toolBar->addWidget(backButton);
    toolBar->addWidget(titleLabel);
    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);
    toolBar->addWidget(purgeAllButton);
    toolBar->addWidget(refreshButton);

    // Prefix editor
    auto *prefixLayout = new QHBoxLayout();
    auto *prefixEdit = new QLineEdit(this);
    prefixEdit->setPlaceholderText("Prefix");
    prefixEdit->setToolTip("Prefix for the message ID");
    connect(prefixEdit, &QLineEdit::textChanged, this, [this,prefixEdit]() {
        _prefixValue = prefixEdit->text();
        _prefixClear->setEnabled(true);
        LoadContent();
    });
    prefixLayout->addWidget(prefixEdit);
    _prefixClear = new QPushButton(IconUtils::GetIcon("clear"), "", this);
    _prefixClear->setDisabled(true);
    _prefixClear->setToolTip("CLear the message ID prefix");
    connect(_prefixClear, &QPushButton::clicked, this, [this, prefixEdit]() {
        prefixEdit->clear();
        _prefixValue = "";
        _prefixClear->setEnabled(false);
    });
    prefixLayout->addWidget(_prefixClear);

    // Table
    const QStringList headers = QStringList() << tr("ID")
                                << tr("ContentType")
                                << tr("Size")
                                << tr("Status")
                                << tr("LastSend")
                                << tr("Created")
                                << tr("Modified")
                                << tr("TopicArn");

    _tableWidget = new QTableWidget();

    _tableWidget->setColumnCount(static_cast<int>(headers.count()));
    _tableWidget->setShowGrid(true);
    _tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    _tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    _tableWidget->setHorizontalHeaderLabels(headers);
    _tableWidget->setSortingEnabled(true);
    _tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    _tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    _tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    _tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Interactive);
    _tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    _tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    _tableWidget->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    _tableWidget->setColumnHidden(7, true);
    _tableWidget->addAction(GetRefreshAction(this));

    // Connect double-click
    connect(_tableWidget, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {
        // Get the position
        const int row = index.row();

        const QString messageId = _tableWidget->item(row, 0)->text();
        if (SNSMessageDetailsDialog dialog(messageId); dialog.exec() == QDialog::Accepted) {
            qDebug() << "SQS Queue edit dialog exit";
        }
    });

    // Add context menu
    _tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_tableWidget, &QTableWidget::customContextMenuRequested, this, &SNSMessageList::ShowContextMenu);

    // Save sort column
    const QHeaderView *header = _tableWidget->horizontalHeader();
    connect(header, &QHeaderView::sortIndicatorChanged, this, [this](const int column, const Qt::SortOrder order) {
        _sortColumn = column;
        _sortOrder = order;
    });

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addLayout(prefixLayout, 1);
    layout->addWidget(_tableWidget, 2);
}

SNSMessageList::~SNSMessageList() {
    StopAutoUpdate();
}

void SNSMessageList::LoadContent() {
    _topicArn = GetArgument<QString>("_topicArn");
    _snsService->ListMessages(_topicArn, _prefixValue);
}

void SNSMessageList::HandleListMessageSignal(const SNSListMessagesResult &listMessageResult) {
    const int selectedRow = _tableWidget->selectionModel()->currentIndex().row();
    _tableWidget->clearContents();
    _tableWidget->setRowCount(0);
    _tableWidget->setSortingEnabled(false);
    _tableWidget->sortItems(-1);
    for (auto r = 0, c = 0; r < listMessageResult.messageCounters.count(); r++, c = 0) {
        _tableWidget->insertRow(r);
        SetColumn(_tableWidget, r, c++, listMessageResult.messageCounters.at(r).messageId);
        SetColumn(_tableWidget, r, c++, listMessageResult.messageCounters.at(r).contentType);
        SetColumn(_tableWidget, r, c++, listMessageResult.messageCounters.at(r).size);
        SetColumn(_tableWidget, r, c++, listMessageResult.messageCounters.at(r).messageStatus);
        SetColumn(_tableWidget, r, c++, listMessageResult.messageCounters.at(r).lastSend);
        SetColumn(_tableWidget, r, c++, listMessageResult.messageCounters.at(r).created);
        SetColumn(_tableWidget, r, c++, listMessageResult.messageCounters.at(r).modified);
        SetHiddenColumn(_tableWidget, r, c, listMessageResult.messageCounters.at(r).topicArn);
    }
    _tableWidget->setRowCount(static_cast<int>(listMessageResult.messageCounters.count()));
    _tableWidget->setSortingEnabled(true);
    _tableWidget->sortItems(_sortColumn, _sortOrder);
    _tableWidget->selectRow(selectedRow);
    NotifyStatusBar();
}

void SNSMessageList::HandleReloadMessageSignal() {
    _snsService->ListMessages(_topicArn, _prefixValue);
    NotifyStatusBar();
}

void SNSMessageList::ShowContextMenu(const QPoint &pos) const {
    const QModelIndex index = _tableWidget->indexAt(pos);
    if (!index.isValid()) return;

    const int row = index.row();

    QMenu menu;
    menu.setToolTipsVisible(true);
    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Message");
    deleteAction->setToolTip("Delete the message");
    if (const auto selectedAction = menu.exec(_tableWidget->viewport()->mapToGlobal(pos)); selectedAction == deleteAction) {
        const QString messageId = _tableWidget->item(row, 0)->text();
        _snsService->DeleteMessage(_topicArn, messageId);
    }
}
