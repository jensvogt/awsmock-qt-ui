
#include <modules/s3/S3ObjectList.h>

S3ObjectList::S3ObjectList(const QString &title, QWidget *parent) : BasePage(parent) {

    // Connect service
    _s3Service = new S3Service();
    connect(_s3Service, &S3Service::ListObjectsSignal, this, &S3ObjectList::HandleListObjectSignal);
    connect(_s3Service, &S3Service::ReloadObjectsSignal, this, &S3ObjectList::LoadContent);

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
        if (S3ObjectAddDialog dialog(_bucketDetailsResponse.bucketName, _bucketDetailsResponse.bucketArn, _bucketDetailsResponse.defaultMetadata); dialog.exec() == QDialog::Accepted) {
            new Awsmock::Components::ToastOverlay("Object uploaded! Bucket: " + _bucketDetailsResponse.bucketName + ", Key: " + dialog.GetS3ObjectKey(), this);
        }
    });

    // Toolbar purge action
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

    // Table
    const QStringList headers = QStringList() = {
                                    tr("Key"), tr("ContentType"), tr("Size"), tr("Created"), tr("Modified"), tr("Oid")
                                };

    // Table view
    _tableView = new PageableTable(this);
    _tableView->SetHeaderNames(headers);
    _tableView->SetResizeModes({QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents});
    _tableView->SetHiddenColumns({5});
    _tableView->SetSorting(3, "created", -1);
    _tableView->SetMultiRowSelection(true);
    _tableView->setServiceApis(_s3Service->getApis());

    // Connect double-click
    connect(_tableView, &PageableTable::DoubleClicked, this, [this](const QModelIndex &index) {

        // Get the position
        const auto objectId = _tableView->GetValue<QString>(index, 5);

        // Open details dialog
        S3ObjectEditDialog dialog(objectId, this);
        dialog.exec();
    });

    // Add context menu
    connect(_tableView, &PageableTable::ContextMenuRequested, this, &S3ObjectList::ShowContextMenu);

    // Connect paging changes
    connect(_tableView, &PageableTable::ReloadTable, this, &S3ObjectList::LoadContent);

    // Add details shortcut
    connect(_tableView, &PageableTable::ShowDetailsSignal, this, [this](const QModelIndex &index) {
        const auto objectId = _tableView->GetValue<QString>(index, 5);
        S3ObjectEditDialog dialog(objectId, this);
        dialog.exec();
    });

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(_tableView, 2);
}

S3ObjectList::~S3ObjectList() = default;

void S3ObjectList::HandleBucketDetailsSignal(const S3GetBucketDetailsResponse &bucketDetailsResponse) {
    this->_bucketDetailsResponse = bucketDetailsResponse;
}

void S3ObjectList::LoadContent() {
    _bucketName = GetArgument<QString>("bucketName");
    _titleLabel->setText("S3 Object List: " + _bucketName);
    _s3Service->GetBucketDetails(_bucketName);
    _s3Service->ListObjects(_bucketName, _tableView->GetPrefix(), _tableView->GetPageSize(), _tableView->GetPageIndex(), _tableView->GetSortAttribute(), _tableView->GetSortDirection());
}

void S3ObjectList::HandleListObjectSignal(const S3ListObjectsResponse &listObjectResponse) const {
    _tableView->SaveSelection();
    _tableView->setUpdatesEnabled(false);
    _tableView->ClearContent();
    _tableView->SetTotalSize(listObjectResponse.total);
    for (auto r = 0, c = 0; r < listObjectResponse.objectCounters.count(); r++, c = 0) {
        _tableView->SetColumn(r, c++, listObjectResponse.objectCounters.at(r).key);
        _tableView->SetColumn(r, c++, listObjectResponse.objectCounters.at(r).contentType);
        _tableView->SetColumn(r, c++, listObjectResponse.objectCounters.at(r).size);
        _tableView->SetColumn(r, c++, listObjectResponse.objectCounters.at(r).created);
        _tableView->SetColumn(r, c++, listObjectResponse.objectCounters.at(r).modified);
        _tableView->SetHiddenColumn(r, c++, listObjectResponse.objectCounters.at(r).oid);
    }
    _tableView->setUpdatesEnabled(true);
    _tableView->UpdateSorting();
    _tableView->RestoreSelection();
}

void S3ObjectList::HandleBulkDelete(const QModelIndexList &proxyIndices) const {

    // Convert to Persistent Source Indexes
    QList<QPersistentModelIndex> persistentRows;
    for (const QModelIndex &proxyIdx: proxyIndices) {
        persistentRows.append(_tableView->GetSourceIndex(proxyIdx));
    }

    // Now it is safe to delete in a loop
    for (const QPersistentModelIndex &srcIdx: persistentRows) {
        if (srcIdx.isValid()) {
            const auto key = _tableView->GetValue<QString>(srcIdx, 0);
            _s3Service->DeleteObject(_bucketName, key);
            _tableView->RemoveRow(srcIdx);
        }
    }
    logInfo << "Deleted objects, count: " << proxyIndices.count();
}

void S3ObjectList::HandleBulkTouch(const QModelIndexList &proxyIndices) const {
    for (const QModelIndex &proxyIdx: proxyIndices) {
        QModelIndex srcIdx = _tableView->GetSourceIndex(proxyIdx);
        const auto key = _tableView->GetValue<QString>(srcIdx, 0);
        _s3Service->TouchObject(_bucketName, key);
    }
    logInfo << "Touched objects, count: " << proxyIndices.count();
}

void S3ObjectList::ShowContextMenu(const QPoint &pos) {

    StopAutoUpdate();

    // Cell index
    const QModelIndex index = _tableView->GetIndexFromPosition(pos);

    QMenu *menu = new ContextMenu();

    QAction *editAction = menu->addAction(IconUtils::GetIcon("edit"), "Edit Object");
    editAction->setToolTip("Edit the S3 object");

    menu->addSeparator();

    QAction *touchAction = menu->addAction(IconUtils::GetIcon("touch"), "Touch Object");
    touchAction->setToolTip("Touch the object");

    menu->addSeparator();

    QAction *deleteAction = menu->addAction(IconUtils::GetIcon("delete"), "Delete Object");
    deleteAction->setToolTip("Delete the object");

    const auto key = _tableView->GetValue<QString>(index, 0);
    const auto objectId = _tableView->GetValue<QString>(index, 5);
    if (const auto selectedAction = menu->exec(_tableView->GetGlobalPosition(pos)); selectedAction == deleteAction) {
        const QModelIndexList selectedProxyIndices = _tableView->GetSelectedRows();
        HandleBulkDelete(selectedProxyIndices);
    } else if (selectedAction == touchAction) {
        const QModelIndexList selectedProxyIndices = _tableView->GetSelectedRows();
        HandleBulkTouch(selectedProxyIndices);
    } else if (selectedAction == editAction) {
        S3ObjectEditDialog dialog(objectId, this);
        dialog.exec();
    }
    StartAutoUpdate();
}
