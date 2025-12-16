#include <modules/secretsmanager/SecretList.h>

SecretList::SecretList(const QString &title, QWidget *parent) : BasePage(parent) {
    setAttribute(Qt::WA_DeleteOnClose);

    // Connect service
    _secretsManagerService = new SecretsManagerService();
    connect(_secretsManagerService, &SecretsManagerService::ReloadSecretsSignal, this, &SecretList::HandleListSecretsSignal);

    // Title label
    const auto titleLabel = new QLabel(title, this);

    // Define toolbar
    const auto toolBar = new QHBoxLayout();
    toolBar->addWidget(titleLabel);
    const auto spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar add action
    const auto addButton = new QPushButton(IconUtils::GetIcon("add"), nullptr, this);
    addButton->setIconSize(QSize(16, 16));
    addButton->setToolTip("Add a new lambda");
    connect(addButton, &QPushButton::clicked, []() {
        /*if (LambdaAddDialog dialog; dialog.exec() == QDialog::Accepted) {
        }*/
    });

    // Toolbar add action
    const auto purgeAllButton = new QPushButton(IconUtils::GetIcon("restart"), nullptr, this);
    purgeAllButton->setIconSize(QSize(16, 16));
    purgeAllButton->setToolTip("Restart all lambdas");
    connect(purgeAllButton, &QPushButton::clicked, [this]() {
        //        _lambdaService->RestartAllLambdas();
    });

    // Toolbar refresh action
    const auto refreshButton = new QPushButton(IconUtils::GetIcon("refresh"), nullptr, this);
    refreshButton->setIconSize(QSize(16, 16));
    refreshButton->setToolTip("Refresh the lambda list");
    connect(refreshButton, &QPushButton::clicked, this, [this]() {
        LoadContent();
    });

    //    toolBar->addWidget(titleLabel);
    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);
    toolBar->addWidget(purgeAllButton);
    toolBar->addWidget(refreshButton);

    // Prefix edit
    auto *prefixLayout = new QHBoxLayout();
    auto *prefixEdit = new QLineEdit(this);
    auto prefixClearButton = new QPushButton(this);
    prefixEdit->setPlaceholderText("Prefix");
    prefixEdit->setEnabled(true);
    connect(prefixEdit, &QLineEdit::textChanged, this, [this,prefixClearButton,prefixEdit]() {
        prefixClearButton->setDisabled(false);
        _proxyModel->setFilterColumn(0);
        _proxyModel->setFilterPrefix(prefixEdit->text());
    });
    prefixLayout->addWidget(prefixEdit);

    // Prefix clear button
    prefixClearButton->setDisabled(true);
    prefixClearButton->setText(nullptr);
    prefixClearButton->setIcon(IconUtils::GetIcon("clear"));
    prefixClearButton->setToolTip("Clear the prefix field");
    connect(prefixClearButton, &QPushButton::clicked, this, [this,prefixClearButton]() {
        _proxyModel->clearFilter();
        prefixClearButton->setDisabled(true);
    });
    prefixLayout->addWidget(prefixClearButton);

    // Table
    const QStringList headers = QStringList() = {
                                    tr("Name"), tr("Secret ID"), tr("Created"), tr("Modified"), tr("Arn")
                                };

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
    _tableView->setColumnHidden(4, true);

    // Connect double-click
    connect(_tableView, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {

        // Extract ARN
        auto arn = _dataModel->item(index.row(), 4)->text();

        //LambdaDetailsDialog dialog(arn);
        //dialog.exec();
    });

    // Add context menu
    _tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_tableView, &QTableWidget::customContextMenuRequested, this, &SecretList::ShowContextMenu);

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

    // List containers
    StartAutoUpdate();
}

SecretList::~SecretList() {
    StopAutoUpdate();
}

void SecretList::LoadContent() {
    if (Configuration::instance().GetConnectionState()) {
        _secretsManagerService->ListSecrets();
    } else {
        QMessageBox::critical(nullptr, "Error", "Backend is not reachable");
    }
}

void SecretList::HandleListSecretsSignal(const SecretsListResponse &listSecretsResponse) {
    const int selectedRow = _tableView->selectionModel()->currentIndex().row();
    _tableView->setSortingEnabled(false);
    for (auto r = 0; r < listSecretsResponse.secretCounters.count(); r++) {
        SetColumn(_dataModel, r, 0, listSecretsResponse.secretCounters.at(r).name);
        SetColumn(_dataModel, r, 1, listSecretsResponse.secretCounters.at(r).secretId);
        SetColumn(_dataModel, r, 2, listSecretsResponse.secretCounters.at(r).createdDate);
        SetColumn(_dataModel, r, 3, listSecretsResponse.secretCounters.at(r).lastChangedDate);
        SetColumn(_dataModel, r, 4, listSecretsResponse.secretCounters.at(r).arn);
    }
    //_tableView->setRowCount(static_cast<int>(listLambdaResponse.lambdaCounters.count()));
    _tableView->setSortingEnabled(true);
    //_tableView->sortItems(_sortColumn, _sortOrder);
    _tableView->selectRow(selectedRow);
    NotifyStatusBar();
}

void SecretList::ShowContextMenu(const QPoint &pos) {
    // Stop auto updater
    StopAutoUpdate();

    // Cell index
    const QModelIndex index = _tableView->indexAt(pos);
    if (!index.isValid()) {
        return;
    }

    const int row = index.row();

    const QString arn = _dataModel->item(row, 4)->text();

    QMenu menu;
    QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit Secret");
    editAction->setToolTip("Edit the secret.");

    // QAction *logsAction = menu.addAction(IconUtils::GetIcon("logs"), "Show the lambda logs");
    // logsAction->setToolTip("Show the lambda logs");
    // if (containerId.isEmpty()) {
    //     logsAction->setDisabled(true);
    // }
    //
    // menu.addSeparator();
    //
    // QAction *enableAction = menu.addAction(IconUtils::GetIcon("enabled"), "Enable Lambda");
    // enableAction->setToolTip("Enable the lambda.");
    //
    // QAction *disableAction = menu.addAction(IconUtils::GetIcon("disabled"), "Disable Lambda");
    // disableAction->setToolTip("Disable the lambda.");
    //
    // menu.addSeparator();
    //
    // QAction *startAction = menu.addAction(IconUtils::GetIcon("start"), "Start Lambda");
    // startAction->setToolTip("Start the lambda");
    //
    // QAction *stopAction = menu.addAction(IconUtils::GetIcon("stop"), "Stop Lambda");
    // stopAction->setToolTip("Stop the lambda");
    //
    // QAction *restartAction = menu.addAction(IconUtils::GetIcon("restart"), "Restart Lambda");
    // restartAction->setToolTip("Restart the lambda");
    //
    // menu.addSeparator();
    //
    // QAction *rebuildAction = menu.addAction(IconUtils::GetIcon("rebuild"), "Rebuild Lambda");
    // rebuildAction->setToolTip("Rebuild the lambda by creating a new image and container.");
    //
    // QAction *uploadAction = menu.addAction(IconUtils::GetIcon("upload"), "Upload Lambda Code");
    // uploadAction->setToolTip("Upload new lambda code");
    //
    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Secret");
    deleteAction->setToolTip("Delete the secret");


    if (const QAction *selectedAction = menu.exec(_tableView->viewport()->mapToGlobal(pos)); selectedAction == editAction) {
        //LambdaDetailsDialog dialog(arn);
        //dialog.exec();
    } else if (selectedAction == deleteAction) {
        //  _lambdaService->DeleteLambda(name);
    }
    LoadContent();
    StartAutoUpdate();
}
