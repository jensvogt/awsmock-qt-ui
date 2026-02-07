#include <modules/kms/KMSKeyList.h>

#include "modules/kms/KMSKeyDialog.h"

KMSKeyList::KMSKeyList(const QString &title, QWidget *parent) : BasePage(parent) {
    // Set region
    _region = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");

    // Connect service
    _kmsService = new KMSService();
    connect(_kmsService, &KMSService::ListKeysSignal, this, &KMSKeyList::HandleListKeysSignal);
    connect(_kmsService, &KMSService::ReloadKeySignal, this, &KMSKeyList::LoadContent);

    // Title label
    const auto titleLabel = new QLabel(title, this);

    // Define toolbar
    const auto toolBar = new QHBoxLayout();
    toolBar->addWidget(titleLabel);
    const auto spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar add action
    const auto addButton = new QPushButton(IconUtils::GetIcon("add"), "", this);
    addButton->setToolTip("Add a new key");
    connect(addButton, &QPushButton::clicked, [this]() {

        // Initialize dialog
        KMSKeyDialog dialog(this);
        dialog.exec();

        //
        // // Get request
        // const DynamoDbCreateTableRequest request = dialog.GetCreateTableRequest();
        // _kmsService->CreateTable(request);
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
    prefixEdit->setToolTip("Prefix to the table name");
    connect(prefixEdit, &QLineEdit::textChanged, this, [this,prefixEdit]() {
        _prefixClear->setDisabled(false);
        _proxyModel->setFilterColumn(0);
        _proxyModel->setFilterPrefix(prefixEdit->text());
    });
    _prefixClear = new QPushButton(IconUtils::GetIcon("clear"), "", this);
    _prefixClear->setDisabled(true);
    _prefixClear->setToolTip("Clear the table name prefix");
    connect(_prefixClear, &QPushButton::clicked, this, [this]() {
        _proxyModel->clearFilter();
        _prefixClear->setDisabled(true);
    });
    prefixLayout->addWidget(prefixEdit);
    prefixLayout->addWidget(_prefixClear);

    // Table
    const QStringList headers = QStringList() = {tr("ID"), tr("State"), tr("Usage"), tr("Spec"), tr("Created"), tr("Modified"), tr("KeyArn")};

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
    _tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    _tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    _tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    _tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    _tableView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    _tableView->setColumnHidden(6, true);
    _tableView->addAction(GetRefreshAction(this));

    // Connect double-click
    connect(_tableView, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {

        const QModelIndex sourceIndex = _proxyModel->mapToSource(index);

        // Extract Key ID
        const QString keyId = _dataModel->item(sourceIndex.row(), 0)->text();

        KMSKeyDialog dialog(keyId, this);
        dialog.exec();

    });

    // Add context menu
    _tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_tableView, &QTableWidget::customContextMenuRequested, this, &KMSKeyList::ShowContextMenu);

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
}

KMSKeyList::~KMSKeyList() {
    StopAutoUpdate();
}

void KMSKeyList::LoadContent() {
    _kmsService->ListKmsKeys(_prefixValue);
}

void KMSKeyList::HandleListKeysSignal(const KMSListKeysResponse &listKeysResponse) {
    const int selectedRow = _tableView->selectionModel()->currentIndex().row();
    _tableView->setSortingEnabled(false);
    _dataModel->removeRows(0, _dataModel->rowCount());

    for (auto r = 0, c = 0; r < listKeysResponse.keyCounters.count(); r++, c = 0) {
        SetColumn(_dataModel, r, c++, listKeysResponse.keyCounters.at(r).keyId);
        SetColumn(_dataModel, r, c++, KeyStateToString(listKeysResponse.keyCounters.at(r).keyState));
        SetColumn(_dataModel, r, c++, KeyUsageToString(listKeysResponse.keyCounters.at(r).keyUsage));
        SetColumn(_dataModel, r, c++, KeySpecToString(listKeysResponse.keyCounters.at(r).keySpec));
        SetColumn(_dataModel, r, c++, listKeysResponse.keyCounters.at(r).created);
        SetColumn(_dataModel, r, c++, listKeysResponse.keyCounters.at(r).modified);
        SetColumn(_dataModel, r, c++, listKeysResponse.keyCounters.at(r).arn);
    }
    // Reset selection
    _tableView->setSortingEnabled(true);
    _tableView->sortByColumn(_sortColumn, _sortOrder);
    _tableView->selectRow(selectedRow);
}

void KMSKeyList::ShowContextMenu(const QPoint &pos) const {

    // Cell index
    const QModelIndex proxyIndex = _tableView->indexAt(pos);
    if (!proxyIndex.isValid()) return;

    const QModelIndex sourceIndex = _proxyModel->mapToSource(proxyIndex);

    // Get container
    const QString keyId = _dataModel->item(sourceIndex.row(), 0)->text();
    const QString keyArn = _dataModel->item(sourceIndex.row(), 6)->text();

    QMenu menu;
    menu.setToolTipsVisible(true);
    QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit Key");
    editAction->setToolTip("Edit the key");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Key");
    deleteAction->setToolTip("Delete the key");

    if (const QAction *selectedAction = menu.exec(_tableView->viewport()->mapToGlobal(pos)); selectedAction == editAction) {
        KMSKeyDialog dialog(keyId);
        dialog.exec();
    } else if (selectedAction == deleteAction) {
        _kmsService->DeleteKey(keyId);
    }
}
