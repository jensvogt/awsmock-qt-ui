#include <QStyleFactory>
#include <ui_SQSMessageAddDialog.h>
#include <modules/sqs/SQSQueueList.h>

#include "components/ContextMenu.h"
#include "components/Toast.h"

SQSQueueList::SQSQueueList(const QString &title, QWidget *parent) : BasePage(parent) {

    // Connect service
    _sqsService = new SQSService();
    connect(_sqsService, &SQSService::ListQueuesSignal, this, &SQSQueueList::HandleListQueueSignal);
    connect(_sqsService, &SQSService::ReloadQueuesSignal, this, &SQSQueueList::LoadContent);

    // Define toolbar
    auto *toolBar = new QHBoxLayout();
    auto *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar title
    auto *titleLabel = new QLabel(title);

    // Toolbar add action
    auto *addButton = new QPushButton(IconUtils::GetIcon("add"), "");
    addButton->setToolTip("Add a new queue");
    connect(addButton, &QPushButton::clicked, [this]() {
        bool ok;
        if (const QString text = QInputDialog::getText(nullptr, "Queue Name", "Queue name:", QLineEdit::Normal, "", &ok); ok && !text.isEmpty()) {
            _sqsService->AddQueue(text);
        }
    });

    // Toolbar add action
    auto *purgeAllButton = new QPushButton(IconUtils::GetIcon("purge"), "");
    purgeAllButton->setToolTip("Purge all queues");
    connect(purgeAllButton, &QPushButton::clicked, [this]() {
        _sqsService->PurgeAllQueues();
        new Awsmock::Components::ToastOverlay("Purge all queues initiated.\nChanges may take some time to propagate.", this);
    });

    // Toolbar reset counter action
    auto *resetCounterButton = new QPushButton(IconUtils::GetIcon("reset-counter"), "", this);
    resetCounterButton->setToolTip("Refresh the queue message counters");
    connect(resetCounterButton, &QPushButton::clicked, [this]() {
        _sqsService->ResetMessageCounters();
        LoadContent();
    });

    // Toolbar refresh action
    auto *refreshButton = new QPushButton(IconUtils::GetIcon("refresh"), "", this);
    refreshButton->setToolTip("Refresh the queue list");
    connect(refreshButton, &QPushButton::clicked, [this]() {
        LoadContent();
    });

    toolBar->addWidget(titleLabel);
    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);
    toolBar->addWidget(purgeAllButton);
    toolBar->addWidget(resetCounterButton);
    toolBar->addWidget(refreshButton);

    // Table
    const QStringList headers = {tr("Name"), tr("Available"), tr("InFlight"), tr("Delayed"), tr("Size"), tr("Created"), tr("Modified"), tr("QueueUrl"), tr("QueueArn"), tr("IsDLQ")};

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

    // Add details shortcut
    connect(_tableView, &PageableTable::ShowDetailsSignal, this, [this](const QModelIndex &index) {
        const auto queueArn = _tableView->GetValue<QString>(index, 8);
        SQSQueueDetailsDialog dialog(queueArn);
        dialog.exec();
    });

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(_tableView, 2);
}

SQSQueueList::~SQSQueueList() {
    StopAutoUpdate();
}

void SQSQueueList::LoadContent() {
    _sqsService->ListQueues(_tableView->GetPrefix(), _tableView->GetPageSize(), _tableView->GetPageIndex(), _tableView->GetSortAttribute(), _tableView->GetSortDirection());
}

void SQSQueueList::HandleListQueueSignal(const SQSQueueListResponse &queueListResponse) const {
    _tableView->SaveSelection();
    _tableView->setUpdatesEnabled(false);
    _tableView->ClearContent();
    _tableView->SetTotalSize(queueListResponse.total);
    for (auto r = 0, c = 0; r < queueListResponse.queueCounters.count(); r++, c = 0) {
        _tableView->SetColumn(r, c++, queueListResponse.queueCounters.at(r).queueName);
        _tableView->SetColumn(r, c++, queueListResponse.queueCounters.at(r).available);
        _tableView->SetColumn(r, c++, queueListResponse.queueCounters.at(r).invisible);
        _tableView->SetColumn(r, c++, queueListResponse.queueCounters.at(r).delayed);
        _tableView->SetColumn(r, c++, StringUtils::FormatSizeColumn(queueListResponse.queueCounters.at(r).size, 1), Qt::AlignRight | Qt::AlignVCenter);
        _tableView->SetColumn(r, c++, queueListResponse.queueCounters.at(r).created);
        _tableView->SetColumn(r, c++, queueListResponse.queueCounters.at(r).modified);
        _tableView->SetHiddenColumn(r, c++, queueListResponse.queueCounters.at(r).queueUrl);
        _tableView->SetHiddenColumn(r, c++, queueListResponse.queueCounters.at(r).queueArn);
        _tableView->SetHiddenColumn(r, c++, queueListResponse.queueCounters.at(r).isDlq);
    }
    _tableView->setUpdatesEnabled(true);
    _tableView->UpdateSorting();
    _tableView->RestoreSelection();
    logInfo << "SQS queue list updated";
}

void SQSQueueList::ShowContextMenu(const QPoint &pos) {

    StopAutoUpdate();

    const QModelIndex index = _tableView->GetIndexFromPosition(pos);
    const long total = _tableView->GetValue<long>(index, 1) + _tableView->GetValue<long>(index, 2) + _tableView->GetValue<long>(index, 3);

    auto *menu = new ContextMenu(this);

    QAction *sendAction = menu->addAction(IconUtils::GetIcon("send"), "Send a Message");
    sendAction->setToolTip("Send a message to the queue.");

    QAction *editAction = menu->addAction(IconUtils::GetIcon("edit"), "Edit Queue");
    editAction->setToolTip("Edit the queue details.");

    menu->addSeparator();

    QAction *purgeAction = menu->addAction(IconUtils::GetIcon("purge"), "Purge Queue");
    purgeAction->setToolTip("Purge the queue.");
    purgeAction->setEnabled(total > 0);

    QAction *redriveAction = menu->addAction(IconUtils::GetIcon("redrive"), "Redrive Queue");
    redriveAction->setToolTip("Redrive all messages. If the selected queue is a DLQ, it will perform an redrive, otherwise it will set all messages to status 'INITIAL'.");
    redriveAction->setEnabled(total > 0);

    menu->addSeparator();

    QAction *deleteAction = menu->addAction(IconUtils::GetIcon("delete"), "Delete Queue");
    deleteAction->setToolTip("Delete the queue.");

    const auto queueUrl = _tableView->GetValue<QString>(index, 7);
    const auto queueArn = _tableView->GetValue<QString>(index, 8);
    if (const QAction *selectedAction = menu->exec(_tableView->GetGlobalPosition(pos)); selectedAction == purgeAction) {
        _sqsService->PurgeQueue(queueUrl);
        new Awsmock::Components::ToastOverlay("Purge queue initiated.\nChanges may take some time to propagate.", this);
    } else if (selectedAction == sendAction) {
        if (SQSMessageAddDialog dialog(queueUrl, queueArn); dialog.exec() == QDialog::Accepted) {
            new Awsmock::Components::ToastOverlay("Message sent!\nID: " + dialog.GetMessageId(), _tableView);
        }
    } else if (selectedAction == redriveAction) {
        _sqsService->RedriveQueue(queueArn);
    } else if (selectedAction == deleteAction) {
        _sqsService->DeleteQueue(queueUrl);
    } else if (selectedAction == editAction) {
        SQSQueueDetailsDialog dialog(queueArn);
        dialog.exec();
    }
    StartAutoUpdate();
}
