
#include <modules/sqs/SQSQueueList.h>

#include "utils/IconUtils.h"

SQSQueueList::SQSQueueList(const QString& title, QWidget *parent) : BasePage(parent)
{
    // Connect service
    sqsService = new SQSService();
    connect(sqsService, &SQSService::LoadContent, this, &SQSQueueList::LoadContent);

    // Define toolbar
    auto *toolBar = new QHBoxLayout();
    auto *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar title
    auto* titleLabel = new QLabel(title);

    // Toolbar add action
    auto *addButton = new QPushButton(IconUtils::GetIcon("dark", "add"),"");
    addButton->setIconSize(QSize(16, 16));
    addButton->setToolTip("Add a new Queue");
    connect(addButton, &QPushButton::clicked, [this](){
        bool ok;
        if (const QString text = QInputDialog::getText(nullptr, "Queue Name", "Queue name:", QLineEdit::Normal, "", &ok); ok && !text.isEmpty()) {
            sqsService->AddQueue(text);
        }
    });

    // Toolbar add action
    auto *purgeAllButton = new QPushButton(IconUtils::GetIcon("dark", "purge"),"");
    purgeAllButton->setIconSize(QSize(16, 16));
    purgeAllButton->setToolTip("Purge all Queues");
    connect(purgeAllButton, &QPushButton::clicked, [this](){
        sqsService->PurgeAllQueues();
    });

    // Toolbar refresh action
    auto *refreshButton = new QPushButton(IconUtils::GetIcon("dark", "refresh"),"");
    refreshButton->setIconSize(QSize(16, 16));
    refreshButton->setToolTip("Refresh the Queuelist");
    connect(refreshButton, &QPushButton::clicked, [this](){
        LoadContent();
    });

    //toolBar->addWidget(backButton);
    toolBar->addWidget(titleLabel);
    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);
    toolBar->addWidget(purgeAllButton);
    toolBar->addWidget(refreshButton);

    // Prefix editor
    auto* prefixEdit = new QLineEdit(this);
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

    tableWidget->setColumnCount(headers.count());
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
    tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Interactive);
    tableWidget->setColumnHidden(7, true);
    tableWidget->setColumnHidden(8, true);
    tableWidget->setColumnHidden(9, true);

    // Connect double-click
    connect(tableWidget, &QTableView::doubleClicked, this, [=](const QModelIndex &index) {

        // Get the position
        int row = index.row();
        int col = index.column();

        // Extract ARN and URL
        QString queueUrl = tableWidget->item(row, 7)->text();
        QString queueArn = tableWidget->item(row, 8)->text();

        // Send notification
        emit ShowMessages(queueArn, queueUrl);
    });

    // Add context menu
    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableWidget, &QTableWidget::customContextMenuRequested, this, &SQSQueueList::ShowContextMenu);

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(prefixEdit, 1);
    layout->addWidget(tableWidget, 2);
    layout->addStretch();
    layout->stretch(1);
}

SQSQueueList::~SQSQueueList(){
    StopAutoUpdate();
}

void SQSQueueList::LoadContent(){
    sqsService->ListQueues(prefixValue, tableWidget);
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

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("dark","delete"), "Delete Queue");
    deleteAction->setToolTip("Delete the Queue");

    // Conditional logic
    redriveAction->setEnabled(isDql);

    QString queueUrl = tableWidget->item(row, 7)->text();
    QString queueArn = tableWidget->item(row, 8)->text();
    QAction *selectedAction = menu.exec(tableWidget->viewport()->mapToGlobal(pos));
    if (selectedAction == purgeAction) {
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
