#include <ui_DynamoDbAddTableDialog.h>
#include <modules/dynamodb/DynamoDBTableList.h>

DynamoDbTableList::DynamoDbTableList(const QString &title, QWidget *parent) : BasePage(parent) {
    // Set region
    _region = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");

    // Connect service
    _dynamoDbService = new DynamoDbService();
    connect(_dynamoDbService, &DynamoDbService::ListTablesSignal, this, &DynamoDbTableList::HandleListTableSignal);
    //connect(_s3Service, &S3Service::ReloadBucketListSignal, this, &DynamoDbTableList::LoadContent);

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
    connect(addButton, &QPushButton::clicked, []() {
        DynamoDbAddTableDialog dialog;
        dialog.exec();
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
    _tableWidget = new QTableView(this);
    _dataModel = new QStandardItemModel(this);
    _dataModel->setHorizontalHeaderLabels(headers);
    _dataModel->setColumnCount(static_cast<int>(headers.count()));

    // Proxy model for prefix filtering
    _proxyModel = new PrefixFilterProxyModel(this);
    _proxyModel->setSourceModel(_dataModel);
    _tableWidget->setModel(_proxyModel);

    _tableWidget->setShowGrid(true);
    _tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    _tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    _tableWidget->setSortingEnabled(true);
    _tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    _tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    _tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    _tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    _tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    _tableWidget->setColumnHidden(5, true);
    _tableWidget->addAction(GetRefreshAction(this));

    // Connect double-click
    connect(_tableWidget, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {
        // Get the position
        const int row = index.row();

        // Extract ARN and URL
        //const QString bucketName = _tableWidget->item(row, 0)->text();

        // Send notification
        //emit ShowS3Objects(bucketName);
    });

    // Add context menu
    _tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_tableWidget, &QTableWidget::customContextMenuRequested, this, &DynamoDbTableList::ShowContextMenu);

    // Save sort column
    const QHeaderView *header = _tableWidget->horizontalHeader();
    connect(header, &QHeaderView::sortIndicatorChanged, this, [this](const int column, const Qt::SortOrder order) {
        _sortColumn = column;
        _sortOrder = order;
    });

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addLayout(prefixLayout, 0);
    layout->addWidget(_tableWidget, 2);

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
    const int selectedRow = _tableWidget->selectionModel()->currentIndex().row();
    _tableWidget->setSortingEnabled(false);

    for (auto r = 0, c = 0; r < listTableResponse.tableCounters.count(); r++, c = 0) {
        SetColumn(_dataModel, r, c++, listTableResponse.tableCounters.at(r).tableName);
        SetColumn(_dataModel, r, c++, listTableResponse.tableCounters.at(r).itemCount);
        SetColumn(_dataModel, r, c++, listTableResponse.tableCounters.at(r).size / 1024);
        SetColumn(_dataModel, r, c++, listTableResponse.tableCounters.at(r).created);
        SetColumn(_dataModel, r, c++, listTableResponse.tableCounters.at(r).modified);
        SetColumn(_dataModel, r, c++, listTableResponse.tableCounters.at(r).tableArn);
    }
    // Reset selection
    _tableWidget->setSortingEnabled(true);
    _tableWidget->sortByColumn(_sortColumn, _sortOrder);
    _tableWidget->selectRow(selectedRow);

    NotifyStatusBar();
}

void DynamoDbTableList::ShowContextMenu(const QPoint &pos) const {

    // Cell index
    const QModelIndex proxyIndex = _tableWidget->indexAt(pos);
    if (!proxyIndex.isValid()) return;

    const QModelIndex sourceIndex = _proxyModel->mapToSource(proxyIndex);

    // Get container
    const QString tableName = _dataModel->item(sourceIndex.row(), 0)->text();
    const QString tableArn = _dataModel->item(sourceIndex.row(), 5)->text();

    QMenu menu;
    QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit Table");
    editAction->setToolTip("Edit the table details");

    menu.addSeparator();

    QAction *purgeAction = menu.addAction(IconUtils::GetIcon("purge"), "Purge Table");
    purgeAction->setToolTip("Purge the table");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Table");
    deleteAction->setToolTip("Delete the table");

    if (const QAction *selectedAction = menu.exec(_tableWidget->viewport()->mapToGlobal(pos));
        selectedAction == purgeAction) {
        //_dynamoDbService->PurgeBucket(bucketName);
    } else if (selectedAction == deleteAction) {
        _dynamoDbService->DeleteTable(tableName);
    } else if (selectedAction == editAction) {
        //S3BucketEditDialog dialog(bucketName);
        //dialog.exec();
    }
}
