
#include <modules/sqs/SQSMessageAddDialog.h>
#include "utils/IconUtils.h"

SQSMessageList::SQSMessageList(const QString &title, QString queueArn, const QString &queueUrl, QWidget *parent) : BasePage(parent), _queueArn(std::move(queueArn)), _queueUrl(queueUrl) {

    // Connect service events
    _sqsService = new SQSService();
    connect(_sqsService, &SQSService::ListMessagesSignal, this, &SQSMessageList::HandleListMessageSignal);
    connect(_sqsService, &SQSService::ReloadMessagesSignal, this, &SQSMessageList::HandleReloadMessageSignal);

    const auto toolBar = new QHBoxLayout();
    const auto spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar back action
    const auto backButton = new QPushButton(IconUtils::GetIcon("dark", "back"), "");
    backButton->setIconSize(QSize(16, 16));
    backButton->setToolTip("Go back to the queue list");
    connect(backButton, &QPushButton::clicked, [this]() {
        OnBackClicked();
    });

    // Toolbar label
    const auto titleLabel = new QLabel(title);

    // Toolbar add message action
    const auto addButton = new QPushButton(IconUtils::GetIcon("dark", "add"), "");
    addButton->setIconSize(QSize(16, 16));
    addButton->setToolTip("Add a new SQS message");
    connect(addButton, &QPushButton::clicked, [this]() {
        SQSMessageAddDialog dialog(_queueUrl);
        dialog.exec();
    });

    // Toolbar purge action
    const auto purgeAllButton = new QPushButton(IconUtils::GetIcon("dark", "purge"), "");
    purgeAllButton->setIconSize(QSize(16, 16));
    purgeAllButton->setToolTip("Purge all Queues");
    connect(purgeAllButton, &QPushButton::clicked, [this,queueUrl]() {
        _sqsService->PurgeAllMessages(queueUrl);
    });

    // Toolbar refresh action
    const auto refreshButton = new QPushButton(IconUtils::GetIcon("dark", "refresh"), "");
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
    auto prefixEdit = new QLineEdit(this);
    prefixEdit->setPlaceholderText("Prefix");
    connect(prefixEdit, &QLineEdit::returnPressed, this, [this,prefixEdit]() {
        prefixValue = prefixEdit->text();
        LoadContent();
    });

    // Table
    const QStringList headers = QStringList() << tr("ID")
                                << tr("ContentType")
                                << tr("Size")
                                << tr("Retries")
                                << tr("Created")
                                << tr("Modified")
                                << tr("QueueUrl")
                                << tr("QueueArn")
                                << tr("ReceiptHandle");

    tableWidget = new QTableWidget();

    tableWidget->setColumnCount(static_cast<int>(headers.count()));
    tableWidget->setShowGrid(true);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setHorizontalHeaderLabels(headers);
    tableWidget->setSortingEnabled(true);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    tableWidget->setColumnHidden(6, true);
    tableWidget->setColumnHidden(7, true);
    tableWidget->setColumnHidden(8, true);

    // Connect double-click
    connect(tableWidget, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {
        // Get the position
        const int row = index.row();

        const QString messageId = tableWidget->item(row, 0)->text();
        if (SQSMessageDetailsDialog dialog(messageId); dialog.exec() == QDialog::Accepted) {
            qDebug() << "SQS Queue edit dialog exit";
        }
    });

    // Add context menu
    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableWidget, &QTableWidget::customContextMenuRequested, this, &SQSMessageList::ShowContextMenu);

    // Save sort column
    const QHeaderView *header = tableWidget->horizontalHeader();
    connect(header, &QHeaderView::sortIndicatorChanged, this, [this](const int column, const Qt::SortOrder order) {
        _sortColumn = column;
        _sortOrder = order;
    });

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(prefixEdit, 1);
    layout->addWidget(tableWidget, 2);
}

SQSMessageList::~SQSMessageList() {
    StopAutoUpdate();
}

void SQSMessageList::LoadContent() {
    if (Configuration::instance().GetConnectionState()) {
        _sqsService->ListMessages(_queueArn, prefixValue);
    } else {
        QMessageBox::critical(nullptr, "Error", "Backend is not reachable");
    }
}

void SQSMessageList::HandleListMessageSignal(const SQSListMessagesResponse &listMessageResponse) {
    const int selectedRow = tableWidget->selectionModel()->currentIndex().row();
    tableWidget->clearContents();
    tableWidget->setRowCount(0);
    tableWidget->setSortingEnabled(false); // stop sorting
    tableWidget->sortItems(-1);

    for (auto r = 0, c = 0; r < listMessageResponse.messageCounters.count(); r++, c = 0) {
        tableWidget->insertRow(r);
        SetColumn(tableWidget, r, c++, listMessageResponse.messageCounters.at(r).messageId);
        SetColumn(tableWidget, r, c++, listMessageResponse.messageCounters.at(r).contentType);
        SetColumn(tableWidget, r, c++, listMessageResponse.messageCounters.at(r).size);
        SetColumn(tableWidget, r, c++, listMessageResponse.messageCounters.at(r).retries);
        SetColumn(tableWidget, r, c++, listMessageResponse.messageCounters.at(r).created);
        SetColumn(tableWidget, r, c++, listMessageResponse.messageCounters.at(r).modified);
        SetHiddenColumn(tableWidget, r, c++, listMessageResponse.messageCounters.at(r).queueUrl);
        SetHiddenColumn(tableWidget, r, c++, listMessageResponse.messageCounters.at(r).queueArn);
        SetHiddenColumn(tableWidget, r, c, listMessageResponse.messageCounters.at(r).receiptHandle);
    }
    tableWidget->setRowCount(static_cast<int>(listMessageResponse.messageCounters.count()));
    tableWidget->setSortingEnabled(true);
    tableWidget->selectRow(selectedRow);
    NotifyStatusBar();
}

void SQSMessageList::HandleReloadMessageSignal() const {
    _sqsService->ListMessages(_queueArn, prefixValue);
}

void SQSMessageList::ShowContextMenu(const QPoint &pos) const {
    const QModelIndex index = tableWidget->indexAt(pos);
    if (!index.isValid()) return;

    const int row = index.row();

    QMenu menu;
    /*    QAction *purgeAction = menu.addAction(QIcon(":/icons/purge.png"), "Purge Queue");
        purgeAction->setToolTip("Purge the Queue");
        QAction *redriveAction = menu.addAction(QIcon(":/icons/redrive.png"), "Redrive Queue");
        redriveAction->setToolTip("Redrive all messages");*/
    //menu.addSeparator();
    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("dark", "delete"), "Delete Message");
    deleteAction->setToolTip("Delete the message");

    if (const QAction *selectedAction = menu.exec(tableWidget->viewport()->mapToGlobal(pos)); selectedAction == deleteAction) {
        const QString queueUrl = tableWidget->item(row, 6)->text();
        const QString receiptHandle = tableWidget->item(row, 8)->text();
        _sqsService->DeleteMessage(queueUrl, receiptHandle);
    }
}
