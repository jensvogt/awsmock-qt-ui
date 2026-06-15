#include <modules/kms/KMSKeyList.h>

KMSKeyList::KMSKeyList(const QString &title, QWidget *parent) : BasePage(parent) {
    // Set region
    _region = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");

    // Connect service
    _kmsService = new KMSService();
    connect(_kmsService, &KMSService::ListKeysSignal, this, &KMSKeyList::HandleListKeysSignal);
    connect(_kmsService, &KMSService::ReloadKeysSignal, this, &KMSKeyList::LoadContent);

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

    // Table
    const QStringList headers = QStringList() = {tr("ID"), tr("State"), tr("Usage"), tr("Spec"), tr("Created"), tr("Modified"), tr("KeyArn")};

    // Table
    _tableView = new PageableTable(this);
    _tableView->SetHeaderNames(headers);
    _tableView->SetResizeModes({QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents});
    _tableView->SetHiddenColumns({5});
    _tableView->SetSorting(1, "id", -1);
    _tableView->setServiceApis(_kmsService->getApis());

    // Connect double-click
    connect(_tableView, &PageableTable::DoubleClicked, this, [this](const QModelIndex &index) {
        const auto keyId = _tableView->GetValue<QString>(index, 0);
        KMSKeyDialog dialog(keyId, this);
        dialog.exec();
    });

    // Add context menu
    connect(_tableView, &PageableTable::ContextMenuRequested, this, &KMSKeyList::ShowContextMenu);

    // Add details shortcut
    connect(_tableView, &PageableTable::ShowDetailsSignal, this, [this](const QModelIndex &index) {
        const auto keyId = _tableView->GetValue<QString>(index, 0);
        KMSKeyDialog dialog(keyId, this);
        dialog.exec();
    });

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(_tableView, 2);
}

KMSKeyList::~KMSKeyList() = default;

void KMSKeyList::LoadContent() {
    _kmsService->ListKmsKeys(_tableView->GetPrefix());
}

void KMSKeyList::HandleListKeysSignal(const KMSListKeysResponse &listKeysResponse) const {
    _tableView->Clear();
    _tableView->SetTotalSize(listKeysResponse.total);
    for (auto r = 0, c = 0; r < listKeysResponse.keyCounters.count(); r++, c = 0) {
        _tableView->SetColumn(r, c++, listKeysResponse.keyCounters.at(r).keyId);
        _tableView->SetColumn(r, c++, KeyStateToString(listKeysResponse.keyCounters.at(r).keyState));
        _tableView->SetColumn(r, c++, KeyUsageToString(listKeysResponse.keyCounters.at(r).keyUsage));
        _tableView->SetColumn(r, c++, KeySpecToString(listKeysResponse.keyCounters.at(r).keySpec));
        _tableView->SetColumn(r, c++, listKeysResponse.keyCounters.at(r).created);
        _tableView->SetColumn(r, c++, listKeysResponse.keyCounters.at(r).modified);
        _tableView->SetHiddenColumn(r, c++, listKeysResponse.keyCounters.at(r).arn);
    }
    _tableView->UpdateSorting();
}

void KMSKeyList::ShowContextMenu(const QPoint &pos) {

    const QModelIndex index = _tableView->GetIndexFromPosition(pos);

    // Get container
    const auto keyId = _tableView->GetValue<QString>(index, 0);
    const auto keyArn = _tableView->GetValue<QString>(index, 6);

    QMenu menu;
    menu.setToolTipsVisible(true);
    QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit Key");
    editAction->setToolTip("Edit the key");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Key");
    deleteAction->setToolTip("Delete the key");

    if (const QAction *selectedAction = menu.exec(_tableView->GetGlobalPosition(pos)); selectedAction == editAction) {
        KMSKeyDialog dialog(keyId, this);
        dialog.exec();
    } else if (selectedAction == deleteAction) {
        _kmsService->DeleteKey(keyId);
    }
}
