
#include <modules/sqs/SQSMessageList.h>

#include "utils/IconUtils.h"

SQSMessageList::SQSMessageList(const QString& title, const QString& queueArn, const QString& queueUrl, QWidget *parent) : BasePage(parent), _queueArn(queueArn), _queueUrl(queueUrl)
{
    m_netManager = new QNetworkAccessManager(this);

    const auto toolBar = new QHBoxLayout();
    const auto spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar back action
    const auto backButton = new QPushButton(IconUtils::GetIcon("dark", "back"),"");
    backButton->setIconSize(QSize(16, 16));
    backButton->setToolTip("Add a new Queue");
    connect(backButton, &QPushButton::clicked, [this](){
        OnBackClicked();
    });

    // Toolbar label
    const auto titleLabel = new QLabel(title);

    // Toolbar add action
    const auto addButton = new QPushButton(IconUtils::GetIcon("dark", "add"),"");
    addButton->setIconSize(QSize(16, 16));
    addButton->setToolTip("Add a new Queue");
    connect(addButton, &QPushButton::clicked, [](){
        bool ok;
        if (const QString text = QInputDialog::getText(0, "Queue Name", "Queue name:", QLineEdit::Normal, "", &ok); ok && !text.isEmpty()) {
           // AddQueue(text);
        }
    });

    // Toolbar add action
    const auto purgeAllButton = new QPushButton(IconUtils::GetIcon("dark", "purge"),"");
    purgeAllButton->setIconSize(QSize(16, 16));
    purgeAllButton->setToolTip("Purge all Queues");
    connect(purgeAllButton, &QPushButton::clicked, [&](){
        sqsService.PurgeAllMessages(queueUrl);
    });

    // Toolbar refresh action
    const auto refreshButton = new QPushButton(IconUtils::GetIcon("dark", "refresh"),"");
    refreshButton->setIconSize(QSize(16, 16));
    refreshButton->setToolTip("Refresh the Queuelist");
    connect(refreshButton, &QPushButton::clicked, [this](){
        LoadContent();
    });

    toolBar->addWidget(backButton);
    toolBar->addWidget(titleLabel);
    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);
    toolBar->addWidget(purgeAllButton);
    toolBar->addWidget(refreshButton);

    // Prefix editor
    QLineEdit* prefixEdit = new QLineEdit(this);
    prefixEdit->setPlaceholderText("Prefix");
    connect(prefixEdit, &QLineEdit::returnPressed, this, [this,prefixEdit]() {
        prefixValue = prefixEdit->text();
        LoadContent();
    });

    // Table
    QStringList headers = QStringList() << tr("ID")
                                        << tr("ContentType")
                                        << tr("Size")
                                        << tr("Retries")
                                        << tr("Created")
                                        << tr("Modified")
                                        << tr("QueueUrl")
                                        << tr("QueueArn");

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
    tableWidget->setColumnHidden(6, true);
    tableWidget->setColumnHidden(7, true);

    // Connect double-click
    connect(tableWidget, &QTableView::doubleClicked, this, [=](const QModelIndex &index) {

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

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(prefixEdit, 1);
    layout->addWidget(tableWidget, 2);
    layout->addStretch();
    layout->stretch(1);
}

SQSMessageList::~SQSMessageList(){
    StopAutoUpdate();
}

void SQSMessageList::LoadContent(){
    sqsService.ListMessages(_queueArn, prefixValue, tableWidget);
    NotifyStatusBar();
}

void SQSMessageList::ShowContextMenu(const QPoint &pos){

    QModelIndex index = tableWidget->indexAt(pos);
    if (!index.isValid()) return;

    int row = index.row();

    QMenu menu;
/*    QAction *purgeAction = menu.addAction(QIcon(":/icons/purge.png"), "Purge Queue");
    purgeAction->setToolTip("Purge the Queue");
    QAction *redriveAction = menu.addAction(QIcon(":/icons/redrive.png"), "Redrive Queue");
    redriveAction->setToolTip("Redrive all messages");*/
    menu.addSeparator();
    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("dark","delete"), "Delete Message");
    deleteAction->setToolTip("Delete the message");

    QAction *selectedAction = menu.exec(tableWidget->viewport()->mapToGlobal(pos));
    /*if (selectedAction == purgeAction) {
        QString QueueUrl = tableWidget->item(row, 7)->text();
//        PurgeQueue(QueueUrl);
    } else if (selectedAction == redriveAction) {
        QString QueueUrl = tableWidget->item(row, 7)->text();
//        DeleteQueue(QueueUrl);
    } else*/ if (selectedAction == deleteAction) {
        QString id = tableWidget->item(row, 0)->text();
        qDebug() << "Delete " << id;
//        DeleteQueue(QueueUrl);
    }
}
