#include <modules/dynamodb/DynamoDbItemList.h>

#include "components/PageableList.h"
#include "components/PageableTable.h"
#include "modules/dynamodb/DynamoDbItemDialog.h"
#include "utils/StringUtils.h"

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

    // Item table
    const QStringList headers = QStringList() = {tr("Partition Key"), tr("Sort Key"), tr("Size"), tr("Created"), tr("Modified"), tr("TableName")};
    _itemView = new PageableTable(this);
    _itemView->SetHeaderNames(headers);
    _itemView->SetResizeModes({QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents});
    _itemView->SetHiddenColumns({5});
    _itemView->SetSorting(1, "sortKey", -1);
    _itemView->SetSearchFieldPlaceholder("Partition key");

    // Connect paging changes
    connect(_itemView, &PageableTable::ReloadTable, this, &DynamoDbItemList::LoadContent);

    // Connect double-click
    connect(_itemView, &PageableTable::DoubleClicked, this, [this](const QModelIndex &index) {

        // Extract partition key and sort key
        const QString partitionKey = _itemView->GetValue<QString>(index, 0);
        const QString sortKey = _itemView->GetValue<QString>(index, 1);
        const QString tableName = _itemView->GetValue<QString>(index, 5);

        // Open edit dialog
        DynamoDbItemDialog dialog(tableName, partitionKey, sortKey);
        dialog.exec();
    });

    // Add context menu
    connect(_itemView, &PageableTable::ContextMenuRequested, this, &DynamoDbItemList::ShowContextMenu);

    const auto mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(toolBar, 0); // fixed height
    mainLayout->addWidget(_itemView, 1); // expands to fill
}

DynamoDbItemList::~DynamoDbItemList() {
    StopAutoUpdate();
}

void DynamoDbItemList::LoadContent() {
    _tableName = GetArgument<QString>("tableName");
    _dynamoDbService->ListItems(_tableName, _itemView->GetPrefix(), _itemView->GetPageSize(), _itemView->GetPageIndex(), _itemView->GetSortAttribute(), _itemView->GetSortDirection());
    _titleLabel->setText("DynamoDB Item List: " + _tableName);
}

void DynamoDbItemList::HandleListItemSignal(const DynamoDbListItemResponse &listItemResponse) const {
    _itemView->Clear();
    _itemView->SetTotalSize(listItemResponse.total);
    for (auto row = 0, c = 0; row < listItemResponse.items.count(); row++, c = 0) {
        _itemView->SetColumn(row, c++, listItemResponse.items.at(row).partitionKey);
        _itemView->SetColumn(row, c++, listItemResponse.items.at(row).sortKey);
        _itemView->SetColumn(row, c++, StringUtils::FormatSizeColumn(listItemResponse.items.at(row).size, 1), Qt::AlignRight | Qt::AlignVCenter);
        _itemView->SetColumn(row, c++, listItemResponse.items.at(row).created);
        _itemView->SetColumn(row, c++, listItemResponse.items.at(row).modified);
        _itemView->SetHiddenColumn(row, c++, listItemResponse.items.at(row).tableName);
    }
}

void DynamoDbItemList::ShowContextMenu(const QPoint &pos) const {

    // Cell index
    const QModelIndex index = _itemView->GetIndexFromPosition(pos);

    // Get container
    //const auto itemName = _itemView->GetValue<QString>(index);
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

