
#include <modules/ssm/SSMParameterList.h>

SSMParameterList::SSMParameterList(const QString &title, QWidget *parent) : BasePage(parent) {

    // Connect service
    _ssmService = new SSMService();
    connect(_ssmService, &SSMService::ListParameterSignal, this, &SSMParameterList::HandleParameterListSignal);

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

    // Prefix editor
    auto *prefixLayout = new QHBoxLayout();
    auto *prefixEdit = new QLineEdit(this);
    prefixEdit->setPlaceholderText("Prefix");
    connect(prefixEdit, &QLineEdit::textChanged, this, [this,prefixEdit]() {
        prefixValue = prefixEdit->text();
        prefixClear->setEnabled(true);
        LoadContent();
    });
    prefixLayout->addWidget(prefixEdit);
    prefixClear = new QPushButton(IconUtils::GetIcon("clear"), "", this);
    prefixClear->setDisabled(true);
    connect(prefixClear, &QPushButton::clicked, this, [this, prefixEdit]() {
        prefixEdit->clear();
        prefixValue = "";
        prefixClear->setEnabled(false);
    });
    prefixLayout->addWidget(prefixClear);

    // Table
    const QStringList headers = QStringList() = {
                                    tr("Name"), tr("Created"), tr("Modified"), tr("Arn")
                                };

    // Table
    _tableView = new QTableView();
    _dataModel = new QStandardItemModel(this);
    _dataModel->setHorizontalHeaderLabels(headers);
    _dataModel->setColumnCount(static_cast<int>(headers.count()));

    // Proxy model for prefix filtering
    _proxyModel = new PrefixFilterProxyModel(this);
    _proxyModel->setSourceModel(_dataModel);
    _tableView->setModel(_proxyModel);

    _tableView->setShowGrid(true);
    _tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    _tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _tableView->setSortingEnabled(true);
    _tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    _tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    _tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    _tableView->setColumnHidden(3, true);
    _tableView->addAction(GetRefreshAction(this));

    // Connect double-click
    connect(_tableView, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {

        // Cell index
        if (!index.isValid()) return;

        const QModelIndex sourceIndex = _proxyModel->mapToSource(index);

        // Get container
        const QString parameterName = _dataModel->item(sourceIndex.row(), 0)->text();

        // Open details dialog
        SSMParameterEditDialog dialog(parameterName, this);
        dialog.exec();
    });

    // Add context menu
    _tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_tableView, &QTableView::customContextMenuRequested, this, &SSMParameterList::ShowContextMenu);

    // Save sort column
    const QHeaderView *header = _tableView->horizontalHeader();
    connect(header, &QHeaderView::sortIndicatorChanged, this, [this](const int column, const Qt::SortOrder order) {
        _sortColumn = column;
        _sortOrder = order;
    });

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addLayout(prefixLayout, 0);
    layout->addWidget(_tableView, 2);
}

SSMParameterList::~SSMParameterList() {
    StopAutoUpdate();
}

void SSMParameterList::LoadContent() {
    if (Configuration::instance().GetConnectionState()) {
        _ssmService->ListParameters(prefixValue, _sortColumn, _sortOrder);
    } else {
        QMessageBox::critical(nullptr, "Error", "Backend is not reachable");
    }
}

void SSMParameterList::HandleParameterListSignal(const SSMParameterListResponse &listParameterResponse) const {
    const int selectedRow = _tableView->selectionModel()->currentIndex().row();
    _tableView->setSortingEnabled(false);
    for (auto r = 0, c = 0; r < listParameterResponse.parameterCounters.count(); r++, c = 0) {
        SetColumn(_dataModel, r, c++, listParameterResponse.parameterCounters.at(r).name);
        SetColumn(_dataModel, r, c++, listParameterResponse.parameterCounters.at(r).created.toString("yyyy-MM-dd hh:mm:ss"));
        SetColumn(_dataModel, r, c++, listParameterResponse.parameterCounters.at(r).modified.toString("yyyy-MM-dd hh:mm:ss"));
        SetColumn(_dataModel, r, c++, listParameterResponse.parameterCounters.at(r).arn);
    }
    // Reset selection
    _tableView->setSortingEnabled(true);
    _tableView->sortByColumn(_sortColumn, _sortOrder);
    _tableView->selectRow(selectedRow);
}

void SSMParameterList::ShowContextMenu(const QPoint &pos) const {
    const QModelIndex index = _tableView->indexAt(pos);
    if (!index.isValid()) return;

    const int row = index.row();

    QMenu menu;
    //QAction *purgeAction = menu.addAction(QIcon(":/icons/purge.png"), "Purge Queue");
    //purgeAction->setToolTip("Purge the bucket");
    /*QAction *redriveAction = menu.addAction(QIcon(":/icons/redrive.png"), "Redrive Queue");
    redriveAction->setToolTip("Redrive all parameters");*/
    QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit Parameter");
    editAction->setToolTip("Edit the SSM parameter");
    QAction *touchAction = menu.addAction(IconUtils::GetIcon("touch"), "Touch Parameter");
    touchAction->setToolTip("Touch the parameter");
    menu.addSeparator();
    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Parameter");
    deleteAction->setToolTip("Delete the parameter");

    /*const QString key = tableView->item(row, 0)->text();
    const QString parameterId = tableView->item(row, 5)->text();
    if (const auto selectedAction = menu.exec(tableView->viewport()->mapToGlobal(pos));
        selectedAction == deleteAction) {
        _ssmService->DeleteParameter(bucketName, key);
    } else if (selectedAction == touchAction) {
        _ssmService->DeleteParameter(bucketName, key);
    } else if (selectedAction == editAction) {
        SSMParameterEditDialog dialog(parameterId);
        dialog.exec();
    }*/
}
