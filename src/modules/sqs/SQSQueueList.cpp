#include <ui_SQSMessageAddDialog.h>
#include <modules/sqs/SQSQueueList.h>

SQSQueueList::SQSQueueList(const QString &title, QWidget *parent) : BasePage(parent) {
    // Connect service
    sqsService = new SQSService();
    connect(sqsService, &SQSService::ListQueuesSignal, this, &SQSQueueList::HandleListQueueSignal);
    connect(sqsService, &SQSService::ReloadQueuesSignal, this, &SQSQueueList::LoadContent);

    // Define toolbar
    auto *toolBar = new QHBoxLayout();
    auto *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar title
    auto *titleLabel = new QLabel(title);

    // Toolbar add action
    auto *addButton = new QPushButton(IconUtils::GetIcon("add"), "");
    addButton->setIconSize(QSize(16, 16));
    addButton->setToolTip("Add a new Queue");
    connect(addButton, &QPushButton::clicked, [this]() {
        bool ok;
        if (const QString text = QInputDialog::getText(nullptr, "Queue Name", "Queue name:", QLineEdit::Normal, "", &ok)
            ; ok && !text.isEmpty()) {
            sqsService->AddQueue(text);
        }
    });

    // Toolbar add action
    auto *purgeAllButton = new QPushButton(IconUtils::GetIcon("purge"), "");
    purgeAllButton->setIconSize(QSize(16, 16));
    purgeAllButton->setToolTip("Purge all Queues");
    connect(purgeAllButton, &QPushButton::clicked, [this]() {
        sqsService->PurgeAllQueues();
    });

    // Toolbar refresh action
    auto *refreshButton = new QPushButton(IconUtils::GetIcon("refresh"), "", this);
    refreshButton->setIconSize(QSize(16, 16));
    refreshButton->setToolTip("Refresh the Queue list");
    connect(refreshButton, &QPushButton::clicked, [this]() {
        LoadContent();
    });

    //toolBar->addWidget(backButton);
    toolBar->addWidget(titleLabel);
    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);
    toolBar->addWidget(purgeAllButton);
    toolBar->addWidget(refreshButton);

    // Table
    const QStringList headers = QStringList() = {
                                    tr("Name"), tr("Available"), tr("InFlight"), tr("Delayed"), tr("Size [kb]"),
                                    tr("Created"), tr("Modified"), tr("QueueUrl"), tr("QueueArn"), tr("IsDLQ")
                                };

    _tableView = new PageableTable();
    _tableView->SetHeaderNames(headers);
    _tableView->SetResizeModes({QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents});
    _tableView->SetHiddenColumns({7, 8, 9});
    _tableView->SetSortColumn(1, "attributes.approximateNumberOfMessages");
    _tableView->SetSortDirection(-1);

    // Connect double-click
    connect(_tableView, &PageableTable::DoubleClicked, this, [this](const QModelIndex &index) {
        // Extract ARN and URL
        QMap<QString, QString> arguments;
        arguments["queueUrl"] = _tableView->GetValue<QString>(index, 7);
        arguments["queueArn"] = _tableView->GetValue<QString>(index, 8);
        arguments["isDlq"] = _tableView->GetValue<bool>(index, 9) ? "true" : "false";

        // Send notification
        emit EventBus::instance().RouteChanged("SQS Message List", arguments);
    });

    // Connect paging changes
    connect(_tableView, &PageableTable::ReloadTable, this, &SQSQueueList::LoadContent);

    // Add context menu
    connect(_tableView, &PageableTable::ContextMenuRequested, this, &SQSQueueList::ShowContextMenu);

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(_tableView, 2);
}

SQSQueueList::~SQSQueueList() {
    StopAutoUpdate();
}

void SQSQueueList::LoadContent() {
    _tableView->Clear();
    sqsService->ListQueues(_tableView->GetPrefix(), _tableView->GetPageSize(), _tableView->GetPageIndex(), _tableView->GetSortAttribute(), _tableView->GetSortDirection());
}

void SQSQueueList::HandleListQueueSignal(const SQSQueueListResponse &queueListResponse) const {
    _tableView->SetTotalSize(queueListResponse.total);
    for (auto r = 0, c = 0; r < queueListResponse.queueCounters.count(); r++, c = 0) {
        _tableView->SetColumn(r, c++, queueListResponse.queueCounters.at(r).queueName);
        _tableView->SetColumn(r, c++, queueListResponse.queueCounters.at(r).available);
        _tableView->SetColumn(r, c++, queueListResponse.queueCounters.at(r).invisible);
        _tableView->SetColumn(r, c++, queueListResponse.queueCounters.at(r).delayed);
        _tableView->SetColumn(r, c++, queueListResponse.queueCounters.at(r).size / 1024);
        _tableView->SetColumn(r, c++, queueListResponse.queueCounters.at(r).created);
        _tableView->SetColumn(r, c++, queueListResponse.queueCounters.at(r).modified);
        _tableView->SetHiddenColumn(r, c++, queueListResponse.queueCounters.at(r).queueUrl);
        _tableView->SetHiddenColumn(r, c++, queueListResponse.queueCounters.at(r).queueArn);
        _tableView->SetHiddenColumn(r, c++, queueListResponse.queueCounters.at(r).isDlq);
    }
    _tableView->UpdateSorting();
}

void SQSQueueList::ShowContextMenu(const QPoint &pos) const {
    const QModelIndex index = _tableView->GetIndexFromPosition(pos);
    const bool isDql = _tableView->GetValue<bool>(index, 9);

    QMenu menu;
    menu.setToolTipsVisible(true);

    QAction *sendAction = menu.addAction(IconUtils::GetIcon("send"), "Send a Message");
    sendAction->setToolTip("Send a Message to the queue");

    QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit Queue");
    editAction->setToolTip("Edit the Queue details");

    menu.addSeparator();

    QAction *purgeAction = menu.addAction(IconUtils::GetIcon("purge"), "Purge Queue");
    purgeAction->setToolTip("Purge the Queue");

    QAction *redriveAction = menu.addAction(IconUtils::GetIcon("redrive"), "Redrive Queue");
    redriveAction->setToolTip("Redrive all messages");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Queue");
    deleteAction->setToolTip("Delete the Queue");

    // Conditional logic
    redriveAction->setEnabled(isDql);

    const auto queueUrl = _tableView->GetValue<QString>(index, 7);
    const auto queueArn = _tableView->GetValue<QString>(index, 8);
    if (const QAction *selectedAction = menu.exec(_tableView->GetGlobalPosition(pos)); selectedAction == purgeAction) {
        sqsService->PurgeQueue(queueUrl);
    } else if (selectedAction == sendAction) {
        SQSMessageAddDialog dialog(queueUrl, queueArn);
        dialog.exec();
    } else if (selectedAction == redriveAction) {
        sqsService->RedriveQueue(queueArn);
    } else if (selectedAction == deleteAction) {
        sqsService->DeleteQueue(queueUrl);
    } else if (selectedAction == editAction) {
        SQSQueueDetailsDialog dialog(queueArn);
        dialog.exec();
    }
}
