#include <modules/sqs/SQSQueueList.h>

#include "dto/sqs/SQSListQueueResponse.h"
#include "utils/IconUtils.h"

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
    auto *addButton = new QPushButton(IconUtils::GetIcon("dark", "add"), "");
    addButton->setIconSize(QSize(16, 16));
    addButton->setToolTip("Add a new Queue");
    connect(addButton, &QPushButton::clicked, [this]() {
        bool ok;
        if (const QString text = QInputDialog::getText(nullptr, "Queue Name", "Queue name:", QLineEdit::Normal, "", &ok); ok && !text.isEmpty()) {
            sqsService->AddQueue(text);
        }
    });

    // Toolbar add action
    auto *purgeAllButton = new QPushButton(IconUtils::GetIcon("dark", "purge"), "");
    purgeAllButton->setIconSize(QSize(16, 16));
    purgeAllButton->setToolTip("Purge all Queues");
    connect(purgeAllButton, &QPushButton::clicked, [this]() {
        sqsService->PurgeAllQueues();
    });

    // Toolbar refresh action
    auto *refreshButton = new QPushButton(IconUtils::GetIcon("dark", "refresh"), "");
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

    // Prefix editor
    auto *prefixEdit = new QLineEdit(this);
    prefixEdit->setPlaceholderText("Prefix");
    connect(prefixEdit, &QLineEdit::returnPressed, this, [this,prefixEdit]() {
        prefixValue = prefixEdit->text();
        LoadContent();
    });

    // Table
    const QStringList headers = QStringList() << tr("Name")
                                << tr("Available")
                                << tr("InFlight")
                                << tr("Delayed")
                                << tr("Size")
                                << tr("Created")
                                << tr("Modified")
                                << tr("QueueUrl")
                                << tr("QueueArn")
                                << tr("IsDLQ");

    tableWidget = new QTableWidget();

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
    tableWidget->setColumnHidden(8, true);
    tableWidget->setColumnHidden(9, true);

    // Connect double-click
    connect(tableWidget, &QTableView::doubleClicked, this, [=](const QModelIndex &index) {
        // Get the position
        const int row = index.row();

        // Extract ARN and URL
        const QString queueUrl = tableWidget->item(row, 7)->text();
        const QString queueArn = tableWidget->item(row, 8)->text();

        // Send notification
        emit ShowMessages(queueArn, queueUrl);
    });

    // Add context menu
    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableWidget, &QTableWidget::customContextMenuRequested, this, &SQSQueueList::ShowContextMenu);

    // Save sort column
    const QHeaderView *header = tableWidget->horizontalHeader();
    connect(header, &QHeaderView::sortIndicatorChanged, this, [=](const int column, const Qt::SortOrder order) {
        _sortColumn = column;
        _sortOrder = order;
    });

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(prefixEdit, 1);
    layout->addWidget(tableWidget, 2);
}

SQSQueueList::~SQSQueueList() {
    StopAutoUpdate();
}

void SQSQueueList::LoadContent() {
    if (Configuration::instance().GetConnectionState()) {
        sqsService->ListQueues(prefixValue);
    } else {
        QMessageBox::critical(nullptr, "Error", "Backend is not reachable");
    }
}

void SQSQueueList::HandleListQueueSignal(const SQSQueueListResponse &queueListResponse) {
    tableWidget->setRowCount(0);
    tableWidget->setSortingEnabled(false); // stop sorting
    for (auto r = 0; r < queueListResponse.queueCounters.count(); r++) {
        tableWidget->insertRow(r);
        SetColumn(tableWidget, r, 0, queueListResponse.queueCounters.at(r).queueName);
        SetColumn(tableWidget, r, 1, queueListResponse.queueCounters.at(r).available);
        SetColumn(tableWidget, r, 2, queueListResponse.queueCounters.at(r).invisible);
        SetColumn(tableWidget, r, 3, queueListResponse.queueCounters.at(r).delayed);
        SetColumn(tableWidget, r, 4, queueListResponse.queueCounters.at(r).size);
        SetColumn(tableWidget, r, 5, queueListResponse.queueCounters.at(r).created);
        SetColumn(tableWidget, r, 6, queueListResponse.queueCounters.at(r).modified);
        SetHiddenColumn(tableWidget, r, 7, queueListResponse.queueCounters.at(r).queueUrl);
        SetHiddenColumn(tableWidget, r, 8, queueListResponse.queueCounters.at(r).queueArn);
        SetHiddenColumn(tableWidget, r, 9, queueListResponse.queueCounters.at(r).isDlq);
    }
    tableWidget->setRowCount(static_cast<int>(queueListResponse.queueCounters.count()));
    tableWidget->setSortingEnabled(true);
    tableWidget->sortItems(_sortColumn, _sortOrder);
    NotifyStatusBar();
}

void SQSQueueList::ShowContextMenu(const QPoint &pos) const {
    const QModelIndex index = tableWidget->indexAt(pos);
    if (!index.isValid()) return;

    const int row = index.row();
    const bool isDql = tableWidget->item(row, 9)->checkState();

    QMenu menu;

    QAction *editAction = menu.addAction(QIcon(":/icons/edit.png"), "Edit Queue");
    editAction->setToolTip("Edit the Queue details");

    menu.addSeparator();

    QAction *purgeAction = menu.addAction(QIcon(":/icons/purge.png"), "Purge Queue");
    purgeAction->setToolTip("Purge the Queue");

    QAction *redriveAction = menu.addAction(QIcon(":/icons/redrive.png"), "Redrive Queue");
    redriveAction->setToolTip("Redrive all messages");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("dark", "delete"), "Delete Queue");
    deleteAction->setToolTip("Delete the Queue");

    // Conditional logic
    redriveAction->setEnabled(isDql);

    const QString queueUrl = tableWidget->item(row, 7)->text();
    const QString queueArn = tableWidget->item(row, 8)->text();
    if (const QAction *selectedAction = menu.exec(tableWidget->viewport()->mapToGlobal(pos)); selectedAction == purgeAction) {
        sqsService->PurgeQueue(queueUrl);
    } else if (selectedAction == redriveAction) {
        sqsService->DeleteQueue(queueUrl);
    } else if (selectedAction == deleteAction) {
        sqsService->DeleteQueue(queueUrl);
    } else if (selectedAction == editAction) {
        SQSQueueDetailsDialog dialog(queueArn);
        if (dialog.exec() == QDialog::Accepted) {
            qDebug() << "SQS Queue edit dialog exit";
        }
    }
}
