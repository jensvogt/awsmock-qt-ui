#include <modules/dynamodb/DynamoDbItemList.h>

#include "components/PageableList.h"

DynamoDbItemList::DynamoDbItemList(const QString &title, QWidget *parent) : BasePage(parent) {

    // Connect service
    _dynamoDbService = new DynamoDbService();
    connect(_dynamoDbService, &DynamoDbService::ListItemsSignal, this, &DynamoDbItemList::HandleListItemSignal);

    // Title label
    _titleLabel = new QLabel(title, this);

    // Define toolbar
    const auto toolBar = new QHBoxLayout();
    const auto spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar back action
    const auto backButton = new QPushButton(IconUtils::GetIcon("back"), "");
    backButton->setToolTip("Go back to table list");
    connect(backButton, &QPushButton::clicked, []() {
        emit EventBus::instance().RouteChanged("DynamoDB");
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
    toolBar->addWidget(_titleLabel);
    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);
    toolBar->addWidget(purgeAllButton);
    toolBar->addWidget(refreshButton);

    // Item
    _itemView = new PageableList(this);
    _itemView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _itemView->setStyleSheet(R"(QListView::item {border-bottom: 1px solid #5c5c5c;})");

    //Connect double-click
    connect(_itemView, &PageableList::DoubleClicked, this, [this](const QModelIndex &index) {

        // Extract ARN and URL
        const QString itemValue = _itemView->GetValue<QString>(index);

        // Open edit dialog
        //DynamoDbEditItemDialog dialog(itemName);
        //dialog.exec();


    });

    // Add context menu
    connect(_itemView, &PageableList::customContextMenuRequested, this, &DynamoDbItemList::ShowContextMenu);

    const auto mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(toolBar, 0); // fixed height
    mainLayout->addWidget(_itemView, 1); // expands to fill
}

DynamoDbItemList::~DynamoDbItemList() {
    StopAutoUpdate();
}

void DynamoDbItemList::LoadContent() {
    _tableName = GetArgument<QString>("tableName");
    _dynamoDbService->ListItems(_tableName, _itemView->GetPrefix(), 1000, 0);
    _titleLabel->setText("DynamoDB Item List: " + _tableName);
}

void DynamoDbItemList::HandleListItemSignal(const DynamoDbListItemResponse &listItemResponse) const {
    _itemView->Clear();
    _itemView->SetTotalSize(listItemResponse.count);
    for (auto r = 0; r < listItemResponse.items.count(); r++) {
        _itemView->Append(new QStandardItem(listItemResponse.items.at(r)));
    }
    //_itemView->
}

void DynamoDbItemList::ShowContextMenu(const QPoint &pos) const {

    // Cell index
    const QModelIndex index = _itemView->GetIndexFromPosition(pos);

    // Get container
    const auto itemName = _itemView->GetValue<QString>(index);
    //const auto itemArn = _itemView->GetValue<QString>(index, 5);

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

    if (const QAction *selectedAction = menu.exec(_itemView->GetGlobalPosition(pos));
        selectedAction == purgeAction) {
        //_dynamoDbService->PurgeBucket(bucketName);
    } else if (selectedAction == deleteAction) {
        //_dynamoDbService->DeleteItem(itemName);
    } else if (selectedAction == editAction) {
        //S3BucketEditDialog dialog(bucketName);
        //dialog.exec();
    }
}

