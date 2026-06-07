
#include <modules/application/ApplicationList.h>

ApplicationList::ApplicationList(const QString &title, QWidget *parent) : BasePage(parent) {

    // Set region
    _region = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");

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
    const auto addButton = new QPushButton(IconUtils::GetIcon("add"), "", this);
    addButton->setToolTip("Add a new application");
    connect(addButton, &QPushButton::clicked, []() {
        ApplicationAddDialog dialog;
        dialog.exec();
    });

    // Toolbar add action
    const auto restartAllButton = new QPushButton(IconUtils::GetIcon("restart"), "", this);
    restartAllButton->setToolTip("Restart all applications");
    connect(restartAllButton, &QPushButton::clicked, [this]() {
        _applicationService->RestartAllApplications();
    });

    // Toolbar monitoring action
    auto *monitoringButton = new QPushButton(IconUtils::GetIcon("monitoring"), nullptr, this);
    monitoringButton->setToolTip("Show the application monitoring counters");
    connect(monitoringButton, &QPushButton::clicked, []() {
        // Send notification
        emit EventBus::instance().RouteChanged("Application Monitoring", {});
    });

    // Toolbar refresh action
    const auto refreshButton = new QPushButton(IconUtils::GetIcon("refresh"), "", this);
    refreshButton->setToolTip("Refresh the application list (F5)");
    connect(refreshButton, &QPushButton::clicked, this, [this]() {
        LoadContent();
    });

    //    toolBar->addWidget(titleLabel);
    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);
    toolBar->addWidget(restartAllButton);
    toolBar->addWidget(monitoringButton);
    toolBar->addWidget(refreshButton);

    // Table
    const QStringList headers = QStringList() = {
                                    tr("Name"), tr("Version"), tr("Enabled"), tr("Status"), tr("Private Port"),
                                    tr("Public Port"), tr("Last Started"), tr("Created"), tr("Modified"),
                                    tr("ContainerId")
                                };

    _tableView = new PageableTable(this);
    _tableView->SetHeaderNames(headers);
    _tableView->SetResizeModes({
        QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents,
        QHeaderView::ResizeToContents, QHeaderView::ResizeToContents
    });
    _tableView->SetHiddenColumns({9});
    _tableView->SetSorting(0, "name", 1);
    _tableView->setServiceApis(_applicationService->getApis());

    // Connect double-click
    connect(_tableView, &PageableTable::DoubleClicked, this, [this](const QModelIndex &index) {

        // Extract name
        const auto name = _tableView->GetValue<QString>(index, 0);

        ApplicationEditDialog dialog(name, this);
        dialog.exec();
    });

    // Add context menu
    connect(_tableView, &PageableTable::ContextMenuRequested, this, &ApplicationList::ShowContextMenu);

    // Add details shortcut
    connect(_tableView, &PageableTable::ShowDetailsSignal, this, [this](const QModelIndex &index) {
        const auto name = _tableView->GetValue<QString>(index, 0);
        ApplicationEditDialog dialog(name, this);
        dialog.exec();
    });

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(_tableView, 2);
}

ApplicationList::~ApplicationList() = default;

void ApplicationList::LoadContent() {
    _applicationService->ListApplications(_tableView->GetPrefix(), _tableView->GetPageSize(), _tableView->GetPageIndex(), _tableView->GetSortAttribute(), _tableView->GetSortDirection());
}

void ApplicationList::HandleListApplicationsSignal(const ApplicationListResponse &listApplicationResponse) const {
    _tableView->SaveSelection();
    _tableView->setUpdatesEnabled(false);
    _tableView->ClearContent();
    _tableView->SetTotalSize(listApplicationResponse.total);
    for (auto r = 0, c = 0; r < listApplicationResponse.applicationCounters.count(); r++, c = 0) {
        _tableView->SetColumn(r, c++, listApplicationResponse.applicationCounters.at(r).name);
        _tableView->SetColumn(r, c++, listApplicationResponse.applicationCounters.at(r).version);
        _tableView->SetColumn(r, c++, listApplicationResponse.applicationCounters.at(r).enabled, IconUtils::GetIcon("enabled"), IconUtils::GetIcon("disabled"));
        _tableView->SetColumn(r, c++, listApplicationResponse.applicationCounters.at(r).status == "RUNNING", IconUtils::GetIcon("running"), IconUtils::GetIcon("stopped"));
        _tableView->SetColumn(r, c++, listApplicationResponse.applicationCounters.at(r).privatePort);
        _tableView->SetColumn(r, c++, listApplicationResponse.applicationCounters.at(r).publicPort);
        _tableView->SetColumn(r, c++, listApplicationResponse.applicationCounters.at(r).lastStarted);
        _tableView->SetColumn(r, c++, listApplicationResponse.applicationCounters.at(r).created);
        _tableView->SetColumn(r, c++, listApplicationResponse.applicationCounters.at(r).modified);
        _tableView->SetColumn(r, c++, listApplicationResponse.applicationCounters.at(r).containerId);
    }
    _tableView->setUpdatesEnabled(true);
    _tableView->RestoreSelection();
    _tableView->UpdateSorting();
}

