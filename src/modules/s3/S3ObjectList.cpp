
#include <modules/s3/S3ObjectList.h>

S3ObjectList::S3ObjectList(const QString &title, QWidget *parent) : BasePage(parent) {

    // Connect service
    _s3Service = new S3Service();
    connect(_s3Service, &S3Service::ListObjectsSignal, this, &S3ObjectList::HandleListObjectSignal);
    connect(_s3Service, &S3Service::ReloadObjectsSignal, this, &S3ObjectList::HandleReloadObjectSignal);

    // Get the bucket
    connect(_s3Service, &S3Service::GetBucketDetailsSignal, this, &S3ObjectList::HandleBucketDetailsSignal);

    // Toolbar
    const auto toolBar = new QHBoxLayout();
    const auto spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar back action
    const auto backButton = new QPushButton(IconUtils::GetIcon("back"), "");
    backButton->setToolTip("Go back to bucket list");
    connect(backButton, &QPushButton::clicked, []() {
        emit EventBus::instance().RouteChanged("S3");
    });

    // Toolbar label
    _titleLabel = new QLabel(title);

    // Toolbar add action
    const auto addButton = new QPushButton(IconUtils::GetIcon("add"), "");
    addButton->setIconSize(QSize(16, 16));
    addButton->setToolTip("Add a new object");
    connect(addButton, &QPushButton::clicked, [this]() {
        S3ObjectAddDialog dialog(_bucketDetailsResponse);
        dialog.exec();
    });

    // Toolbar add action
    const auto purgeAllButton = new QPushButton(IconUtils::GetIcon("purge"), "");
    purgeAllButton->setIconSize(QSize(16, 16));
    purgeAllButton->setToolTip("Purge all objects");
    connect(purgeAllButton, &QPushButton::clicked, [this]() {
        _s3Service->PurgeBucket(_bucketName);
    });

    // Toolbar refresh action
    const auto refreshButton = new QPushButton(IconUtils::GetIcon("refresh"), "");
    refreshButton->setIconSize(QSize(16, 16));
    refreshButton->setToolTip("Refresh the S3 object list");
    connect(refreshButton, &QPushButton::clicked, [this]() {
        LoadContent();
    });

    toolBar->addWidget(backButton);
    toolBar->addWidget(_titleLabel);
    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);
    toolBar->addWidget(purgeAllButton);
    toolBar->addWidget(refreshButton);

    // Prefix editor
    auto *prefixLayout = new QHBoxLayout();
    auto *prefixEdit = new QLineEdit(this);
    prefixEdit->setPlaceholderText("Prefix");
    prefixEdit->setToolTip("Prefix fot the object key");
    connect(prefixEdit, &QLineEdit::textChanged, this, [this,prefixEdit]() {
        _prefixValue = prefixEdit->text();
        _prefixClear->setEnabled(true);
        LoadContent();
    });
    prefixLayout->addWidget(prefixEdit);
    _prefixClear = new QPushButton(IconUtils::GetIcon("clear"), "", this);
    _prefixClear->setDisabled(true);
    _prefixClear->setToolTip("Clear the object key prefix");
    connect(_prefixClear, &QPushButton::clicked, this, [this, prefixEdit]() {
        prefixEdit->clear();
        _prefixValue = "";
        _prefixClear->setEnabled(false);
    });
    prefixLayout->addWidget(_prefixClear);

    // Table
    const QStringList headers = QStringList() = {
                                    tr("Key"), tr("ContentType"), tr("Size"), tr("Created"), tr("Modified"), tr("Oid")
                                };

    // Table data model
    _tableModel = new QStandardItemModel(this);
    _tableModel->setHorizontalHeaderLabels(headers);
    _tableModel->setColumnCount(static_cast<int>(headers.count()));

    // Proxy model for prefix filtering
    _proxyModel = new PrefixFilterProxyModel(this);
    _proxyModel->setSourceModel(_tableModel);

    // Table view
    _tableView = new QTableView(this);
    _tableView->setModel(_proxyModel);

    _tableView->setShowGrid(true);
    _tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    _tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _tableView->setSortingEnabled(true);
    _tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    _tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    _tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    _tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    _tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    _tableView->setColumnHidden(5, true);
    _tableView->addAction(GetRefreshAction(this));

    // Connect double-click
    connect(_tableView, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {

        // Get the position
        const QModelIndex sourceIndex = _proxyModel->mapToSource(index);
        const QString objectId = _tableModel->item(sourceIndex.row(), 5)->text();

        // Open details dialog
        S3ObjectEditDialog dialog(objectId, this);
        dialog.exec();
    });

    // Add context menu
    _tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_tableView, &QTableWidget::customContextMenuRequested, this, &S3ObjectList::ShowContextMenu);

    // Save sort column
    const QHeaderView *header = _tableView->horizontalHeader();
    connect(header, &QHeaderView::sortIndicatorChanged, this, [this](const int column, const Qt::SortOrder order) {
        _sortColumn = column;
        _sortOrder = order;
    });

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addLayout(prefixLayout, 0);
    layout->addWidget(_tableView, 2);
}

