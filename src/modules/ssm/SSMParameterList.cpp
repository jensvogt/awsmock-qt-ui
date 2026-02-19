
#include <modules/ssm/SSMParameterList.h>

SSMParameterList::SSMParameterList(const QString &title, QWidget *parent) : BasePage(parent) {

    // Connect service
    _ssmService = new SSMService();
    connect(_ssmService, &SSMService::ListParameterSignal, this, &SSMParameterList::HandleParameterListSignal);
    connect(_ssmService, &SSMService::ReloadParameterListSignal, this, &SSMParameterList::LoadContent);

    // Toolbar
    const auto toolBar = new QHBoxLayout();
    const auto spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar label
    const auto titleLabel = new QLabel(title);

    // Toolbar add action
    const auto addButton = new QPushButton(IconUtils::GetIcon("add"), "");
    addButton->setIconSize(QSize(16, 16));
    addButton->setToolTip("Add a new parameter");
    connect(addButton, &QPushButton::clicked, [this]() {
        SSMParameterAddDialog dialog(this);
        dialog.exec();
    });

    // Toolbar refresh action
    const auto refreshButton = new QPushButton(IconUtils::GetIcon("refresh"), "");
    refreshButton->setIconSize(QSize(16, 16));
    refreshButton->setToolTip("Refresh the SSM parameter list");
    connect(refreshButton, &QPushButton::clicked, [this]() {
        LoadContent();
    });

    //    toolBar->addWidget(backButton);
    toolBar->addWidget(titleLabel);
    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);
    toolBar->addWidget(refreshButton);

    // Table
    const QStringList headers = QStringList() = {
                                    tr("Name"), tr("Created"), tr("Modified"), tr("Arn")
                                };

    // Table
    _tableView = new PageableTable();
    _tableView->SetHeaderNames(headers);
    _tableView->SetResizeModes({QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents});
    _tableView->SetHiddenColumns({3});
    _tableView->SetSorting(0, "name", 1);

    // Connect double-click
    connect(_tableView, &PageableTable::DoubleClicked, this, [this](const QModelIndex &index) {

        // Get container
        const QString parameterName = _tableView->GetValue<QString>(index, 0);

        // Open details dialog
        SSMParameterEditDialog dialog(parameterName, this);
        dialog.exec();
    });

    // Add context menu
    connect(_tableView, &PageableTable::ContextMenuRequested, this, &SSMParameterList::ShowContextMenu);

    // Add context menu
    connect(_tableView, &PageableTable::ReloadTable, this, &SSMParameterList::LoadContent);

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(_tableView, 2);
}

SSMParameterList::~SSMParameterList() {
    StopAutoUpdate();
}

void SSMParameterList::LoadContent() {
    _tableView->Clear();
    _ssmService->ListParameters(_tableView->GetPrefix(), _tableView->GetPageSize(), _tableView->GetPageIndex(), _tableView->GetSortAttribute(), _tableView->GetSortDirection());
}

void SSMParameterList::HandleParameterListSignal(const SSMParameterListResponse &listParameterResponse) const {
    _tableView->SetTotalSize(listParameterResponse.total);
    for (auto r = 0, c = 0; r < listParameterResponse.parameterCounters.count(); r++, c = 0) {
        _tableView->SetColumn(r, c++, listParameterResponse.parameterCounters.at(r).name);
        _tableView->SetColumn(r, c++, listParameterResponse.parameterCounters.at(r).created);
        _tableView->SetColumn(r, c++, listParameterResponse.parameterCounters.at(r).modified);
        _tableView->SetColumn(r, c++, listParameterResponse.parameterCounters.at(r).arn);
    }
    _tableView->UpdateSorting();
}

void SSMParameterList::ShowContextMenu(const QPoint &pos) {

    const QModelIndex index = _tableView->GetIndexFromPosition(pos);

    QMenu menu;
    menu.setToolTipsVisible(true);
    QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit Parameter");
    editAction->setToolTip("Edit the parameter");
    menu.addSeparator();
    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Parameter");
    deleteAction->setToolTip("Delete the parameter");

    const QString parameterName = _tableView->GetValue<QString>(index, 0);
    if (const auto selectedAction = menu.exec(_tableView->GetGlobalPosition(pos)); selectedAction == deleteAction) {
        _ssmService->DeleteParameter(parameterName);
    } else if (selectedAction == editAction) {
        SSMParameterEditDialog dialog(parameterName, this);
        dialog.exec();
    }
}
