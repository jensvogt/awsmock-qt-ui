
//
// Created by vogje01 on 2/15/26.
//

#include <modules/cognito/CognitoUserpoolList.h>

CognitoUserpoolList::CognitoUserpoolList(const QString &title, QWidget *parent) : BasePage(parent) {

    // Service
    _cognitoService = new CognitoService();
    connect(_cognitoService, &CognitoService::ListUserPoolsSignal, this, &CognitoUserpoolList::HandleUserpoolList);
    _layout = new QVBoxLayout(this);
    setLayout(_layout);

    // Setup table
    _table = new PageableTable(this);
    _table->SetHeaderNames({"ID", "Created", "Modified"});
    _table->SetResizeModes({QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents});
    _layout->addWidget(_table);

    // Test
    _table->SetTotalSize(10000);
}

CognitoUserpoolList::~CognitoUserpoolList() = default;

void CognitoUserpoolList::LoadContent() {
    _cognitoService->ListUserpools("", _table->GetPageSize(), _table->GetPageIndex(), _table->GetSortColumn(), _table->GetSortDirection());
}

void CognitoUserpoolList::HandleUserpoolList(const CognitoUserpoolListResponse &response) {

}
