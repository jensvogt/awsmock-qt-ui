#include <QMessageBox>
#include <modules/secretsmanager/SecretList.h>

SecretList::SecretList(const QString &title, QWidget *parent) : BasePage(parent) {

    // Connect service
    _secretsManagerService = new SecretsManagerService();
    connect(_secretsManagerService, &SecretsManagerService::ReloadSecretsSignal, this, &SecretList::HandleListSecretsSignal);
    connect(_secretsManagerService, &SecretsManagerService::LoadAllSecrets, this, &SecretList::LoadContent);

    // Title label
    const auto titleLabel = new QLabel(title, this);

    // Define toolbar
    const auto toolBar = new QHBoxLayout();
    toolBar->addWidget(titleLabel);
    const auto spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar add action
    const auto addButton = new QPushButton(IconUtils::GetIcon("add"), nullptr, this);
    addButton->setToolTip("Add a new secret");
    connect(addButton, &QPushButton::clicked, []() {
        SecretAddDialog dialog;
        dialog.exec();
    });

    // Toolbar refresh action
    const auto refreshButton = new QPushButton(IconUtils::GetIcon("refresh"), nullptr, this);
    refreshButton->setToolTip("Refresh the secret list");
    connect(refreshButton, &QPushButton::clicked, this, [this]() {
        LoadContent();
    });

    //    toolBar->addWidget(titleLabel);
    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);
    toolBar->addWidget(refreshButton);

    // Table
    const QStringList headers = QStringList() = {
                                    tr("Name"), tr("Secret ID"), tr("Created"), tr("Modified"), tr("Arn")
                                };

    // Table
    _tableView = new PageableTable(this);
    _tableView->SetHeaderNames(headers);
    _tableView->SetResizeModes({QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents});
    _tableView->SetHiddenColumns({4});
    _tableView->SetSorting(1, "name", -1);
    _tableView->setServiceApis(_secretsManagerService->getApis());

    // Connect double-click
    connect(_tableView, &PageableTable::DoubleClicked, this, [this](const QModelIndex &index) {

        // Extract ARN
        const auto secretId = _tableView->GetValue<QString>(index, 1);

        SecretsDetailsDialog dialog(secretId, this);
        dialog.exec();
    });

    // Add context menu
    connect(_tableView, &PageableTable::ContextMenuRequested, this, &SecretList::ShowContextMenu);

    // Delete key
    connect(_tableView, &PageableTable::DeleteRequested, this, [this]() {
        DeleteSelected();
        LoadContent();
    });

    // Add details shortcut
    connect(_tableView, &PageableTable::ShowDetailsSignal, this, [this](const QModelIndex &index) {
        const auto secretId = _tableView->GetValue<QString>(index, 1);
        SecretsDetailsDialog dialog(secretId, this);
        dialog.exec();
    });

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(_tableView, 2);
}

SecretList::~SecretList() = default;

void SecretList::LoadContent() {
    _secretsManagerService->ListSecrets(_tableView->GetPrefix(), _tableView->GetPageSize(), _tableView->GetPageIndex(), _tableView->GetSortAttribute(), _tableView->GetSortDirection());
}

void SecretList::HandleListSecretsSignal(const SecretsListResponse &secretsListResponse) const {
    _tableView->Clear();
    _tableView->SetTotalSize(secretsListResponse.total);
    for (auto r = 0, c = 0; r < secretsListResponse.secretCounters.count(); r++, c = 0) {
        _tableView->SetColumn(r, c++, secretsListResponse.secretCounters.at(r).name);
        _tableView->SetColumn(r, c++, secretsListResponse.secretCounters.at(r).secretId);
        _tableView->SetColumn(r, c++, secretsListResponse.secretCounters.at(r).createdDate);
        _tableView->SetColumn(r, c++, secretsListResponse.secretCounters.at(r).lastChangedDate);
        _tableView->SetHiddenColumn(r, c++, secretsListResponse.secretCounters.at(r).arn);
    }
    _tableView->UpdateSorting();
}

void SecretList::ShowContextMenu(const QPoint &pos) {

    const QModelIndexList selectedRows = _tableView->GetSelectedRows();
    if (selectedRows.isEmpty()) return;

    const QModelIndex index = selectedRows.first();
    const bool multiSelect = selectedRows.count() > 1;

    // Get container
    const auto secretId = _tableView->GetValue<QString>(index, 1);

    QMenu menu;
    QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit Secret");
    editAction->setToolTip("Edit the secret.");
    editAction->setEnabled(!multiSelect);

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), multiSelect ? QString("Delete %1 Secrets").arg(selectedRows.count()) : "Delete Secret");
    deleteAction->setToolTip("Delete the selected secret(s)");

    if (const QAction *selectedAction = menu.exec(_tableView->GetGlobalPosition(pos)); selectedAction == editAction) {
        SecretsDetailsDialog dialog(secretId, this);
        dialog.exec();
    } else if (selectedAction == deleteAction) {
        DeleteSelected();
    }
    LoadContent();
    StartAutoUpdate();
}

void SecretList::DeleteSelected() {
    const QModelIndexList selectedRows = _tableView->GetSelectedRows();
    if (selectedRows.isEmpty()) return;

    const bool multiSelect = selectedRows.count() > 1;
    if (!multiSelect || QMessageBox::question(this, "Delete Secrets", QString("Delete %1 selected secrets?").arg(selectedRows.count())) == QMessageBox::Yes) {
        for (const QModelIndex &row: selectedRows) {
            _secretsManagerService->DeleteSecret(_tableView->GetValue<QString>(row, 1));
        }
    }
}
