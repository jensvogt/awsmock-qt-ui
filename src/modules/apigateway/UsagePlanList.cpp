#include <modules/apigateway/UsagePlanList.h>

UsagePlanList::UsagePlanList(const QString &title, QWidget *parent) : BasePage(parent) {

    _region = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");

    _apiGatewayService = new ApiGatewayService();
    connect(_apiGatewayService, &ApiGatewayService::ListUsagePlansSignal, this, &UsagePlanList::HandleListUsagePlansSignal);
    connect(_apiGatewayService, &ApiGatewayService::ReloadUsagePlansSignal, this, &UsagePlanList::LoadContent);

    // Title label
    const auto titleLabel = new QLabel(title, this);

    // Toolbar
    const auto toolBar = new QHBoxLayout();
    toolBar->addWidget(titleLabel);
    const auto spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    const auto addButton = new QPushButton(IconUtils::GetIcon("add"), "", this);
    addButton->setToolTip("Add a new usage plan");
    connect(addButton, &QPushButton::clicked, this, [this]() {
        UsagePlanDetailDialog dialog(this);
        dialog.exec();
    });

    const auto refreshButton = new QPushButton(IconUtils::GetIcon("refresh"), "", this);
    refreshButton->setToolTip("Refresh the usage plan list (F5)");
    connect(refreshButton, &QPushButton::clicked, this, [this]() {
        LoadContent();
    });

    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);
    toolBar->addWidget(refreshButton);

    // Table
    const QStringList headers = QStringList() = {
        tr("ID"), tr("Name"), tr("Description"),
        tr("Rate Limit"), tr("Burst Limit"),
        tr("Quota Limit"), tr("Quota Period"),
        tr("Created"), tr("Modified")
    };

    _tableView = new PageableTable(this);
    _tableView->SetHeaderNames(headers);
    _tableView->SetResizeModes({
        QHeaderView::ResizeToContents, QHeaderView::Stretch, QHeaderView::Stretch,
        QHeaderView::ResizeToContents, QHeaderView::ResizeToContents,
        QHeaderView::ResizeToContents, QHeaderView::ResizeToContents,
        QHeaderView::ResizeToContents, QHeaderView::ResizeToContents
    });
    _tableView->SetSorting(1, "name", 1);
    _tableView->setServiceApis(_apiGatewayService->getApis());

    connect(_tableView, &PageableTable::ContextMenuRequested, this, &UsagePlanList::ShowContextMenu);
    connect(_tableView, &PageableTable::DoubleClicked, this, [this](const QModelIndex &index) {
        const auto planId = _tableView->GetValue<QString>(index, 0);
        UsagePlanDetailDialog dialog(planId, this);
        dialog.exec();
    });

    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(_tableView, 2);
}

UsagePlanList::~UsagePlanList() = default;

void UsagePlanList::LoadContent() {
    _apiGatewayService->ListUsagePlans(_tableView->GetPrefix(), _tableView->GetPageSize(), _tableView->GetPageIndex(), _tableView->GetSortAttribute(), _tableView->GetSortDirection());
}

void UsagePlanList::HandleListUsagePlansSignal(const UsagePlanListResponse &response) const {
    _tableView->SaveSelection();
    _tableView->setUpdatesEnabled(false);
    _tableView->ClearContent();
    _tableView->SetTotalSize(response.total);
    for (auto r = 0, c = 0; r < response.usagePlans.count(); r++, c = 0) {
        _tableView->SetColumn(r, c++, response.usagePlans.at(r).id);
        _tableView->SetColumn(r, c++, response.usagePlans.at(r).name);
        _tableView->SetColumn(r, c++, response.usagePlans.at(r).description);
        _tableView->SetColumn(r, c++, response.usagePlans.at(r).throttleRateLimit);
        _tableView->SetColumn(r, c++, response.usagePlans.at(r).throttleBurstLimit);
        _tableView->SetColumn(r, c++, response.usagePlans.at(r).quotaLimit);
        _tableView->SetColumn(r, c++, response.usagePlans.at(r).quotaPeriod);
        _tableView->SetColumn(r, c++, response.usagePlans.at(r).created);
        _tableView->SetColumn(r, c++, response.usagePlans.at(r).modified);
    }
    _tableView->setUpdatesEnabled(true);
    _tableView->RestoreSelection();
    _tableView->UpdateSorting();
}

void UsagePlanList::ShowContextMenu(const QPoint &pos) {
    const QModelIndex index = _tableView->GetIndexFromPosition(pos);
    const auto planId = _tableView->GetValue<QString>(index, 0);
    const auto name = _tableView->GetValue<QString>(index, 1);

    QMenu *menu = new ContextMenu();

    QAction *editAction = menu->addAction(IconUtils::GetIcon("edit"), "Edit Usage Plan");
    editAction->setToolTip("Edit the usage plan.");

    menu->addSeparator();

    QAction *deleteAction = menu->addAction(IconUtils::GetIcon("delete"), "Delete Usage Plan");
    deleteAction->setToolTip("Delete the usage plan.");

    if (const QAction *selectedAction = menu->exec(_tableView->GetGlobalPosition(pos)); selectedAction == editAction) {
        UsagePlanDetailDialog dialog(planId, this);
        dialog.exec();
    } else if (selectedAction == deleteAction) {
        _apiGatewayService->DeleteUsagePlan(planId);
        new Awsmock::Components::ToastOverlay("Usage plan deleted!\nName: " + name, this);
    }
}
