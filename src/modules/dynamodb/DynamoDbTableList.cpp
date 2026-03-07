#include <modules/dynamodb/DynamoDbTableList.h>

DynamoDbTableList::DynamoDbTableList(const QString &title, QWidget *parent) : BasePage(parent) {

    // Set region
    _region = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");

    // Connect service
    _dynamoDbService = new DynamoDbService();
    connect(_dynamoDbService, &DynamoDbService::ListTablesSignal, this, &DynamoDbTableList::HandleListTableSignal);
    connect(_dynamoDbService, &DynamoDbService::ReloadTableListSignal, this, &DynamoDbTableList::LoadContent);

    // Title label
    const auto titleLabel = new QLabel(title, this);

    // Define toolbar
    const auto toolBar = new QHBoxLayout();
    toolBar->addWidget(titleLabel);
    const auto spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar add action
    const auto addButton = new QPushButton(IconUtils::GetIcon("add"), "", this);
    addButton->setToolTip("Add a new table");
    connect(addButton, &QPushButton::clicked, [this]() {

        // Initialize dialog
        DynamoDbAddTableDialog dialog;
        dialog.exec();

        // Get request
        const DynamoDbCreateTableRequest request = dialog.GetCreateTableRequest();
        _dynamoDbService->CreateTable(request);
    });

    // Toolbar add action
    const auto purgeAllButton = new QPushButton(IconUtils::GetIcon("purge"), "", this);
    purgeAllButton->setToolTip("Purge all tables");
    connect(purgeAllButton, &QPushButton::clicked, [this]() {
        //  _s3Service->PurgeAllBuckets();
    });

    // Toolbar refresh action
    const auto refreshButton = new QPushButton(IconUtils::GetIcon("refresh"), "", this);
    refreshButton->setToolTip("Refresh the DynamoDb table list");
    connect(refreshButton, &QPushButton::clicked, this, [this]() {
        LoadContent();
    });

    //    toolBar->addWidget(titleLabel);
    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);
    toolBar->addWidget(purgeAllButton);
    toolBar->addWidget(refreshButton);

    // Table
    const QStringList headers = QStringList() = {tr("Name"), tr("Items"), tr("Size [kb]"), tr("Created"), tr("Modified"), tr("TableArn")};

    // Table
    _tableView = new PageableTable(this);
    _tableView->SetHeaderNames(headers);
    _tableView->SetResizeModes({QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents});
    _tableView->SetHiddenColumns({5});
    _tableView->SetSorting(1, "keys", -1);

    // Connect double-click
    connect(_tableView, &PageableTable::DoubleClicked, this, [this](const QModelIndex &index) {

        // Extract ARN and URL
        QMap<QString, QString> arguments;
        arguments["tableName"] = _tableView->GetValue<QString>(index, 0);

        // Send notification
        emit EventBus::instance().RouteChanged("DynamoDB Item List", arguments);
    });

    // Add context menu
    connect(_tableView, &PageableTable::ContextMenuRequested, this, &DynamoDbTableList::ShowContextMenu);

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(_tableView, 2);
}

DynamoDbTableList::~DynamoDbTableList() {
    StopAutoUpdate();
}

void DynamoDbTableList::LoadContent() {
    _dynamoDbService->ListTables(_prefixValue);
}

void DynamoDbTableList::HandleListTableSignal(const DynamoDbListTableResponse &listTableResponse) const {
    _tableView->Clear();
    _tableView->SetTotalSize(listTableResponse.total);
    for (auto r = 0, c = 0; r < listTableResponse.tableCounters.count(); r++, c = 0) {
        _tableView->SetColumn(r, c++, listTableResponse.tableCounters.at(r).tableName);
        _tableView->SetColumn(r, c++, listTableResponse.tableCounters.at(r).itemCount);
        _tableView->SetColumn(r, c++, listTableResponse.tableCounters.at(r).size / 1024);
        _tableView->SetColumn(r, c++, listTableResponse.tableCounters.at(r).created);
        _tableView->SetColumn(r, c++, listTableResponse.tableCounters.at(r).modified);
        _tableView->SetHiddenColumn(r, c++, listTableResponse.tableCounters.at(r).tableArn);
    }
    _tableView->UpdateSorting();
}

void DynamoDbTableList::ShowContextMenu(const QPoint &pos) const {

    const QModelIndex index = _tableView->GetIndexFromPosition(pos);

    // Get container
    const auto tableName = _tableView->GetValue<QString>(index, 0);
    const auto tableArn = _tableView->GetValue<QString>(index, 5);

    QMenu menu;
    menu.setToolTipsVisible(true);
    QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit Table");
    editAction->setToolTip("Edit the table details");

    menu.addSeparator();

    QAction *purgeAction = menu.addAction(IconUtils::GetIcon("purge"), "Purge Table");
    purgeAction->setToolTip("Purge the table");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Table");
    deleteAction->setToolTip("Delete the table");

    if (const QAction *selectedAction = menu.exec(_tableView->GetGlobalPosition(pos)); selectedAction == purgeAction) {
        _dynamoDbService->PurgeTable(tableName);
    } else if (selectedAction == deleteAction) {
        _dynamoDbService->DeleteTable(tableName);
    } else if (selectedAction == editAction) {
        DynamoDbEditTableDialog dialog(tableName);
        dialog.exec();
    }
}
