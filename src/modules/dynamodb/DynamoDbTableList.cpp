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

    // Prefix editor
    auto *prefixLayout = new QHBoxLayout();
    auto *prefixEdit = new QLineEdit(this);
    prefixEdit->setPlaceholderText("Prefix");
    _prefixClear = new QPushButton(IconUtils::GetIcon("clear"), "", this);
    connect(prefixEdit, &QLineEdit::textChanged, this, [this,prefixEdit]() {
        _prefixClear->setDisabled(false);
        _proxyModel->setFilterColumn(0);
        _proxyModel->setFilterPrefix(prefixEdit->text());
    });
    prefixLayout->addWidget(prefixEdit);
    _prefixClear->setDisabled(true);
    connect(_prefixClear, &QPushButton::clicked, this, [this]() {
        _proxyModel->clearFilter();
        _prefixClear->setDisabled(true);
    });
    prefixLayout->addWidget(_prefixClear);

    // Table
    const QStringList headers = QStringList() = {tr("Name"), tr("Items"), tr("Size [kb]"), tr("Created"), tr("Modified"), tr("TableArn")};

    // Table
    _tableView = new QTableView(this);
    _dataModel = new QStandardItemModel(this);
    _dataModel->setHorizontalHeaderLabels(headers);
    _dataModel->setColumnCount(static_cast<int>(headers.count()));

    // Proxy model for prefix filtering
    _proxyModel = new PrefixFilterProxyModel(this);
    _proxyModel->setSourceModel(_dataModel);
    _tableView->setModel(_proxyModel);

    _tableView->setShowGrid(true);
    _tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    _tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _tableView->setSortingEnabled(true);
    _tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    _tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    _tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    _tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    _tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    _tableView->setColumnHidden(5, true);
    _tableView->addAction(GetRefreshAction(this));

    // Connect double-click
    connect(_tableView, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {

        const QModelIndex sourceIndex = _proxyModel->mapToSource(index);

        // Get container
        const QString tableName = _dataModel->item(sourceIndex.row(), 0)->text();

        // Send notification to main window
        emit ShowItemsSignal(tableName);
    });

    // Add context menu
    _tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_tableView, &QTableWidget::customContextMenuRequested, this, &DynamoDbTableList::ShowContextMenu);

    // Save sort column
    connect(_tableView->horizontalHeader(), &QHeaderView::sortIndicatorChanged, this, [this](const int column, const Qt::SortOrder order)-> void {
        this->_sortColumn = column;
        this->_sortOrder = order;
    });

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addLayout(prefixLayout, 0);
    layout->addWidget(_tableView, 2);

    // Initialization
    LoadContent();
}

DynamoDbTableList::~DynamoDbTableList() {
    StopAutoUpdate();
}

void DynamoDbTableList::LoadContent() {
    if (Configuration::instance().GetConnectionState()) {
        _dynamoDbService->ListTables(_prefixValue);
    } else {
        QMessageBox::critical(nullptr, "Error", "Backend is not reachable");
    }
}

void DynamoDbTableList::HandleListTableSignal(const DynamoDbListTableResponse &listTableResponse) {
    const int selectedRow = _tableView->selectionModel()->currentIndex().row();
    _tableView->setSortingEnabled(false);
    _dataModel->removeRows(0, _dataModel->rowCount());

    for (auto r = 0, c = 0; r < listTableResponse.tableCounters.count(); r++, c = 0) {
        SetColumn(_dataModel, r, c++, listTableResponse.tableCounters.at(r).tableName);
        SetColumn(_dataModel, r, c++, listTableResponse.tableCounters.at(r).itemCount);
        SetColumn(_dataModel, r, c++, listTableResponse.tableCounters.at(r).size / 1024);
        SetColumn(_dataModel, r, c++, listTableResponse.tableCounters.at(r).created);
        SetColumn(_dataModel, r, c++, listTableResponse.tableCounters.at(r).modified);
        SetColumn(_dataModel, r, c++, listTableResponse.tableCounters.at(r).tableArn);
    }
    // Reset selection
    _tableView->setSortingEnabled(true);
    _tableView->sortByColumn(_sortColumn, _sortOrder);
    _tableView->selectRow(selectedRow);

    NotifyStatusBar();
}

void DynamoDbTableList::ShowContextMenu(const QPoint &pos) const {

    // Cell index
    const QModelIndex proxyIndex = _tableView->indexAt(pos);
    if (!proxyIndex.isValid()) return;

    const QModelIndex sourceIndex = _proxyModel->mapToSource(proxyIndex);

    // Get container
    const QString tableName = _dataModel->item(sourceIndex.row(), 0)->text();
    // const QString tableArn = _dataModel->item(sourceIndex.row(), 5)->text();

    QMenu menu;
    QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit Table");
    editAction->setToolTip("Edit the table details");

    menu.addSeparator();

    QAction *purgeAction = menu.addAction(IconUtils::GetIcon("purge"), "Purge Table");
    purgeAction->setToolTip("Purge the table");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Table");
    deleteAction->setToolTip("Delete the table");

    if (const QAction *selectedAction = menu.exec(_tableView->viewport()->mapToGlobal(pos)); selectedAction == purgeAction) {
        _dynamoDbService->PurgeTable(tableName);
    } else if (selectedAction == deleteAction) {
        _dynamoDbService->DeleteTable(tableName);
    } else if (selectedAction == editAction) {
        DynamoDbEditTableDialog dialog(tableName);
        dialog.exec();
    }
}
