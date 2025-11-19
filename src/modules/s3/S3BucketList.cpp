#include <ui_S3BucketEditDialog.h>
#include <modules/s3/S3BucketList.h>

#include "modules/s3/S3BucketEditDialog.h"

S3BucketList::S3BucketList(const QString &title, QWidget *parent) : BasePage(parent) {

    // Set region
    _region = Configuration::instance().GetRegion();

    // Connect service
    _s3Service = new S3Service();
    connect(_s3Service, &S3Service::ListBucketSignal, this, &S3BucketList::HandleListBucketSignal);
    connect(_s3Service, &S3Service::ReloadBucketListSignal, this, &S3BucketList::LoadContent);

    // Title label
    const auto titleLabel = new QLabel(title, this);

    // Define toolbar
    const auto toolBar = new QHBoxLayout();
    toolBar->addWidget(titleLabel);
    const auto spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar add action
    const auto addButton = new QPushButton(IconUtils::GetIcon("dark", "add"), "", this);
    addButton->setIconSize(QSize(16, 16));
    addButton->setToolTip("Add a new Bucket");
    connect(addButton, &QPushButton::clicked, [this]() {
        bool ok;
        if (const QString bucketName = QInputDialog::getText(nullptr, "Bucket Name", "Bucket name:", QLineEdit::Normal, "", &ok); ok && !bucketName.isEmpty()) {
            _s3Service->AddBucket(bucketName);
        }
    });

    // Toolbar add action
    const auto purgeAllButton = new QPushButton(IconUtils::GetIcon("dark", "purge"), "", this);
    purgeAllButton->setIconSize(QSize(16, 16));
    purgeAllButton->setToolTip("Purge all Buckets");
    connect(purgeAllButton, &QPushButton::clicked, [this]() {
        //  _s3Service->PurgeAllBuckets();
    });

    // Toolbar refresh action
    const auto refreshButton = new QPushButton(IconUtils::GetIcon("dark", "refresh"), "", this);
    refreshButton->setIconSize(QSize(16, 16));
    refreshButton->setToolTip("Refresh the bucket list");
    connect(refreshButton, &QPushButton::clicked, this, [this]() {
        LoadContent();
    });

    //    toolBar->addWidget(titleLabel);
    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);
    toolBar->addWidget(purgeAllButton);
    toolBar->addWidget(refreshButton);

    // Prefix editor
    auto prefixEdit = new QLineEdit(this);
    prefixEdit->setPlaceholderText("Prefix");
    connect(prefixEdit, &QLineEdit::returnPressed, this, [this, prefixEdit]() {
        prefixValue = prefixEdit->text();
        LoadContent();
    });

    // Table
    const QStringList headers = QStringList() << tr("Name")
                                << tr("Keys")
                                << tr("Size")
                                << tr("Created")
                                << tr("Modified")
                                << tr("BucketArn");

    tableWidget = new QTableWidget(this);

    tableWidget->setColumnCount(static_cast<int>(headers.count()));
    tableWidget->setShowGrid(true);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setHorizontalHeaderLabels(headers);
    tableWidget->setSortingEnabled(true);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    tableWidget->setColumnHidden(5, true);

    // Connect double-click
    connect(tableWidget, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {

        // Get the position
        const int row = index.row();

        // Extract ARN and URL
        const QString bucketName = tableWidget->item(row, 0)->text();

        // Send notification
        emit ShowS3Objects(bucketName);
    });

    // Add context menu
    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableWidget, &QTableWidget::customContextMenuRequested, this, &S3BucketList::ShowContextMenu);

    // Save sort column
    const QHeaderView *header = tableWidget->horizontalHeader();
    connect(header, &QHeaderView::sortIndicatorChanged, this, [this](const int column, const Qt::SortOrder order) {
        _sortColumn = column;
        _sortOrder = order;
    });

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(prefixEdit, 0);
    layout->addWidget(tableWidget, 2);
}

S3BucketList::~S3BucketList() {
    StopAutoUpdate();
}

void S3BucketList::LoadContent() {
    if (Configuration::instance().GetConnectionState()) {
        _s3Service->ListBuckets(prefixValue);
    } else {
        QMessageBox::critical(nullptr, "Error", "Backend is not reachable");
    }
}

void S3BucketList::HandleListBucketSignal(const S3ListBucketResult &listBucketResult) {
    const int selectedRow = tableWidget->selectionModel()->currentIndex().row();
    tableWidget->setRowCount(0);
    tableWidget->setSortingEnabled(false);
    for (auto r = 0, c = 0; r < listBucketResult.bucketCounters.count(); r++, c = 0) {
        tableWidget->insertRow(r);
        SetColumn(tableWidget, r, c++, listBucketResult.bucketCounters.at(r).bucketName);
        SetColumn(tableWidget, r, c++, listBucketResult.bucketCounters.at(r).objectCount);
        SetColumn(tableWidget, r, c++, listBucketResult.bucketCounters.at(r).size);
        SetColumn(tableWidget, r, c++, listBucketResult.bucketCounters.at(r).created);
        SetColumn(tableWidget, r, c++, listBucketResult.bucketCounters.at(r).modified);
        SetHiddenColumn(tableWidget, r, c++, listBucketResult.bucketCounters.at(r).bucketArn);
    }
    tableWidget->setRowCount(static_cast<int>(listBucketResult.bucketCounters.count()));
    tableWidget->setSortingEnabled(true);
    tableWidget->sortItems(_sortColumn, _sortOrder);
    tableWidget->selectRow(selectedRow);
    NotifyStatusBar();
}

void S3BucketList::ShowContextMenu(const QPoint &pos) const {
    const QModelIndex index = tableWidget->indexAt(pos);
    if (!index.isValid()) return;

    const int row = index.row();

    QMenu menu;
    QAction *editAction = menu.addAction(IconUtils::GetIcon("dark", "edit"), "Edit Bucket");
    editAction->setToolTip("Edit the bucket details");

    menu.addSeparator();

    QAction *purgeAction = menu.addAction(IconUtils::GetIcon("dark", "purge"), "Purge Bucket");
    purgeAction->setToolTip("Purge the bucket");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("dark", "delete"), "Delete Bucket");
    deleteAction->setToolTip("Delete the Bucket");

    const QString bucketName = tableWidget->item(row, 0)->text();
    const QString bucketArn = tableWidget->item(row, 5)->text();
    if (const QAction *selectedAction = menu.exec(tableWidget->viewport()->mapToGlobal(pos)); selectedAction == purgeAction) {
        _s3Service->PurgeBucket(bucketArn);
    } else if (selectedAction == deleteAction) {
        _s3Service->DeleteBucket(bucketName);
    } else if (selectedAction == editAction) {
        S3BucketEditDialog dialog(bucketName);
        dialog.exec();
    }
}
