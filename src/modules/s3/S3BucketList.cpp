#include <ui_S3BucketEditDialog.h>
#include <modules/s3/S3BucketList.h>

S3BucketList::S3BucketList(const QString &title, QWidget *parent) : BasePage(parent) {
    // Set region
    _region = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");

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
    const auto addButton = new QPushButton(IconUtils::GetIcon("add"), "", this);
    addButton->setIconSize(QSize(16, 16));
    addButton->setToolTip("Add a new Bucket");
    connect(addButton, &QPushButton::clicked, [this]() {
        bool ok;
        if (const QString bucketName = QInputDialog::getText(nullptr, "Bucket Name", "Bucket name:", QLineEdit::Normal, "", &ok); ok && !bucketName.isEmpty()) {
            _s3Service->AddBucket(bucketName);
            new Awsmock::Components::ToastOverlay("Bucket created! Name: " + bucketName, _tableView);
        }
    });

    // Toolbar add action
    const auto purgeAllButton = new QPushButton(IconUtils::GetIcon("purge"), "", this);
    purgeAllButton->setIconSize(QSize(16, 16));
    purgeAllButton->setToolTip("Purge all Buckets");
    connect(purgeAllButton, &QPushButton::clicked, [this]() {
        _s3Service->PurgeAllBuckets();
        new Awsmock::Components::ToastOverlay("Purge all buckets initiated.\nChanges may take some time to propagate.");
    });

    // Toolbar reset counter action
    auto *resetCounterButton = new QPushButton(IconUtils::GetIcon("reset-counter"), "", this);
    resetCounterButton->setToolTip("Refresh the bucket object counters");
    connect(resetCounterButton, &QPushButton::clicked, [this]() {
        _s3Service->ResetObjectCounters();
        LoadContent();
    });

    // Toolbar refresh action
    const auto refreshButton = new QPushButton(IconUtils::GetIcon("refresh"), "", this);
    refreshButton->setIconSize(QSize(16, 16));
    refreshButton->setToolTip("Refresh the S3 bucket list");
    connect(refreshButton, &QPushButton::clicked, this, [this]() {
        LoadContent();
    });

    //    toolBar->addWidget(titleLabel);
    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);
    toolBar->addWidget(purgeAllButton);
    toolBar->addWidget(resetCounterButton);
    toolBar->addWidget(refreshButton);

    // Table
    const QStringList headers = QStringList() = {tr("Name"), tr("Keys"), tr("Size"), tr("Created"), tr("Modified"), tr("BucketArn")};
    _tableView = new PageableTable(this);
    _tableView->SetHeaderNames(headers);
    _tableView->SetResizeModes({QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents});
    _tableView->SetHiddenColumns({5});
    _tableView->SetSorting(1, "keys", -1);

    // Connect double-click
    connect(_tableView, &PageableTable::DoubleClicked, this, [this](const QModelIndex &index) {

        // Extract ARN and URL
        QMap<QString, QString> arguments;
        arguments["bucketName"] = _tableView->GetValue<QString>(index, 0);

        // Send notification
        emit EventBus::instance().RouteChanged("S3 Object List", arguments);
    });

    // Add context menu
    connect(_tableView, &PageableTable::ContextMenuRequested, this, &S3BucketList::ShowContextMenu);

    // Connect paging changes
    connect(_tableView, &PageableTable::ReloadTable, this, &S3BucketList::LoadContent);

    // Add details shortcut
    connect(_tableView, &PageableTable::ShowDetailsSignal, this, [this](const QModelIndex &index) {
        const auto bucketName = _tableView->GetValue<QString>(index, 0);
        S3BucketEditDialog dialog(bucketName);
        dialog.exec();
    });

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(_tableView, 2);
}

S3BucketList::~S3BucketList() = default;

void S3BucketList::LoadContent() {
    _s3Service->ListBuckets(_tableView->GetPrefix(), _tableView->GetPageSize(), _tableView->GetPageIndex(), _tableView->GetSortAttribute(), _tableView->GetSortDirection());
}

void S3BucketList::HandleListBucketSignal(const S3ListBucketResult &listBucketResult) const {
    _tableView->SaveSelection();
    _tableView->setUpdatesEnabled(false);
    _tableView->ClearContent();
    _tableView->SetTotalSize(listBucketResult.total);
    for (auto r = 0, c = 0; r < listBucketResult.bucketCounters.count(); r++, c = 0) {
        _tableView->SetColumn(r, c++, listBucketResult.bucketCounters.at(r).bucketName);
        _tableView->SetColumn(r, c++, listBucketResult.bucketCounters.at(r).objectCount);
        _tableView->SetColumn(r, c++, StringUtils::FormatSizeColumn(listBucketResult.bucketCounters.at(r).size, 1), Qt::AlignRight | Qt::AlignVCenter);
        _tableView->SetColumn(r, c++, listBucketResult.bucketCounters.at(r).created);
        _tableView->SetColumn(r, c++, listBucketResult.bucketCounters.at(r).modified);
        _tableView->SetHiddenColumn(r, c++, listBucketResult.bucketCounters.at(r).bucketArn);
    }
    _tableView->setUpdatesEnabled(true);
    _tableView->UpdateSorting();
    _tableView->RestoreSelection();
}

void S3BucketList::ShowContextMenu(const QPoint &pos) const {
    const QModelIndex index = _tableView->GetIndexFromPosition(pos);

    const long total = _tableView->GetValue<long>(index, 1);

    QMenu *menu = new ContextMenu();

    QAction *uploadAction = menu->addAction(IconUtils::GetIcon("upload"), "Upload Object");
    uploadAction->setToolTip("Upload an object");

    QAction *editAction = menu->addAction(IconUtils::GetIcon("edit"), "Edit Bucket");
    editAction->setToolTip("Edit the bucket details");

    menu->addSeparator();

    QAction *purgeAction = menu->addAction(IconUtils::GetIcon("purge"), "Purge Bucket");
    purgeAction->setToolTip("Purge the bucket");
    purgeAction->setEnabled(total > 0);

    menu->addSeparator();

    QAction *deleteAction = menu->addAction(IconUtils::GetIcon("delete"), "Delete Bucket");
    deleteAction->setToolTip("Delete the Bucket");

    const auto bucketName = _tableView->GetValue<QString>(index, 0);
    const auto bucketArn = _tableView->GetValue<QString>(index, 5);
    if (const QAction *selectedAction = menu->exec(_tableView->GetGlobalPosition(pos));
        selectedAction == purgeAction) {
        _s3Service->PurgeBucket(bucketName);
    } else if (selectedAction == deleteAction) {
        _s3Service->DeleteBucket(bucketName);
    } else if (selectedAction == uploadAction) {
        if (S3ObjectAddDialog dialog(bucketName, bucketArn); dialog.exec() == QDialog::Accepted) {
            new Awsmock::Components::ToastOverlay("Object uploaded! Bucket: " + bucketName + ", Key: " + dialog.GetS3ObjectKey(), _tableView);
        }
    } else if (selectedAction == editAction) {
        S3BucketEditDialog dialog(bucketName);
        dialog.exec();
    }
}