void ApplicationList::ShowContextMenu(const QPoint &pos) {
    // Cell index
    const QModelIndex index = _tableView->GetIndexFromPosition(pos);

    const auto name = _tableView->GetValue<QString>(index, 0);
    const auto enabled = _tableView->GetValue<bool>(index, 2);
    const auto containerId = _tableView->GetValue<QString>(index, 9);

    QMenu *menu = new ContextMenu();

    QAction *editAction = menu->addAction(IconUtils::GetIcon("edit"), "Edit Application");
    editAction->setToolTip("Edit the application details.");

    QAction *logsAction = menu->addAction(IconUtils::GetIcon("logs"), "Show the application logs");
    logsAction->setToolTip("Show the application logs");
    logsAction->setEnabled(enabled);

    menu->addSeparator();

    QAction *enableAction = menu->addAction(IconUtils::GetIcon("enabled"), "Enable Application");
    enableAction->setToolTip("Enable the application.");
    enableAction->setEnabled(!enabled);

    QAction *disableAction = menu->addAction(IconUtils::GetIcon("disabled"), "Disable Application");
    disableAction->setToolTip("Disable the application.");
    disableAction->setEnabled(enabled);

    menu->addSeparator();

    QAction *startAction = menu->addAction(IconUtils::GetIcon("start"), "Start Application");
    startAction->setToolTip("Start the application");

    QAction *stopAction = menu->addAction(IconUtils::GetIcon("stop"), "Stop Application");
    stopAction->setToolTip("Stop the application");

    QAction *restartAction = menu->addAction(IconUtils::GetIcon("restart"), "Restart Application");
    restartAction->setToolTip("Restart the application");

    menu->addSeparator();

    QAction *rebuildAction = menu->addAction(IconUtils::GetIcon("rebuild"), "Rebuild Application");
    rebuildAction->setToolTip("Rebuild the application by creating a new image and container.");

    QAction *uploadAction = menu->addAction(IconUtils::GetIcon("upload"), "Upload Application Code");
    uploadAction->setToolTip("Upload new application code");

    menu->addSeparator();

    QAction *deleteAction = menu->addAction(IconUtils::GetIcon("delete"), "Delete Application");
    deleteAction->setToolTip("Delete the application");

    if (const QAction *selectedAction = menu->exec(_tableView->GetGlobalPosition(pos)); selectedAction == editAction) {
        if (ApplicationEditDialog dialog(name, this); dialog.exec() == QFileDialog::Accept) {
            new Awsmock::Components::ToastOverlay("Application updated!\nName: " + name, this);
            LoadContent();
        }
    } else if (selectedAction == logsAction) {
        auto *dialog = new ApplicationLogsDialog(name, containerId, this);
        dialog->setModal(false);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->show();
    } else if (selectedAction == startAction) {
        _applicationService->StartApplication(name);
        LoadContent();
        new Awsmock::Components::ToastOverlay("Application started!\nName: " + name, this);
    } else if (selectedAction == enableAction) {
        _applicationService->EnableApplication(name);
        LoadContent();
        new Awsmock::Components::ToastOverlay("Application enabled!\nName: " + name, this);
    } else if (selectedAction == disableAction) {
        _applicationService->DisableApplication(name);
        LoadContent();
        new Awsmock::Components::ToastOverlay("Application disabled!\nName: " + name, this);
    } else if (selectedAction == stopAction) {
        _applicationService->StopApplication(name);
        LoadContent();
        new Awsmock::Components::ToastOverlay("Application stopped\nName: " + name, this);
    } else if (selectedAction == restartAction) {
        _applicationService->RestartApplication(name);
        LoadContent();
        new Awsmock::Components::ToastOverlay("Application restarted\nName: " + name, this);
    } else if (selectedAction == rebuildAction) {
        _applicationService->RebuildApplication(name);
        LoadContent();
        new Awsmock::Components::ToastOverlay("Application rebuild\nName: " + name, this);
    } else if (selectedAction == uploadAction) {
        if (ApplicationUploadCodeDialog dialog(name); dialog.exec() == QFileDialog::Accept) {
            LoadContent();
            new Awsmock::Components::ToastOverlay("Application uploaded!\nName: " + name, this);
        }
    } else if (selectedAction == deleteAction) {
        _applicationService->DeleteApplication(name);
        LoadContent();
        new Awsmock::Components::ToastOverlay("Application deleted!\nName: " + name, this);
    }
}
