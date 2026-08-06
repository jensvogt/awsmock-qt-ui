
//
// Created by vogje01 on 2/15/26.
//

#include <QMessageBox>
#include <modules/cognito/CognitoUserpoolList.h>
#include <ui_CognitoAddUserpoolDialog.h>

CognitoUserpoolList::CognitoUserpoolList(const QString &title, QWidget *parent) : BasePage(parent) {

    // Service
    _cognitoService = new CognitoService();
    connect(_cognitoService, &CognitoService::ListUserPoolsSignal, this, &CognitoUserpoolList::HandleUserpoolList);
    connect(_cognitoService, &CognitoService::RefreshContentSignal, this, &CognitoUserpoolList::LoadContent);

    // General layout
    _layout = new QVBoxLayout(this);
    setLayout(_layout);

    // Title label
    const auto titleLabel = new QLabel(title, this);

    // Define toolbar
    const auto toolBar = new QHBoxLayout();
    toolBar->addWidget(titleLabel);
    const auto spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar add action
    const auto addButton = new QPushButton(IconUtils::GetIcon("add"), "", this);
    addButton->setToolTip("Add a new table");
    connect(addButton, &QPushButton::clicked, [this]() {
        CognitoAddUserpoolDialog dialog;
        dialog.exec();

        // Get request
        CognitoAddUserpoolRequest request;
        request.userpool = dialog.GetUserpoolName();
        _cognitoService->CreateUserpool(request);
    });

    // Add toolbar
    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);

    // Setup table
    _tableView = new PageableTable(this);
    _tableView->SetHeaderNames({"Name", "ID", "Users", "Created", "Modified"});
    _tableView->SetResizeModes({QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents});
    _tableView->SetSortColumn(0, "name");
    _tableView->SetSortDirection(1);
    _tableView->setServiceApis(_cognitoService->getApis());

    // Add context menu
    connect(_tableView, &PageableTable::ContextMenuRequested, this, &CognitoUserpoolList::ShowContextMenu);

    // Delete key
    connect(_tableView, &PageableTable::DeleteRequested, this, &CognitoUserpoolList::DeleteSelected);

    // Add details shortcut
    connect(_tableView, &PageableTable::ShowDetailsSignal, this, [this](const QModelIndex &index) {
        const auto tableName = _tableView->GetValue<QString>(index, 0);
        CognitoAddUserpoolDialog dialog;
        dialog.exec();
    });

    // Add all to the layout
    _layout->setContentsMargins(0, 0, 0, 0);
    _layout->addLayout(toolBar, 0);
    _layout->addWidget(_tableView);
}

CognitoUserpoolList::~CognitoUserpoolList() = default;

void CognitoUserpoolList::LoadContent() {
    _cognitoService->ListUserpools("", _tableView->GetPageSize(), _tableView->GetPageIndex(), _tableView->GetSortAttribute(), _tableView->GetSortDirection());
}

void CognitoUserpoolList::HandleUserpoolList(const CognitoUserpoolListResponse &response) const {
    _tableView->Clear();
    _tableView->SetTotalSize(response.total);
    for (auto r = 0, c = 0; r < response.userpools.count(); r++, c = 0) {
        _tableView->SetColumn(r, c++, response.userpools.at(r).name);
        _tableView->SetColumn(r, c++, response.userpools.at(r).id);
        _tableView->SetColumn(r, c++, response.userpools.at(r).userCount);
        _tableView->SetColumn(r, c++, response.userpools.at(r).created);
        _tableView->SetColumn(r, c++, response.userpools.at(r).modified);
    }
}

void CognitoUserpoolList::ShowContextMenu(const QPoint &pos) const {

    const QModelIndexList selectedRows = _tableView->GetSelectedRows();
    if (selectedRows.isEmpty()) return;

    const QModelIndex index = selectedRows.first();
    const bool multiSelect = selectedRows.count() > 1;

    // Get container
    const auto userPoolId = _tableView->GetValue<QString>(index, 1);

    QMenu menu;
    menu.setToolTipsVisible(true);
    QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit User Pool");
    editAction->setToolTip("Edit the user pool details");
    editAction->setEnabled(!multiSelect);

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), multiSelect ? QString("Delete %1 User Pools").arg(selectedRows.count()) : "Delete User Pool");
    deleteAction->setToolTip("Delete the selected user pool(s)");

    if (const QAction *selectedAction = menu.exec(_tableView->GetGlobalPosition(pos)); selectedAction == editAction) {
        CognitoAddUserpoolDialog dialog;
        dialog.exec();
    } else if (selectedAction == deleteAction) {
        DeleteSelected();
    }
}

void CognitoUserpoolList::DeleteSelected() const {
    const QModelIndexList selectedRows = _tableView->GetSelectedRows();
    if (selectedRows.isEmpty()) return;

    const bool multiSelect = selectedRows.count() > 1;
    if (!multiSelect || QMessageBox::question(nullptr, "Delete User Pools", QString("Delete %1 selected user pools?").arg(selectedRows.count())) == QMessageBox::Yes) {
        for (const QModelIndex &row: selectedRows) {
            CognitoDeleteUserpoolRequest request;
            request.userPoolId = _tableView->GetValue<QString>(row, 1);
            _cognitoService->DeleteUserpool(request);
        }
    }
}
