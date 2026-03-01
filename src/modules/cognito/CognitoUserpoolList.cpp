
//
// Created by vogje01 on 2/15/26.
//

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
    _table = new PageableTable(this);
    _table->SetHeaderNames({"Name", "ID", "Users", "Created", "Modified"});
    _table->SetResizeModes({QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents});
    _table->SetSortColumn(0, "name");
    _table->SetSortDirection(1);

    // Add context menu
    connect(_table, &PageableTable::ContextMenuRequested, this, &CognitoUserpoolList::ShowContextMenu);

    // Add all to the layout
    _layout->setContentsMargins(0, 0, 0, 0);
    _layout->addLayout(toolBar, 0);
    _layout->addWidget(_table);
}

CognitoUserpoolList::~CognitoUserpoolList() = default;

void CognitoUserpoolList::LoadContent() {
    _cognitoService->ListUserpools("", _table->GetPageSize(), _table->GetPageIndex(), _table->GetSortAttribute(), _table->GetSortDirection());
}

void CognitoUserpoolList::HandleUserpoolList(const CognitoUserpoolListResponse &response) const {
    _table->Clear();
    _table->SetTotalSize(response.total);
    for (auto r = 0, c = 0; r < response.userpools.count(); r++, c = 0) {
        _table->SetColumn(r, c++, response.userpools.at(r).name);
        _table->SetColumn(r, c++, response.userpools.at(r).id);
        _table->SetColumn(r, c++, response.userpools.at(r).userCount);
        _table->SetColumn(r, c++, response.userpools.at(r).created);
        _table->SetColumn(r, c++, response.userpools.at(r).modified);
    }
}

void CognitoUserpoolList::ShowContextMenu(const QPoint &pos) const {

    // Cell index
    const QModelIndex index = _table->GetIndexFromPosition(pos);

    // Get container
    const QString userPoolId = _table->GetValue<QString>(index, 1);

    QMenu menu;
    menu.setToolTipsVisible(true);
    QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit User Pool");
    editAction->setToolTip("Edit the user pool details");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete User Pool");
    deleteAction->setToolTip("Delete the user pool");

    if (const QAction *selectedAction = menu.exec(_table->GetGlobalPosition(pos)); selectedAction == editAction) {
        CognitoAddUserpoolDialog dialog;
        dialog.exec();
    } else if (selectedAction == deleteAction) {
        CognitoDeleteUserpoolRequest request;
        request.userPoolId = userPoolId;
        _cognitoService->DeleteUserpool(request);
    }
}
