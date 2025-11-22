
#include <QHeaderView>
#include <modules/s3/S3ObjectList.h>

S3ObjectList::S3ObjectList(const QString &title, const QString &bucketName, QWidget *parent) : BasePage(parent), bucketName(bucketName) {
    // Connect service
    _s3Service = new S3Service();
    connect(_s3Service, &S3Service::ListObjectsSignal, this, &S3ObjectList::HandleListObjectSignal);
    connect(_s3Service, &S3Service::ReloadObjectsSignal, this, &S3ObjectList::HandleReloadObjectSignal);

    // Toolbar
    const auto toolBar = new QHBoxLayout();
    const auto spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar back action
    const auto backButton = new QPushButton(IconUtils::GetIcon("back"), "");
    backButton->setIconSize(QSize(16, 16));
    backButton->setToolTip("Add a new object");
    connect(backButton, &QPushButton::clicked, [this]() {
        StopAutoUpdate();
        emit BackToBucketList();
    });

    // Toolbar label
    const auto titleLabel = new QLabel(title);

    // Toolbar add action
    const auto addButton = new QPushButton(IconUtils::GetIcon("add"), "");
    addButton->setIconSize(QSize(16, 16));
    addButton->setToolTip("Add a new object");
    connect(addButton, &QPushButton::clicked, []() {
        bool ok;
        if (const QString text = QInputDialog::getText(nullptr, "Queue Name", "Queue name:", QLineEdit::Normal, "", &ok); ok && !text.isEmpty()) {
            // AddQueue(text);
        }
    });

    // Toolbar add action
    const auto purgeAllButton = new QPushButton(IconUtils::GetIcon("purge"), "");
    purgeAllButton->setIconSize(QSize(16, 16));
    purgeAllButton->setToolTip("Purge all objects");
    connect(purgeAllButton, &QPushButton::clicked, [this,bucketName]() {
        _s3Service->PurgeBucket(bucketName);
    });

    // Toolbar refresh action
    const auto refreshButton = new QPushButton(IconUtils::GetIcon("refresh"), "");
    refreshButton->setIconSize(QSize(16, 16));
    refreshButton->setToolTip("Refresh the queue list");
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
    connect(prefixEdit, &QLineEdit::textChanged, this, [this,prefixEdit]() {
        prefixValue = prefixEdit->text();
        prefixClear->setEnabled(true);
        LoadContent();
    });
    prefixLayout->addWidget(prefixEdit);
    prefixClear = new QPushButton(IconUtils::GetIcon("clear"), "", this);
    prefixClear->setDisabled(true);
    connect(prefixClear, &QPushButton::clicked, this, [this, prefixEdit]() {
        prefixEdit->clear();
        prefixValue = "";
        prefixClear->setEnabled(false);
    });
    prefixLayout->addWidget(prefixClear);

    // Table
    const QStringList headers = QStringList() = {tr("Key"), tr("ContentType"), tr("Size"), tr("Created"), tr("Modified"), tr("Oid")};

    tableWidget = new QTableWidget();
    tableWidget->setColumnCount(static_cast<int>(headers.count()));
    tableWidget->setShowGrid(true);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setHorizontalHeaderLabels(headers);
    tableWidget->setSortingEnabled(true);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    tableWidget->setColumnHidden(5, true);

    // Connect double-click
    connect(tableWidget, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {

        // Get the position
        const int row = index.row();

        const QString objectId = tableWidget->item(row, 0)->text();
        // TODO: add object details dialog
        //S3ObjectDetailsDialog dialog(objectId);
        //dialog.exec();
    });

    // Add context menu
    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableWidget, &QTableWidget::customContextMenuRequested, this, &S3ObjectList::ShowContextMenu);

    // Save sort column
    const QHeaderView *header = tableWidget->horizontalHeader();
    connect(header, &QHeaderView::sortIndicatorChanged, this, [this](const int column, const Qt::SortOrder order) {
        _sortColumn = column;
        _sortOrder = order;
    });

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addLayout(prefixLayout, 0);
    layout->addWidget(tableWidget, 2);
}

S3ObjectList::~S3ObjectList() {
    StopAutoUpdate();
}

void S3ObjectList::LoadContent() {
    if (Configuration::instance().GetConnectionState()) {
        _s3Service->ListObjects(bucketName, prefixValue);
    } else {
        QMessageBox::critical(nullptr, "Error", "Backend is not reachable");
    }
}

void S3ObjectList::HandleListObjectSignal(const S3ListObjectsResponse &listObjectResponse) {

    const int selectedRow = tableWidget->selectionModel()->currentIndex().row();
    tableWidget->setRowCount(0);
    tableWidget->setSortingEnabled(false);
    for (auto r = 0, c = 0; r < listObjectResponse.objectCounters.count(); r++, c = 0) {

        tableWidget->insertRow(r);
        SetColumn(tableWidget, r, c++, listObjectResponse.objectCounters.at(r).key);
        SetColumn(tableWidget, r, c++, listObjectResponse.objectCounters.at(r).contentType);
        SetColumn(tableWidget, r, c++, listObjectResponse.objectCounters.at(r).size);
        SetColumn(tableWidget, r, c++, listObjectResponse.objectCounters.at(r).created);
        SetColumn(tableWidget, r, c++, listObjectResponse.objectCounters.at(r).modified);
        SetHiddenColumn(tableWidget, r, c++, listObjectResponse.objectCounters.at(r).oid);
    }
    tableWidget->setRowCount(static_cast<int>(listObjectResponse.objectCounters.count()));
    tableWidget->setSortingEnabled(true);
    tableWidget->sortItems(_sortColumn, _sortOrder);
    tableWidget->selectRow(selectedRow);
    NotifyStatusBar();
}

void S3ObjectList::HandleReloadObjectSignal() {
    _s3Service->ListObjects(bucketName, prefixValue);
    NotifyStatusBar();
}

void S3ObjectList::ShowContextMenu(const QPoint &pos) const {

    const QModelIndex index = tableWidget->indexAt(pos);
    if (!index.isValid()) return;

    const int row = index.row();

    QMenu menu;
    //QAction *purgeAction = menu.addAction(QIcon(":/icons/purge.png"), "Purge Queue");
    //purgeAction->setToolTip("Purge the bucket");
    /*QAction *redriveAction = menu.addAction(QIcon(":/icons/redrive.png"), "Redrive Queue");
    redriveAction->setToolTip("Redrive all objects");*/
    QAction *touchAction = menu.addAction(IconUtils::GetIcon("touch"), "Touch Object");
    touchAction->setToolTip("Touch the object");
    menu.addSeparator();
    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Object");
    deleteAction->setToolTip("Delete the object");

    const QString key = tableWidget->item(row, 0)->text();
    if (const auto selectedAction = menu.exec(tableWidget->viewport()->mapToGlobal(pos)); selectedAction == deleteAction) {
        _s3Service->DeleteObject(bucketName, key);
    } else if (selectedAction == touchAction) {
        _s3Service->DeleteObject(bucketName, key);
    }
}