S3ObjectList::~S3ObjectList() {
    StopAutoUpdate();
}

void S3ObjectList::HandleBucketDetailsSignal(const S3GetBucketDetailsResponse &bucketDetailsResponse) {
    this->_bucketDetailsResponse = bucketDetailsResponse;
}

void S3ObjectList::LoadContent() {
    _bucketName = GetArgument<QString>("bucketName");
    _titleLabel->setText("S3 Object List: " + _bucketName);
    _s3Service->ListObjects(_bucketName, _prefixValue);
}

void S3ObjectList::HandleListObjectSignal(const S3ListObjectsResponse &listObjectResponse) const {
    const int selectedRow = _tableView->selectionModel()->currentIndex().row();
    _tableView->setSortingEnabled(false);
    _tableModel->removeRows(0, _tableModel->rowCount());
    for (auto r = 0, c = 0; r < listObjectResponse.objectCounters.count(); r++, c = 0) {
        SetColumn(_tableModel, r, c++, listObjectResponse.objectCounters.at(r).key);
        SetColumn(_tableModel, r, c++, listObjectResponse.objectCounters.at(r).contentType);
        SetColumn(_tableModel, r, c++, listObjectResponse.objectCounters.at(r).size);
        SetColumn(_tableModel, r, c++, listObjectResponse.objectCounters.at(r).created);
        SetColumn(_tableModel, r, c++, listObjectResponse.objectCounters.at(r).modified);
        SetHiddenColumn(_tableModel, r, c++, listObjectResponse.objectCounters.at(r).oid);
    }
    _tableView->setSortingEnabled(true);
    _tableView->selectRow(selectedRow);
}

void S3ObjectList::HandleReloadObjectSignal() {
    _s3Service->ListObjects(_bucketName, _prefixValue);
    NotifyStatusBar();
}

void S3ObjectList::HandleBulkDelete(QModelIndexList proxyIndices) const {
    // Convert to Persistent Source Indexes
    QList<QPersistentModelIndex> persistentRows;
    for (const QModelIndex &proxyIdx: proxyIndices) {
        persistentRows.append(_proxyModel->mapToSource(proxyIdx));
    }

    // Now it is safe to delete in a loop
    for (const QPersistentModelIndex &srcIdx: persistentRows) {
        if (srcIdx.isValid()) {
            const QString key = _tableModel->item(srcIdx.row(), 0)->text();
            _s3Service->DeleteObject(_bucketName, key);
            _tableModel->removeRow(srcIdx.row(), srcIdx.parent());
        }
    }
}

void S3ObjectList::HandleBulkTouch(QModelIndexList proxyIndices) const {
    for (const QModelIndex &proxyIdx: proxyIndices) {
        QModelIndex srcIdx = _proxyModel->mapToSource(proxyIdx);
        const QString key = _tableModel->item(srcIdx.row(), 0)->text();
        _s3Service->TouchObject(_bucketName, key);
    }
}

void S3ObjectList::ShowContextMenu(const QPoint &pos) {

    const QModelIndexList selectedProxyIndices = _tableView->selectionModel()->selectedRows();

    // Cell index
    const QModelIndex proxyIndex = _tableView->indexAt(pos);
    if (!proxyIndex.isValid()) return;

    const QModelIndex sourceIndex = _proxyModel->mapToSource(proxyIndex);

    QMenu menu;
    menu.setToolTipsVisible(true);
    QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit Object");
    editAction->setToolTip("Edit the S3 object");

    menu.addSeparator();

    QAction *touchAction = menu.addAction(IconUtils::GetIcon("touch"), "Touch Object");
    touchAction->setToolTip("Touch the object");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Object");
    deleteAction->setToolTip("Delete the object");

    //_s3Service->DeleteObject(_bucketName, key);
    const QString key = _tableModel->item(sourceIndex.row(), 0)->text();
    const QString objectId = _tableModel->item(sourceIndex.row(), 5)->text();
    if (const auto selectedAction = menu.exec(_tableView->viewport()->mapToGlobal(pos)); selectedAction == deleteAction) {
        HandleBulkDelete(selectedProxyIndices);
    } else if (selectedAction == touchAction) {
        HandleBulkTouch(selectedProxyIndices);
    } else if (selectedAction == editAction) {
        S3ObjectEditDialog dialog(objectId, this);
        dialog.exec();
    }
}
