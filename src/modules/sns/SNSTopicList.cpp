#include <modules/sns/SNSTopicList.h>

SNSTopicList::SNSTopicList(const QString &title, QWidget *parent) : BasePage(parent) {

    setAttribute(Qt::WA_DeleteOnClose);

    // Set region
    _region = Configuration::instance().GetRegion();

    // Connect service
    snsService = new SNSService();
    connect(snsService, &SNSService::ListTopicSignal, this, &SNSTopicList::HandleListTopicSignal);
    connect(snsService, &SNSService::ReloadMessagesSignal, this, &SNSTopicList::LoadContent);

    // Title label
    const auto titleLabel = new QLabel(title, this);

    // Define toolbar
    const auto toolBar = new QHBoxLayout();
    toolBar->addWidget(titleLabel);
    const auto spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar add action
    const auto addButton = new QPushButton(IconUtils::GetIcon("dark", "add"), "", this);
    addButton->setIconSize(QSize(16, 16));
    addButton->setToolTip("Add a new Topic");
    connect(addButton, &QPushButton::clicked, [this]() {
        bool ok;
        if (const QString topicName = QInputDialog::getText(0, "Topic Name", "Topic name:", QLineEdit::Normal, "", &ok); ok && !topicName.isEmpty()) {
            snsService->AddTopic(_region, topicName);
        }
    });

    // Toolbar add action
    const auto purgeAllButton = new QPushButton(IconUtils::GetIcon("dark", "purge"), "", this);
    purgeAllButton->setIconSize(QSize(16, 16));
    purgeAllButton->setToolTip("Purge all Topics");
    connect(purgeAllButton, &QPushButton::clicked, [this]() {
        snsService->PurgeAllTopics();
    });

    // Toolbar refresh action
    const auto refreshButton = new QPushButton(IconUtils::GetIcon("dark", "refresh"), "", this);
    refreshButton->setIconSize(QSize(16, 16));
    refreshButton->setToolTip("Refresh the Topiclist");
    connect(refreshButton, &QPushButton::clicked, this, [this]() {
        LoadContent();
    });

    //    toolBar->addWidget(titleLabel);
    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);
    toolBar->addWidget(purgeAllButton);
    toolBar->addWidget(refreshButton);

    // Prefix editor
    auto prefixEdit = new QLineEdit(this);
    prefixEdit->setPlaceholderText("Prefix");
    connect(prefixEdit, &QLineEdit::returnPressed, this, [this, prefixEdit]() {
        prefixValue = prefixEdit->text();
        LoadContent();
    });

    // Table
    const QStringList headers = QStringList() << tr("Name")
                                << tr("Available")
                                << tr("Send")
                                << tr("Resend")
                                << tr("Size")
                                << tr("Created")
                                << tr("Modified")
                                << tr("TopicArn");

    tableWidget = new QTableWidget(this);

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

    // Connect double-click
    connect(tableWidget, &QTableView::doubleClicked, this, [=](const QModelIndex &index) {
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

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(prefixEdit, 0);
    layout->addWidget(tableWidget, 2);
}

SNSTopicList::~SNSTopicList() {
    StopAutoUpdate();
}

void SNSTopicList::LoadContent() {
    snsService->ListTopics(prefixValue);
}

void SNSTopicList::HandleListTopicSignal(const SNSListTopicResult &listTopicResult) {
    tableWidget->setRowCount(0);
    tableWidget->setSortingEnabled(false);
    tableWidget->sortItems(-1);
    for (auto r = 0; r < listTopicResult.topicCounters.count(); r++) {
        tableWidget->insertRow(r);
        tableWidget->setItem(r, 0, new QTableWidgetItem(listTopicResult.topicCounters.at(r).topicName));

        const auto item1 = new QTableWidgetItem();
        item1->setData(Qt::EditRole, QVariant::fromValue(listTopicResult.topicCounters.at(r).messages));
        tableWidget->setItem(r, 1, item1);

        const auto item2 = new QTableWidgetItem();
        item2->setData(Qt::EditRole, QVariant::fromValue(listTopicResult.topicCounters.at(r).messagesSend));
        tableWidget->setItem(r, 2, item2);

        const auto item3 = new QTableWidgetItem();
        item3->setData(Qt::EditRole, QVariant::fromValue(listTopicResult.topicCounters.at(r).messagesResend));
        tableWidget->setItem(r, 3, item3);

        const auto item4 = new QTableWidgetItem();
        item4->setData(Qt::EditRole, QVariant::fromValue(listTopicResult.topicCounters.at(r).size));
        tableWidget->setItem(r, 4, item4);

        tableWidget->setItem(
            r, 5, new QTableWidgetItem(listTopicResult.topicCounters.at(r).created.toString(Qt::ISODate)));
        tableWidget->setItem(
            r, 6, new QTableWidgetItem(listTopicResult.topicCounters.at(r).modified.toString(Qt::ISODate)));
        tableWidget->setItem(r, 7, new QTableWidgetItem(listTopicResult.topicCounters.at(r).topicArn));
    }
    tableWidget->setRowCount(listTopicResult.topicCounters.count());
    tableWidget->setSortingEnabled(true);
    NotifyStatusBar();
}

void SNSTopicList::ShowContextMenu(const QPoint &pos) const {
    const QModelIndex index = tableWidget->indexAt(pos);
    if (!index.isValid()) return;

    const int row = index.row();

    QMenu menu;
    QAction *editAction = menu.addAction(QIcon(":/icons/edit.png"), "Edit Topic");
    editAction->setToolTip("Edit the Topic details");

    menu.addSeparator();

    QAction *purgeAction = menu.addAction(QIcon(":/icons/purge.png"), "Purge Topic");
    purgeAction->setToolTip("Purge the Topic");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("dark", "delete"), "Delete Topic");
    deleteAction->setToolTip("Delete the Topic");

    const QString topicArn = tableWidget->item(row, 7)->text();
    if (const QAction *selectedAction = menu.exec(tableWidget->viewport()->mapToGlobal(pos)); selectedAction == purgeAction) {
        snsService->PurgeTopic(topicArn);
    } else if (selectedAction == deleteAction) {
        snsService->DeleteTopic(topicArn);
    } else if (selectedAction == editAction) {
        if (SNSTopicDetailsDialog dialog(topicArn); dialog.exec() == QDialog::Accepted) {
            qDebug() << "SNS Topic edit dialog exit";
        }
    }
}
