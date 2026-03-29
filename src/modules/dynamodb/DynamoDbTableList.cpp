#include <modules/dynamodb/DynamoDbTableList.h>

#include "../../../include/modules/dynamodb/DynamoDbExportTableDialog.h"

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

    // Toolbar reset counter action
    auto *resetCounterButton = new QPushButton(IconUtils::GetIcon("reset-counter"), "", this);
    resetCounterButton->setToolTip("Refresh the item counters");
    connect(resetCounterButton, &QPushButton::clicked, [this]() {
        _dynamoDbService->ResetItemCounters();
        LoadContent();
    });

    //    toolBar->addWidget(titleLabel);
    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);
    toolBar->addWidget(purgeAllButton);
    toolBar->addWidget(resetCounterButton);
    toolBar->addWidget(refreshButton);

    // Table
    const QStringList headers = QStringList() = {tr("Name"), tr("Items"), tr("Size"), tr("Created"), tr("Modified"), tr("TableArn")};

    // Table
    _tableView = new PageableTable(this);
    _tableView->SetHeaderNames(headers);
    _tableView->SetResizeModes({QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents});
    _tableView->SetHiddenColumns({5});
    _tableView->SetSorting(1, "itemCount", -1);

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

void DynamoDbTableList::HandleListTableSignal(const DynamoDbListTableResponse &listTableResponse) {

    // Get the ids
    auto view = listTableResponse.tableCounters | std::views::transform(&DynamoDbTableCounter::tableName);
    const QVector<QString> names(view.begin(), view.end());

    // Get the rows
    QHash<QString, int> idToRow = _tableView->GetRowIndexesFromIds(names);

    _tableView->SetTotalSize(listTableResponse.total);
    for (auto r = 0, c = 0; r < listTableResponse.tableCounters.count(); r++, c = 0) {
        const int row = idToRow.contains(listTableResponse.tableCounters.at(r).tableName) ? idToRow[listTableResponse.tableCounters.at(r).tableName] : _tableView->GetSize();
        if (r == _tableView->GetSize() || _internalData.at(r) != listTableResponse.tableCounters.at(r)) {
            _tableView->SetColumn(row, c++, listTableResponse.tableCounters.at(r).tableName);
            _tableView->SetColumn(row, c++, listTableResponse.tableCounters.at(r).itemCount);
            _tableView->SetColumn(row, c++, StringUtils::FormatSizeColumn(listTableResponse.tableCounters.at(r).size, 1), Qt::AlignRight | Qt::AlignVCenter);
            _tableView->SetColumn(row, c++, listTableResponse.tableCounters.at(r).created);
            _tableView->SetColumn(row, c++, listTableResponse.tableCounters.at(r).modified);
            _tableView->SetHiddenColumn(row, c, listTableResponse.tableCounters.at(r).tableArn);
        }
    }
    _internalData = listTableResponse.tableCounters;
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

    QAction *exportAction = menu.addAction(IconUtils::GetIcon("export"), "Export Table Data");
    exportAction->setToolTip("Export the table items");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Table");
    deleteAction->setToolTip("Delete the table");

    if (const QAction *selectedAction = menu.exec(_tableView->GetGlobalPosition(pos)); selectedAction == purgeAction) {
        _dynamoDbService->PurgeTable(tableName);
    } else if (selectedAction == exportAction) {
        DynamoDbExportTableDialog dialog(tableName);
        dialog.exec();
    } else if (selectedAction == deleteAction) {
        _dynamoDbService->DeleteTable(tableName);
    } else if (selectedAction == editAction) {
        DynamoDbEditTableDialog dialog(tableName);
        dialog.exec();
    }
}
