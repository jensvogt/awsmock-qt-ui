#include <modules/sns/SNSTopicList.h>

SNSTopicList::SNSTopicList(const QString &title, QWidget *parent) : BasePage(parent) {

    // Set region
    _region = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");

    // Connect service
    _snsService = new SNSService();
    connect(_snsService, &SNSService::ListTopicSignal, this, &SNSTopicList::HandleListTopicSignal);
    connect(_snsService, &SNSService::ReloadMessagesSignal, this, &SNSTopicList::LoadContent);

    // Title label
    const auto titleLabel = new QLabel(title, this);

    // Define toolbar
    const auto toolBar = new QHBoxLayout();
    toolBar->addWidget(titleLabel);
    const auto spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar add action
    const auto addButton = new QPushButton(IconUtils::GetIcon("add"), "", this);
    addButton->setIconSize(QSize(16, 16));
    addButton->setToolTip("Add a new topic");
    connect(addButton, &QPushButton::clicked, [this]() {
        bool ok;
        if (const QString topicName = QInputDialog::getText(nullptr, "Topic Name", "Topic name:", QLineEdit::Normal, "", &ok); ok && !topicName.isEmpty()) {
            _snsService->AddTopic(_region, topicName);
        }
    });

    // Toolbar add action
    const auto purgeAllButton = new QPushButton(IconUtils::GetIcon("purge"), "", this);
    purgeAllButton->setIconSize(QSize(16, 16));
    purgeAllButton->setToolTip("Purge all topics");
    connect(purgeAllButton, &QPushButton::clicked, [this]() {
        _snsService->PurgeAllTopics();
    });

    // Toolbar refresh action
    const auto refreshButton = new QPushButton(IconUtils::GetIcon("refresh"), "", this);
    refreshButton->setIconSize(QSize(16, 16));
    refreshButton->setToolTip("Refresh the topic list");
    connect(refreshButton, &QPushButton::clicked, this, [this]() {
        LoadContent();
    });

    //    toolBar->addWidget(titleLabel);
    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);
    toolBar->addWidget(purgeAllButton);
    toolBar->addWidget(refreshButton);

    // Prefix editor
    auto *prefixLayout = new QHBoxLayout();
    auto *prefixEdit = new QLineEdit(this);
    prefixEdit->setPlaceholderText("Prefix");
    connect(prefixEdit, &QLineEdit::textChanged, this, [this,prefixEdit]() {
        prefixValue = prefixEdit->text();
        prefixClear->setEnabled(true);
        LoadContent();
    });
    prefixLayout->addWidget(prefixEdit);
    prefixClear = new QPushButton(IconUtils::GetIcon("clear"), "", this);
    prefixClear->setDisabled(true);
    connect(prefixClear, &QPushButton::clicked, this, [this, prefixEdit]() {
        prefixEdit->clear();
        prefixValue = "";
        prefixClear->setEnabled(false);
    });
    prefixLayout->addWidget(prefixClear);

    // Table
    const QStringList headers = QStringList() = {tr("Name"), tr("Available"), tr("Send"), tr("Resend"), tr("Size"), tr("Created"), tr("Modified"), tr("TopicArn")};

    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(static_cast<int>(headers.count()));
    tableWidget->setShowGrid(true);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setHorizontalHeaderLabels(headers);
    tableWidget->setSortingEnabled(true);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    tableWidget->setColumnHidden(7, true);

    // Connect double-click
    connect(tableWidget, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {
        // Get the position
        const int row = index.row();

        // Extract ARN and URL
        const QString topicArn = tableWidget->item(row, 7)->text();

        // Send notification
        emit ShowSnsMessages(topicArn);
    });

    // Add context menu
    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableWidget, &QTableWidget::customContextMenuRequested, this, &SNSTopicList::ShowContextMenu);

    // Save sort column
    const QHeaderView *header = tableWidget->horizontalHeader();
    connect(header, &QHeaderView::sortIndicatorChanged, this, [this](const int column, const Qt::SortOrder order) {
        _sortColumn = column;
        _sortOrder = order;
    });

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addLayout(prefixLayout, 0);
    layout->addWidget(tableWidget, 2);
}

SNSTopicList::~SNSTopicList() {
    StopAutoUpdate();
}

void SNSTopicList::LoadContent() {
    if (Configuration::instance().GetConnectionState()) {
        _snsService->ListTopics(prefixValue);
    } else {
        QMessageBox::critical(nullptr, "Error", "Backend is not reachable");
    }
}

void SNSTopicList::HandleListTopicSignal(const SNSListTopicResult &listTopicResult) {
    const int selectedRow = tableWidget->selectionModel()->currentIndex().row();
    tableWidget->setRowCount(0);
    tableWidget->setSortingEnabled(false);
    for (auto r = 0, c = 0; r < listTopicResult.topicCounters.count(); r++, c = 0) {
        tableWidget->insertRow(r);
        SetColumn(tableWidget, r, c++, listTopicResult.topicCounters.at(r).topicName);
        SetColumn(tableWidget, r, c++, listTopicResult.topicCounters.at(r).messages);
        SetColumn(tableWidget, r, c++, listTopicResult.topicCounters.at(r).messagesSend);
        SetColumn(tableWidget, r, c++, listTopicResult.topicCounters.at(r).messagesResend);
        SetColumn(tableWidget, r, c++, listTopicResult.topicCounters.at(r).size);
        SetColumn(tableWidget, r, c++, listTopicResult.topicCounters.at(r).created);
        SetColumn(tableWidget, r, c++, listTopicResult.topicCounters.at(r).modified);
        SetHiddenColumn(tableWidget, r, c, listTopicResult.topicCounters.at(r).topicArn);
    }
    tableWidget->setRowCount(static_cast<int>(listTopicResult.topicCounters.count()));
    tableWidget->setSortingEnabled(true);
    tableWidget->sortItems(_sortColumn, _sortOrder);
    tableWidget->selectRow(selectedRow);
    NotifyStatusBar();
}

void SNSTopicList::ShowContextMenu(const QPoint &pos) const {
    const QModelIndex index = tableWidget->indexAt(pos);
    if (!index.isValid()) return;

    const int row = index.row();

    QMenu menu;
    QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit Topic");
    editAction->setToolTip("Edit the Topic details");

    menu.addSeparator();

    QAction *purgeAction = menu.addAction(IconUtils::GetIcon("purge"), "Purge Topic");
    purgeAction->setToolTip("Purge the Topic");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Topic");
    deleteAction->setToolTip("Delete the Topic");

    const QString topicArn = tableWidget->item(row, 7)->text();
    if (const QAction *selectedAction = menu.exec(tableWidget->viewport()->mapToGlobal(pos)); selectedAction == purgeAction) {
        _snsService->PurgeTopic(topicArn);
    } else if (selectedAction == deleteAction) {
        _snsService->DeleteTopic(topicArn);
    } else if (selectedAction == editAction) {
        if (SNSTopicDetailsDialog dialog(topicArn); dialog.exec() == QDialog::Accepted) {
            qDebug() << "SNS Topic edit dialog exit";
        }
    }
}
