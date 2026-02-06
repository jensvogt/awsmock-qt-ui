#include <modules/dynamodb/DynamoDbItemList.h>

DynamoDbItemList::DynamoDbItemList(const QString &title, QWidget *parent) : BasePage(parent) {

    // Connect service
    _dynamoDbService = new DynamoDbService();
    connect(_dynamoDbService, &DynamoDbService::ListItemsSignal, this, &DynamoDbItemList::HandleListItemSignal);
    //connect(_dynamoDbService, &DynamoDbService::ReloadItemListSignal, this, &DynamoDbItemList::LoadContent);

    // Title label
    const auto titleLabel = new QLabel(title, this);

    // Define toolbar
    const auto toolBar = new QHBoxLayout();
    const auto spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar back action
    const auto backButton = new QPushButton(IconUtils::GetIcon("back"), "");
    backButton->setToolTip("Go back to table list");
    connect(backButton, &QPushButton::clicked, [this]() {
        StopAutoUpdate();
        emit BackNavigationSignal();
    });

    // Toolbar add action
    const auto addButton = new QPushButton(IconUtils::GetIcon("add"), "", this);
    addButton->setToolTip("Add a new item");
    connect(addButton, &QPushButton::clicked, [this]() {

        // Initialize dialog
        //  DynamoDbAddItemDialog dialog;
        //        dialog.exec();

        // Get request
        //      const DynamoDbCreateItemRequest request = dialog.GetCreateItemRequest();
        //        _dynamoDbService->CreateItem(request);
    });

    // Toolbar add action
    const auto purgeAllButton = new QPushButton(IconUtils::GetIcon("purge"), "", this);
    purgeAllButton->setToolTip("Purge all items");
    connect(purgeAllButton, &QPushButton::clicked, [this]() {
        _dynamoDbService->PurgeTable(_tableName);
    });

    // Toolbar refresh action
    const auto refreshButton = new QPushButton(IconUtils::GetIcon("refresh"), "", this);
    refreshButton->setToolTip("Refresh the DynamoDb item list");
    connect(refreshButton, &QPushButton::clicked, this, [this]() {
        LoadContent();
    });

    toolBar->addWidget(backButton);
    toolBar->addWidget(titleLabel);
    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);
    toolBar->addWidget(purgeAllButton);
    toolBar->addWidget(refreshButton);

    // Prefix editor
    auto *prefixLayout = new QHBoxLayout();
    auto *prefixEdit = new QLineEdit(this);
    prefixEdit->setPlaceholderText("Prefix");
    prefixEdit->setToolTip("Prefix for the item ID");
    _prefixClear = new QPushButton(IconUtils::GetIcon("clear"), "", this);
    connect(prefixEdit, &QLineEdit::textChanged, this, [this,prefixEdit]() {
        _prefixClear->setDisabled(false);
        _proxyModel->setFilterColumn(0);
        _proxyModel->setFilterPrefix(prefixEdit->text());
    });
    prefixLayout->addWidget(prefixEdit);
    _prefixClear->setDisabled(true);
    _prefixClear->setToolTip("Clear the prefix for the item ID");
    connect(_prefixClear, &QPushButton::clicked, this, [this]() {
        _proxyModel->clearFilter();
        _prefixClear->setDisabled(true);
    });
    prefixLayout->addWidget(_prefixClear);

    // Item
    _itemView = new QListView(this);
    _itemView->setWordWrap(true);
    _itemView->setResizeMode(QListView::Adjust);
    _itemView->setUniformItemSizes(false);
    _itemView->setLineWidth(2);
    _itemView->setStyleSheet(R"(QListView::item {border-bottom: 1px solid #5c5c5c;})");
    _dataModel = new QStandardItemModel(_itemView);
    _itemView->setModel(_dataModel);

    // Connect double-click
    connect(_itemView, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {
        // Get the position
        const int row = index.row();

        // Extract ARN and URL
        const QString itemName = _dataModel->item(row)->text();

        // Open edit dialog
        //DynamoDbEditItemDialog dialog(itemName);
        //dialog.exec();
    });

    // Add context menu
    _itemView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_itemView, &QTableWidget::customContextMenuRequested, this, &DynamoDbItemList::ShowContextMenu);

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addLayout(prefixLayout, 0);
    layout->addWidget(_itemView, 2);
}

DynamoDbItemList::~DynamoDbItemList() {
    StopAutoUpdate();
}

void DynamoDbItemList::LoadContent() {
    _tableName = GetArgument<QString>("tableName");
    _dynamoDbService->ListItems(_tableName);
}

void DynamoDbItemList::HandleListItemSignal(const DynamoDbListItemResponse &listItemResponse) {
    _dataModel->removeRows(0, _dataModel->rowCount());

    for (auto r = 0; r < listItemResponse.items.count(); r++) {
        _dataModel->appendRow(new QStandardItem(listItemResponse.items.at(r)));
    }
    NotifyStatusBar();
}

void DynamoDbItemList::ShowContextMenu(const QPoint &pos) const {

    // Cell index
    const QModelIndex proxyIndex = _itemView->indexAt(pos);
    if (!proxyIndex.isValid()) return;

    const QModelIndex sourceIndex = _proxyModel->mapToSource(proxyIndex);

    // Get container
    const QString itemName = _dataModel->item(sourceIndex.row(), 0)->text();
    const QString itemArn = _dataModel->item(sourceIndex.row(), 5)->text();

    QMenu menu;
    menu.setToolTipsVisible(true);
    QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit Item");
    editAction->setToolTip("Edit the item details");

    menu.addSeparator();

    QAction *purgeAction = menu.addAction(IconUtils::GetIcon("purge"), "Purge Item");
    purgeAction->setToolTip("Purge the item");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Item");
    deleteAction->setToolTip("Delete the item");

    if (const QAction *selectedAction = menu.exec(_itemView->viewport()->mapToGlobal(pos));
        selectedAction == purgeAction) {
        //_dynamoDbService->PurgeBucket(bucketName);
    } else if (selectedAction == deleteAction) {
        //_dynamoDbService->DeleteItem(itemName);
    } else if (selectedAction == editAction) {
        //S3BucketEditDialog dialog(bucketName);
        //dialog.exec();
    }
}

