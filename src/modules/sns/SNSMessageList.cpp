
#include <modules/sns/SNSMessageList.h>

#include "utils/IconUtils.h"

SNSMessageList::SNSMessageList(const QString& title, const QString& topicArn, QWidget *parent) : BasePage(parent), topicArn(topicArn)
{
    // Connect service
    snsService = new SNSService();
    connect(snsService, &SNSService::ListMessagesSignal, this, &SNSMessageList::HandleListMessageSignal);
    connect(snsService, &SNSService::ReloadMessagesSignal, this, &SNSMessageList::HandleReloadMessageSignal);

    // Toolbar
    QHBoxLayout *toolBar = new QHBoxLayout();
    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar back action
    QPushButton *backButton = new QPushButton(IconUtils::GetIcon("dark", "back"),"");
    backButton->setIconSize(QSize(16, 16));
    backButton->setToolTip("Add a new Queue");
    QObject::connect(backButton, &QPushButton::clicked, [this](){
        OnBackClicked();
    });

    // Toolbar label
    QLabel* titleLabel = new QLabel(title);

    // Toolbar add action
    QPushButton *addButton = new QPushButton(IconUtils::GetIcon("dark", "add"),"");
    addButton->setIconSize(QSize(16, 16));
    addButton->setToolTip("Add a new Queue");
    QObject::connect(addButton, &QPushButton::clicked, [](){
        bool ok;
        QString text = QInputDialog::getText(0, "Queue Name", "Queue name:", QLineEdit::Normal, "", &ok);
        if (ok && !text.isEmpty()) {
            // AddQueue(text);
        }
    });

    // Toolbar add action
    QPushButton *purgeAllButton = new QPushButton(IconUtils::GetIcon("dark", "purge"),"");
    purgeAllButton->setIconSize(QSize(16, 16));
    purgeAllButton->setToolTip("Purge all Queues");
    QObject::connect(purgeAllButton, &QPushButton::clicked, [&](){
        qDebug() << "Purge topic: "<<topicArn;
        snsService->PurgeTopic(topicArn);
    });

    // Toolbar refresh action
    QPushButton *refreshButton = new QPushButton(IconUtils::GetIcon("dark", "refresh"),"");
    refreshButton->setIconSize(QSize(16, 16));
    refreshButton->setToolTip("Refresh the Queuelist");
    QObject::connect(refreshButton, &QPushButton::clicked, [this](){
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
                                        << tr("Status")
                                        << tr("Created")
                                        << tr("Modified")
                                        << tr("LastSend")
                                        << tr("TopicArn");

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

    // COnnect double-click
    connect(tableWidget, &QTableView::doubleClicked, this, [=](const QModelIndex &index) {

        // Get the position
        int row = index.row();
        int col = index.column();

        QString messageId = tableWidget->item(row, 0)->text();
        SNSMessageDetailsDialog dialog(messageId);
        if (dialog.exec() == QDialog::Accepted) {
            qDebug() << "SQS Queue edit dialog exit";
        }
    });

    // Add context menu
    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableWidget, &QTableWidget::customContextMenuRequested, this, &SNSMessageList::ShowContextMenu);

    // Set up the layout for the individual content pages
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(prefixEdit, 1);
    layout->addWidget(tableWidget, 2);
    layout->addStretch();
    layout->stretch(1);
}

SNSMessageList::~SNSMessageList(){
    StopAutoUpdate();
}

void SNSMessageList::LoadContent(){
    snsService->ListMessages(topicArn, prefixValue);
    NotifyStatusBar();
}

void SNSMessageList::HandleListMessageSignal(const SNSListMessagesResult &listMessageResult) {

    tableWidget->setRowCount(0);
    tableWidget->setSortingEnabled(false);
    tableWidget->sortItems(-1);
    for (auto r = 0; r < listMessageResult.messageCounters.count(); r++) {

        tableWidget->insertRow(r);
        tableWidget->setItem(r, 0, new QTableWidgetItem(listMessageResult.messageCounters.at(r).messageId));
        tableWidget->setItem(r, 1, new QTableWidgetItem(listMessageResult.messageCounters.at(r).contentType));

        QTableWidgetItem *item2 = new QTableWidgetItem;
        item2->setData(Qt::EditRole, QVariant::fromValue(listMessageResult.messageCounters.at(r).size));
        tableWidget->setItem(r, 2, item2);

        tableWidget->setItem(r, 3, new QTableWidgetItem(listMessageResult.messageCounters.at(r).messageStatus));

        tableWidget->setItem(r, 4, new QTableWidgetItem(listMessageResult.messageCounters.at(r).created.toString(Qt::ISODate)));
        tableWidget->setItem(r, 5, new QTableWidgetItem(listMessageResult.messageCounters.at(r).modified.toString(Qt::ISODate)));
        tableWidget->setItem(r, 6, new QTableWidgetItem(listMessageResult.messageCounters.at(r).lastSend.toString(Qt::ISODate)));
        tableWidget->setItem(r, 7, new QTableWidgetItem(listMessageResult.messageCounters.at(r).topicArn));
    }
    tableWidget->setRowCount(listMessageResult.messageCounters.count());
    tableWidget->setSortingEnabled(true);
    NotifyStatusBar();
}

void SNSMessageList::HandleReloadMessageSignal(){
    snsService->ListMessages(topicArn, prefixValue);
    NotifyStatusBar();
}

void SNSMessageList::ShowContextMenu(const QPoint &pos){

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
        const QString id = tableWidget->item(row, 0)->text();
        qDebug() << "Delete " << id;
        //        DeleteQueue(QueueUrl);
    }
}
