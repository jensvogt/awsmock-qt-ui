
#include <modules/apigateway/RestApiList.h>

RestApiList::RestApiList(const QString &title, QWidget *parent) : BasePage(parent) {

    // Set region
    _region = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");

    // Connect service
    _apiGatewayService = new ApiGatewayService();
    connect(_apiGatewayService, &ApiGatewayService::ListRestApisSignal, this, &RestApiList::HandleListApplicationsSignal);
    connect(_apiGatewayService, &ApiGatewayService::ReloadRestApisSignal, this, &RestApiList::LoadContent);

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
        RestApiDetailsDialog dialog;
        dialog.exec();
    });

    // Toolbar add action
    const auto restartAllButton = new QPushButton(IconUtils::GetIcon("restart"), "", this);
    restartAllButton->setToolTip("Restart all applications");
    connect(restartAllButton, &QPushButton::clicked, []() {
        //_apiGatewayService->RestartAllApplications();
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
                                    tr("Name"), tr("SourceKey"), tr("Enabled"), tr("Created"), tr("Modified"), tr("Region")
                                };

    _tableView = new PageableTable(this);
    _tableView->SetHeaderNames(headers);
    _tableView->SetResizeModes({
        QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents,
        QHeaderView::ResizeToContents, QHeaderView::ResizeToContents
    });
    _tableView->SetHiddenColumns({5});
    _tableView->SetSorting(0, "name", 1);
    _tableView->setServiceApis(_apiGatewayService->getApis());

    // Connect double-click
    connect(_tableView, &PageableTable::DoubleClicked, this, [this](const QModelIndex &index) {

        // Extract name
        const auto name = _tableView->GetValue<QString>(index, 0);

        RestApiDetailsDialog dialog(name, this);
        dialog.exec();
    });

    // Add context menu
    connect(_tableView, &PageableTable::ContextMenuRequested, this, &RestApiList::ShowContextMenu);

    // Add details shortcut
    connect(_tableView, &PageableTable::ShowDetailsSignal, this, [this](const QModelIndex &index) {
        const auto name = _tableView->GetValue<QString>(index, 0);
        RestApiDetailsDialog dialog(name, this);
        dialog.exec();
    });

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(_tableView, 2);
}

RestApiList::~RestApiList() = default;

void RestApiList::LoadContent() {
    _apiGatewayService->ListRestApis(_tableView->GetPrefix(), _tableView->GetPageSize(), _tableView->GetPageIndex(), _tableView->GetSortAttribute(), _tableView->GetSortDirection());
}

void RestApiList::HandleListApplicationsSignal(const RestApiListResponse &restApiListResponse) const {
    _tableView->SaveSelection();
    _tableView->setUpdatesEnabled(false);
    _tableView->ClearContent();
    _tableView->SetTotalSize(restApiListResponse.total);
    for (auto r = 0, c = 0; r < restApiListResponse.restApiCounters.count(); r++, c = 0) {
        _tableView->SetColumn(r, c++, restApiListResponse.restApiCounters.at(r).name);
        _tableView->SetColumn(r, c++, restApiListResponse.restApiCounters.at(r).apiKeySource);
        _tableView->SetColumn(r, c++, restApiListResponse.restApiCounters.at(r).enabled, IconUtils::GetIcon("enabled"), IconUtils::GetIcon("disabled"));
        _tableView->SetColumn(r, c++, restApiListResponse.restApiCounters.at(r).created);
        _tableView->SetColumn(r, c++, restApiListResponse.restApiCounters.at(r).modified);
    }
    _tableView->setUpdatesEnabled(true);
    _tableView->RestoreSelection();
    _tableView->UpdateSorting();
}

void RestApiList::ShowContextMenu(const QPoint &pos) {
    // Cell index
    const QModelIndex index = _tableView->GetIndexFromPosition(pos);

    const auto name = _tableView->GetValue<QString>(index, 0);
    const auto region = _tableView->GetValue<QString>(index, 5);
    const auto enabled = _tableView->GetValue<bool>(index, 2);

    QMenu *menu = new ContextMenu();

    QAction *editAction = menu->addAction(IconUtils::GetIcon("edit"), "Edit Rest API");
    editAction->setToolTip("Edit the Rest API details.");

    menu->addSeparator();

    QAction *enableAction = menu->addAction(IconUtils::GetIcon("enabled"), "Enable Rest API");
    enableAction->setToolTip("Enable the Rest API.");
    enableAction->setEnabled(!enabled);

    QAction *disableAction = menu->addAction(IconUtils::GetIcon("disabled"), "Disable Rest API");
    disableAction->setToolTip("Disable the Rest API.");
    disableAction->setEnabled(enabled);

    menu->addSeparator();

    QAction *deleteAction = menu->addAction(IconUtils::GetIcon("delete"), "Delete Rest API");
    deleteAction->setToolTip("Delete the Rest API");

    if (const QAction *selectedAction = menu->exec(_tableView->GetGlobalPosition(pos)); selectedAction == editAction) {
        if (RestApiDetailsDialog dialog(name, this); dialog.exec() == QFileDialog::Accept) {
            new Awsmock::Components::ToastOverlay("REST API updated!\nName: " + name, this);
        }
        // } else if (selectedAction == logsAction) {
        //     auto *dialog = new ApplicationLogsDialog(name, containerId, this);
        //     dialog->setModal(false);
        //     dialog->setAttribute(Qt::WA_DeleteOnClose);
        //     dialog->show();
        // } else if (selectedAction == startAction) {
        //     //        _apiGatewayService->StartApplication(name);
        //     LoadContent();
        //     new Awsmock::Components::ToastOverlay("Application started!\nName: " + name, this);
    } else if (selectedAction == enableAction) {
        //        _apiGatewayService->EnableApplication(name);
        new Awsmock::Components::ToastOverlay("Application enabled!\nName: " + name, this);
    } else if (selectedAction == disableAction) {
        //        _apiGatewayService->DisableApplication(name);
        new Awsmock::Components::ToastOverlay("Application disabled!\nName: " + name, this);
        // } else if (selectedAction == stopAction) {
        //     //        _apiGatewayService->StopApplication(name);
        //     LoadContent();
        //     new Awsmock::Components::ToastOverlay("Application stopped\nName: " + name, this);
        // } else if (selectedAction == restartAction) {
        //     //        _apiGatewayService->RestartApplication(name);
        //     LoadContent();
        //     new Awsmock::Components::ToastOverlay("Application restarted\nName: " + name, this);
        // } else if (selectedAction == rebuildAction) {
        //     //        _applicationService->RebuildApplication(name);
        //     LoadContent();
        //     new Awsmock::Components::ToastOverlay("Application rebuild\nName: " + name, this);
        // } else if (selectedAction == uploadAction) {
        //     if (ApplicationUploadCodeDialog dialog(name); dialog.exec() == QFileDialog::Accept) {
        //         LoadContent();
        //         new Awsmock::Components::ToastOverlay("Application uploaded!\nName: " + name, this);
        //     }
    } else if (selectedAction == deleteAction) {
        _apiGatewayService->DeleteRestApi(name);
        new Awsmock::Components::ToastOverlay("REST API deleted!\nName: " + name, this);
    }
}
