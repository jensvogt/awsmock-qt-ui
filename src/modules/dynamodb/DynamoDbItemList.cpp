#include <modules/dynamodb/DynamoDbItemList.h>

#include "components/PageableList.h"

DynamoDbItemList::DynamoDbItemList(const QString &title, QWidget *parent) : BasePage(parent) {

    // Connect service
    _moduleService = new ModuleService();
    _dynamoDbService = new DynamoDbService();
    connect(_dynamoDbService, &DynamoDbService::ListItemsSignal, this, &DynamoDbItemList::HandleListItemSignal);

    // Title label
    const auto titleLabel = new QLabel(title, this);

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

    // Toolbar export action
    const auto exportButton = new QPushButton(IconUtils::GetIcon("export"), "", this);
    exportButton->setToolTip("Export all items to a file");
    connect(exportButton, &QPushButton::clicked, this, [this]() {
        ExportItems();
    });

    toolBar->addWidget(backButton);
    toolBar->addWidget(titleLabel);
    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);
    toolBar->addWidget(purgeAllButton);
    toolBar->addWidget(exportButton);
    toolBar->addWidget(refreshButton);

    // Item
    _itemView = new PageableList(this);

    // Connect double-click
    /*connect(_itemView, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {
        // Get the position
        const int row = index.row();

        // Extract ARN and URL
        const QString itemName = _dataModel->item(row)->text();

        // Open edit dialog
        //DynamoDbEditItemDialog dialog(itemName);
        //dialog.exec();
    });*/

    // Add context menu
    connect(_itemView, &PageableList::ContextMenuRequested, this, &DynamoDbItemList::ShowContextMenu);

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(_itemView, 2);
}

DynamoDbItemList::~DynamoDbItemList() {
    StopAutoUpdate();
}

void DynamoDbItemList::LoadContent() {
    _tableName = GetArgument<QString>("tableName");
    _dynamoDbService->ListItems(_tableName, _itemView->GetPrefix(), _itemView->GetPageSize(), _itemView->GetPageIndex());
}

void DynamoDbItemList::ExportItems() const {
    // Create a QFileDialog set to select existing files
    const auto filter = "JSON Files (*.json);All Files (*.*)";
    const auto defaultDir = Configuration::instance().GetValue<QString>("ui.default-directory.ExportInfrastructure", "/usr/local/awsmock-qt-_ui");

    if (const QString filePath = QFileDialog::getSaveFileName(nullptr, "Open JSON File", defaultDir, filter); !filePath.isEmpty()) {
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            logWarning << "Failed to open file:" << file.errorString();
            return;
        }
        /*
        for (int i = 0; i < _dataModel->rowCount(); i++) {
            file.write(_dataModel->index(i, 0).data(Qt::DisplayRole).toString().toUtf8());
        }
        */
        file.close();
    }
}

void DynamoDbItemList::HandleListItemSignal(const DynamoDbListItemResponse &listItemResponse) {
    _itemView->Clear();

    for (auto r = 0; r < listItemResponse.items.count(); r++) {
        _itemView->Append(new QStandardItem(listItemResponse.items.at(r)));
    }
    NotifyStatusBar();
}

void DynamoDbItemList::ShowContextMenu(const QPoint &pos) const {

    // Cell index
    const QModelIndex index = _itemView->GetIndexFromPosition(pos);

    // Get container
    const QString itemName = _itemView->GetValue<QString>(index, 0);
    const QString itemArn = _itemView->GetValue<QString>(index, 5);

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

