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
        }
    });

    // Toolbar add action
    const auto purgeAllButton = new QPushButton(IconUtils::GetIcon("purge"), "", this);
    purgeAllButton->setIconSize(QSize(16, 16));
    purgeAllButton->setToolTip("Purge all Buckets");
    connect(purgeAllButton, &QPushButton::clicked, [this]() {
        //  _s3Service->PurgeAllBuckets();
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
    toolBar->addWidget(refreshButton);

    // Table
    const QStringList headers = QStringList() = {
                                    tr("Name"), tr("Keys"), tr("Size"), tr("Created"), tr("Modified"),
                                    tr("BucketArn")
                                };
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

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(_tableView, 2);
}

S3BucketList::~S3BucketList() {
    StopAutoUpdate();
}

void S3BucketList::LoadContent() {
    _s3Service->ListBuckets(_tableView->GetPrefix(), _tableView->GetPageSize(), _tableView->GetPageIndex(), _tableView->GetSortAttribute(), _tableView->GetSortDirection());
}

void S3BucketList::HandleListBucketSignal(const S3ListBucketResult &listBucketResult) const {
    _tableView->Clear();
    _tableView->SetTotalSize(listBucketResult.total);
    for (auto r = 0, c = 0; r < listBucketResult.bucketCounters.count(); r++, c = 0) {
        _tableView->SetColumn(r, c++, listBucketResult.bucketCounters.at(r).bucketName);
        _tableView->SetColumn(r, c++, listBucketResult.bucketCounters.at(r).objectCount);
        _tableView->SetColumn(r, c++, listBucketResult.bucketCounters.at(r).size);
        _tableView->SetColumn(r, c++, listBucketResult.bucketCounters.at(r).created);
        _tableView->SetColumn(r, c++, listBucketResult.bucketCounters.at(r).modified);
        _tableView->SetHiddenColumn(r, c++, listBucketResult.bucketCounters.at(r).bucketArn);
    }
    _tableView->UpdateSorting();
}

void S3BucketList::ShowContextMenu(const QPoint &pos) const {
    const QModelIndex index = _tableView->GetIndexFromPosition(pos);

    QMenu menu;
    menu.setToolTipsVisible(true);
    QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit Bucket");
    editAction->setToolTip("Edit the bucket details");

    menu.addSeparator();

    QAction *purgeAction = menu.addAction(IconUtils::GetIcon("purge"), "Purge Bucket");
    purgeAction->setToolTip("Purge the bucket");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Bucket");
    deleteAction->setToolTip("Delete the Bucket");

    const auto bucketName = _tableView->GetValue<QString>(index, 0);
    const auto bucketArn = _tableView->GetValue<QString>(index, 5);
    if (const QAction *selectedAction = menu.exec(_tableView->GetGlobalPosition(pos));
        selectedAction == purgeAction) {
        _s3Service->PurgeBucket(bucketName);
    } else if (selectedAction == deleteAction) {
        _s3Service->DeleteBucket(bucketName);
    } else if (selectedAction == editAction) {
        S3BucketEditDialog dialog(bucketName);
        dialog.exec();
    }
}
