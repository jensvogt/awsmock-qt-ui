
#include <modules/application/ApplicationList.h>

ApplicationList::ApplicationList(const QString &title, QWidget *parent) : BasePage(parent) {
    setAttribute(Qt::WA_DeleteOnClose);

    // Set region
    _region = Configuration::instance().GetRegion();

    // Connect service
    _applicationService = new ApplicationService();
    connect(_applicationService, &ApplicationService::LoadAllApplications, this, &ApplicationList::LoadContent);
    connect(_applicationService, &ApplicationService::ReloadApplicationsSignal, this, &ApplicationList::HandleListApplicationsSignal);

    // Title label
    const auto titleLabel = new QLabel(title, this);

    // Define toolbar
    const auto toolBar = new QHBoxLayout();
    toolBar->addWidget(titleLabel);
    const auto spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar add action
    const auto addButton = new QPushButton(IconUtils::GetIcon("dark", "add"), "", this);
    addButton->setIconSize(QSize(16, 16));
    addButton->setToolTip("Add a new application");
    connect(addButton, &QPushButton::clicked, []() {
        if (ApplicationAddDialog dialog; dialog.exec() == QDialog::Accepted) {
        }
    });

    // Toolbar add action
    const auto purgeAllButton = new QPushButton(IconUtils::GetIcon("dark", "restart"), "", this);
    purgeAllButton->setIconSize(QSize(16, 16));
    purgeAllButton->setToolTip("Restart all applications");
    connect(purgeAllButton, &QPushButton::clicked, [this]() {
        _applicationService->RestartAllApplications();
    });

    // Toolbar refresh action
    const auto refreshButton = new QPushButton(IconUtils::GetIcon("dark", "refresh"), "", this);
    refreshButton->setIconSize(QSize(16, 16));
    refreshButton->setToolTip("Refresh the application list");
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
                                << tr("Version")
                                << tr("Enabled")
                                << tr("Status")
                                << tr("Private Port")
                                << tr("Public Port")
                                << tr("Last Started")
                                << tr("Created")
                                << tr("Modified");

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
    tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents);

    // Connect double-click
    connect(tableWidget, &QTableView::doubleClicked, this, [=](const QModelIndex &index) {
        // Get the position
        const int row = index.row();

        // Extract ARN and URL
        const QString name = tableWidget->item(row, 0)->text();

        ApplicationEditDialog dialog(name);
        dialog.exec();
    });

    // Add context menu
    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableWidget, &QTableWidget::customContextMenuRequested, this, &ApplicationList::ShowContextMenu);

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

ApplicationList::~ApplicationList() {
    StopAutoUpdate();
}

void ApplicationList::LoadContent() {
    if (Configuration::instance().GetConnectionState()) {
        _applicationService->ListApplications(prefixValue);
    } else {
        QMessageBox::critical(nullptr, "Error", "Backend is not reachable");
    }
}

void ApplicationList::HandleListApplicationsSignal(const ApplicationListResponse &listApplicationResponse) {
    tableWidget->setRowCount(0);
    tableWidget->setSortingEnabled(false);
    for (auto r = 0; r < listApplicationResponse.applicationCounters.count(); r++) {
        tableWidget->insertRow(r);
        SetColumn(tableWidget, r, 0, listApplicationResponse.applicationCounters.at(r).name);
        SetColumn(tableWidget, r, 1, listApplicationResponse.applicationCounters.at(r).version);
        SetColumn(tableWidget, r, 2, listApplicationResponse.applicationCounters.at(r).enabled, IconUtils::GetIcon("dark", "enabled"), IconUtils::GetIcon("dark", "disabled"));
        SetColumn(tableWidget, r, 3, listApplicationResponse.applicationCounters.at(r).status == "RUNNING", IconUtils::GetIcon("dark", "running"), IconUtils::GetIcon("dark", "stopped"));
        //SetColumn(tableWidget, r, 3, listApplicationResponse.applicationCounters.at(r).status);
        SetColumn(tableWidget, r, 4, listApplicationResponse.applicationCounters.at(r).privatePort);
        SetColumn(tableWidget, r, 5, listApplicationResponse.applicationCounters.at(r).publicPort);
        SetColumn(tableWidget, r, 6, listApplicationResponse.applicationCounters.at(r).lastStarted);
        SetColumn(tableWidget, r, 7, listApplicationResponse.applicationCounters.at(r).created);
        SetColumn(tableWidget, r, 8, listApplicationResponse.applicationCounters.at(r).modified);
    }
    tableWidget->setRowCount(static_cast<int>(listApplicationResponse.applicationCounters.count()));
    tableWidget->setSortingEnabled(true);
    tableWidget->sortItems(_sortColumn, _sortOrder);
    NotifyStatusBar();
}

void ApplicationList::ShowContextMenu(const QPoint &pos) {
    StopAutoUpdate();

    // Cell index
    const QModelIndex index = tableWidget->indexAt(pos);
    if (!index.isValid()) return;

    const int row = index.row();

    QMenu menu;
    QAction *editAction = menu.addAction(IconUtils::GetIcon("dark", "edit"), "Edit Application");
    editAction->setToolTip("Edit the Topic details");

    menu.addSeparator();

    QAction *startAction = menu.addAction(IconUtils::GetIcon("dark", "start"), "Start Application");
    startAction->setToolTip("Start the application");

    QAction *stopAction = menu.addAction(IconUtils::GetIcon("dark", "stop"), "Stop Application");
    stopAction->setToolTip("Stop the application");

    QAction *restartAction = menu.addAction(IconUtils::GetIcon("dark", "restart"), "Restart Application");
    restartAction->setToolTip("Restart the application");

    menu.addSeparator();

    QAction *reloadAction = menu.addAction(IconUtils::GetIcon("dark", "reload"), "Reload Application");
    reloadAction->setToolTip("Reload the by creating a new container");

    QAction *uploadAction = menu.addAction(IconUtils::GetIcon("dark", "reload"), "Upload Application Code");
    uploadAction->setToolTip("Upload new application code");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("dark", "delete"), "Delete Application");
    deleteAction->setToolTip("Delete the Topic");

    const QString name = tableWidget->item(row, 0)->text();
    const QString version = tableWidget->item(row, 1)->text();
    if (const QAction *selectedAction = menu.exec(tableWidget->viewport()->mapToGlobal(pos)); selectedAction == editAction) {
        if (ApplicationEditDialog dialog(name); dialog.exec() == QDialog::Accepted) {
        }
    } else if (selectedAction == startAction) {
        _applicationService->StartApplication(name);
    } else if (selectedAction == stopAction) {
        _applicationService->StopApplication(name);
    } else if (selectedAction == restartAction) {
        _applicationService->RestartApplication(name);
    } else if (selectedAction == reloadAction) {
        //_applicationService->UploadApplicationCode(name, TODO, TODO);
    } else if (selectedAction == uploadAction) {
        ApplicationUploadCodeDialog dialog(name);
        dialog.exec();
        //        _applicationService->UploadApplicationCode(name, version, "");
    } else if (selectedAction == deleteAction) {
        _applicationService->DeleteApplication(name);
    } else if (selectedAction == editAction) {
        ApplicationEditDialog dialog(name);
        dialog.exec();
    }
    StartAutoUpdate();
}
