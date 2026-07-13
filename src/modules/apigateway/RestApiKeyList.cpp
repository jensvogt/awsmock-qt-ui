#include <modules/apigateway/RestApiKeyList.h>

RestApiKeyList::RestApiKeyList(const QString &title, QWidget *parent) : BasePage(parent) {

    _region = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");

    _apiGatewayService = new ApiGatewayService();
    connect(_apiGatewayService, &ApiGatewayService::ListApiKeysSignal, this, &RestApiKeyList::HandleListApiKeysSignal);
    connect(_apiGatewayService, &ApiGatewayService::ReloadApiKeysSignal, this, &RestApiKeyList::LoadContent);

    // Title label
    const auto titleLabel = new QLabel(title, this);

    // Toolbar
    const auto toolBar = new QHBoxLayout();
    toolBar->addWidget(titleLabel);
    const auto spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    const auto addButton = new QPushButton(IconUtils::GetIcon("add"), "", this);
    addButton->setToolTip("Add a new API key");
    connect(addButton, &QPushButton::clicked, this, [this]() {
        RestApiKeyDialog dialog(this);
        dialog.exec();
    });

    const auto refreshButton = new QPushButton(IconUtils::GetIcon("refresh"), "", this);
    refreshButton->setToolTip("Refresh the API key list (F5)");
    connect(refreshButton, &QPushButton::clicked, this, [this]() {
        LoadContent();
    });

    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);
    toolBar->addWidget(refreshButton);

    // Table
    const QStringList headers = QStringList() = {tr("ID"), tr("Name"), tr("Description"), tr("Enabled"), tr("Created"), tr("Modified")};

    _tableView = new PageableTable(this);
    _tableView->SetHeaderNames(headers);
    _tableView->SetResizeModes({
        QHeaderView::ResizeToContents, QHeaderView::Stretch, QHeaderView::Stretch,
        QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents
    });
    _tableView->SetSorting(1, "name", 1);
    _tableView->setServiceApis(_apiGatewayService->getApis());

    connect(_tableView, &PageableTable::ContextMenuRequested, this, &RestApiKeyList::ShowContextMenu);

    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(_tableView, 2);
}

RestApiKeyList::~RestApiKeyList() = default;

void RestApiKeyList::LoadContent() {
    _apiGatewayService->ListApiKeys(_tableView->GetPrefix(), _tableView->GetPageSize(), _tableView->GetPageIndex(), _tableView->GetSortAttribute(), _tableView->GetSortDirection());
}

void RestApiKeyList::HandleListApiKeysSignal(const RestApiKeyListResponse &response) const {
    _tableView->SaveSelection();
    _tableView->setUpdatesEnabled(false);
    _tableView->ClearContent();
    _tableView->SetTotalSize(response.total);
    for (auto r = 0, c = 0; r < response.apiKeys.count(); r++, c = 0) {
        _tableView->SetColumn(r, c++, response.apiKeys.at(r).id);
        _tableView->SetColumn(r, c++, response.apiKeys.at(r).name);
        _tableView->SetColumn(r, c++, response.apiKeys.at(r).description);
        _tableView->SetColumn(r, c++, response.apiKeys.at(r).enabled, IconUtils::GetIcon("enabled"), IconUtils::GetIcon("disabled"));
        _tableView->SetColumn(r, c++, response.apiKeys.at(r).created);
        _tableView->SetColumn(r, c++, response.apiKeys.at(r).modified);
    }
    _tableView->setUpdatesEnabled(true);
    _tableView->RestoreSelection();
    _tableView->UpdateSorting();
}

void RestApiKeyList::ShowContextMenu(const QPoint &pos) {
    const QModelIndex index = _tableView->GetIndexFromPosition(pos);
    const auto keyId = _tableView->GetValue<QString>(index, 0);
    const auto name = _tableView->GetValue<QString>(index, 1);
    const auto enabled = _tableView->GetValue<bool>(index, 3);

    QMenu *menu = new ContextMenu();

    QAction *enableAction = menu->addAction(IconUtils::GetIcon("enabled"), "Enable API Key");
    enableAction->setToolTip("Enable the API key.");
    enableAction->setEnabled(!enabled);

    QAction *disableAction = menu->addAction(IconUtils::GetIcon("disabled"), "Disable API Key");
    disableAction->setToolTip("Disable the API key.");
    disableAction->setEnabled(enabled);

    menu->addSeparator();

    QAction *deleteAction = menu->addAction(IconUtils::GetIcon("delete"), "Delete API Key");
    deleteAction->setToolTip("Delete the API key.");

    if (const QAction *selectedAction = menu->exec(_tableView->GetGlobalPosition(pos)); selectedAction == enableAction) {
        _apiGatewayService->EnableApiKey(keyId);
        new Awsmock::Components::ToastOverlay("API key enabled!\nName: " + name, this);
    } else if (selectedAction == disableAction) {
        _apiGatewayService->DisableApiKey(keyId);
        new Awsmock::Components::ToastOverlay("API key disabled!\nName: " + name, this);
    } else if (selectedAction == deleteAction) {
        _apiGatewayService->DeleteApiKey(keyId);
        new Awsmock::Components::ToastOverlay("API key deleted!\nName: " + name, this);
    }
}
