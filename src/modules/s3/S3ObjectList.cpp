
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
    const auto titleLabel = new QLabel(title);

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
    toolBar->addWidget(titleLabel);
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

    _tableWidget = new QTableWidget();
    _tableWidget->setColumnCount(static_cast<int>(headers.count()));
    _tableWidget->setShowGrid(true);
    _tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    _tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    _tableWidget->setHorizontalHeaderLabels(headers);
    _tableWidget->setSortingEnabled(true);
    _tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    _tableWidget->horizontalHeaderItem(0)->setToolTip("S3 object key");
    _tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    _tableWidget->horizontalHeaderItem(1)->setToolTip("Mime type");
    _tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    _tableWidget->horizontalHeaderItem(2)->setToolTip("Object size in bytes");
    _tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    _tableWidget->horizontalHeaderItem(3)->setToolTip("Created timestamp");
    _tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    _tableWidget->horizontalHeaderItem(4)->setToolTip("Modified timestamp");
    _tableWidget->setColumnHidden(5, true);
    _tableWidget->addAction(GetRefreshAction(this));

    // Connect double-click
    connect(_tableWidget, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {
        // Get the position
        const int row = index.row();

        const QString objectId = _tableWidget->item(row, 5)->text();

        // Open details dialog
        S3ObjectEditDialog dialog(objectId, this);
        dialog.exec();
    });

    // Add context menu
    _tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_tableWidget, &QTableWidget::customContextMenuRequested, this, &S3ObjectList::ShowContextMenu);

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
}

S3ObjectList::~S3ObjectList() {
    StopAutoUpdate();
}

void S3ObjectList::HandleBucketDetailsSignal(const S3GetBucketDetailsResponse &bucketDetailsResponse) {
    this->_bucketDetailsResponse = bucketDetailsResponse;
}

void S3ObjectList::LoadContent() {
    _bucketName = GetArgument<QString>("bucketName");
    _s3Service->ListObjects(_bucketName, _prefixValue);
}

void S3ObjectList::HandleListObjectSignal(const S3ListObjectsResponse &listObjectResponse) const {
    const int selectedRow = _tableWidget->selectionModel()->currentIndex().row();
    _tableWidget->setRowCount(0);
    _tableWidget->setSortingEnabled(false);
    for (auto r = 0, c = 0; r < listObjectResponse.objectCounters.count(); r++, c = 0) {
        _tableWidget->insertRow(r);
        SetColumn(_tableWidget, r, c++, listObjectResponse.objectCounters.at(r).key);
        SetColumn(_tableWidget, r, c++, listObjectResponse.objectCounters.at(r).contentType);
        SetColumn(_tableWidget, r, c++, listObjectResponse.objectCounters.at(r).size);
        SetColumn(_tableWidget, r, c++, listObjectResponse.objectCounters.at(r).created);
        SetColumn(_tableWidget, r, c++, listObjectResponse.objectCounters.at(r).modified);
        SetHiddenColumn(_tableWidget, r, c++, listObjectResponse.objectCounters.at(r).oid);
    }
    _tableWidget->setRowCount(static_cast<int>(listObjectResponse.objectCounters.count()));
    _tableWidget->setSortingEnabled(true);
    _tableWidget->sortItems(_sortColumn, _sortOrder);
    _tableWidget->selectRow(selectedRow);
}

void S3ObjectList::HandleReloadObjectSignal() {
    _s3Service->ListObjects(_bucketName, _prefixValue);
    NotifyStatusBar();
}

void S3ObjectList::ShowContextMenu(const QPoint &pos) {
    const QModelIndex index = _tableWidget->indexAt(pos);
    if (!index.isValid()) return;

    const int row = index.row();

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

    const QString key = _tableWidget->item(row, 0)->text();
    const QString objectId = _tableWidget->item(row, 5)->text();
    if (const auto selectedAction = menu.exec(_tableWidget->viewport()->mapToGlobal(pos)); selectedAction == deleteAction) {
        _s3Service->DeleteObject(_bucketName, key);
    } else if (selectedAction == touchAction) {
        _s3Service->TouchObject(_bucketName, key);
    } else if (selectedAction == editAction) {
        S3ObjectEditDialog dialog(objectId, this);
        dialog.exec();
    }
}
