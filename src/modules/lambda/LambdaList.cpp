#include <ui_LambdaDetailsDialog.h>
#include <modules/lambda/LambdaList.h>

#include "modules/lambda/LambdaDetailsDialog.h"
#include "modules/lambda/LambdaLogsDialog.h"

//#include "modules/lambda/LambdaLogsDialog.h"

LambdaList::LambdaList(const QString &title, QWidget *parent) : BasePage(parent) {
    setAttribute(Qt::WA_DeleteOnClose);

    // Set region
    _region = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");

    // Connect service
    _lambdaService = new LambdaService();
    connect(_lambdaService, &LambdaService::LoadAllLambdas, this, &LambdaList::LoadContent);
    connect(_lambdaService, &LambdaService::ReloadLambdasSignal, this,
            &LambdaList::HandleListLambdasSignal);

    // Title label
    const auto titleLabel = new QLabel(title, this);

    // Define toolbar
    const auto toolBar = new QHBoxLayout();
    toolBar->addWidget(titleLabel);
    const auto spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar add action
    const auto addButton = new QPushButton(IconUtils::GetIcon("add"), "", this);
    addButton->setIconSize(QSize(16, 16));
    addButton->setToolTip("Add a new lambda");
    connect(addButton, &QPushButton::clicked, []() {
        /*if (LambdaAddDialog dialog; dialog.exec() == QDialog::Accepted) {
        }*/
    });

    // Toolbar add action
    const auto purgeAllButton = new QPushButton(IconUtils::GetIcon("restart"), "", this);
    purgeAllButton->setIconSize(QSize(16, 16));
    purgeAllButton->setToolTip("Restart all lambdas");
    connect(purgeAllButton, &QPushButton::clicked, [this]() {
        //        _lambdaService->RestartAllLambdas();
    });

    // Toolbar refresh action
    const auto refreshButton = new QPushButton(IconUtils::GetIcon("refresh"), "", this);
    refreshButton->setIconSize(QSize(16, 16));
    refreshButton->setToolTip("Refresh the lambda list");
    connect(refreshButton, &QPushButton::clicked, this, [this]() {
        LoadContent();
    });

    //    toolBar->addWidget(titleLabel);
    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);
    toolBar->addWidget(purgeAllButton);
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
                                    tr("Name"), tr("Version"), tr("Enabled"), tr("Status"), tr("Instances"),
                                    tr("Invocations"), tr("Avg. Execution Time"), tr("Created"), tr("Modified"),
                                    tr("ContainerId")
                                };

    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(static_cast<int>(headers.count()));
    tableWidget->setShowGrid(true);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setHorizontalHeaderLabels(headers);
    tableWidget->setSortingEnabled(true);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents);
    tableWidget->setColumnHidden(9, true);

    // Connect double-click
    connect(tableWidget, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {
        // Get the position
        const int row = index.row();

        // Extract ARN
        const QString arn = tableWidget->item(row, 9)->text();

        LambdaDetailsDialog dialog(arn);
        dialog.exec();
    });

    // Add context menu
    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableWidget, &QTableWidget::customContextMenuRequested, this, &LambdaList::ShowContextMenu);

    // Save sort column
    const QHeaderView *header = tableWidget->horizontalHeader();
    connect(header, &QHeaderView::sortIndicatorChanged, this, [this](const int column, const Qt::SortOrder order) {
        _sortColumn = column;
        _sortOrder = order;
    });

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addLayout(prefixLayout, 0);
    layout->addWidget(tableWidget, 2);
}

LambdaList::~LambdaList() {
    StopAutoUpdate();
}

void LambdaList::LoadContent() {
    if (Configuration::instance().GetConnectionState()) {
        _lambdaService->ListLambdas(prefixValue);
    } else {
        QMessageBox::critical(nullptr, "Error", "Backend is not reachable");
    }
}

void LambdaList::HandleListLambdasSignal(const LambdaListResponse &listLambdaResponse) {
    const int selectedRow = tableWidget->selectionModel()->currentIndex().row();
    tableWidget->setRowCount(0);
    tableWidget->setSortingEnabled(false);
    for (auto r = 0; r < listLambdaResponse.lambdaCounters.count(); r++) {
        tableWidget->insertRow(r);
        SetColumn(tableWidget, r, 0, listLambdaResponse.lambdaCounters.at(r).name);
        SetColumn(tableWidget, r, 1, listLambdaResponse.lambdaCounters.at(r).version);
        SetColumn(tableWidget, r, 2, listLambdaResponse.lambdaCounters.at(r).enabled, IconUtils::GetIcon("enabled"), IconUtils::GetIcon("disabled"));
        SetColumn(tableWidget, r, 3, listLambdaResponse.lambdaCounters.at(r).state == "Active", IconUtils::GetIcon("running"), IconUtils::GetIcon("stopped"));
        SetColumn(tableWidget, r, 4, listLambdaResponse.lambdaCounters.at(r).instances);
        SetColumn(tableWidget, r, 5, listLambdaResponse.lambdaCounters.at(r).invocations);
        SetColumn(tableWidget, r, 6, listLambdaResponse.lambdaCounters.at(r).averageRuntime);
        SetColumn(tableWidget, r, 7, listLambdaResponse.lambdaCounters.at(r).created);
        SetColumn(tableWidget, r, 8, listLambdaResponse.lambdaCounters.at(r).modified);
        SetColumn(tableWidget, r, 9, listLambdaResponse.lambdaCounters.at(r).arn);
    }
    tableWidget->setRowCount(static_cast<int>(listLambdaResponse.lambdaCounters.count()));
    tableWidget->setSortingEnabled(true);
    tableWidget->sortItems(_sortColumn, _sortOrder);
    tableWidget->selectRow(selectedRow);
    NotifyStatusBar();
}

void LambdaList::ShowContextMenu(const QPoint &pos) {
    StopAutoUpdate();

    // Cell index
    const QModelIndex index = tableWidget->indexAt(pos);
    if (!index.isValid()) return;

    const int row = index.row();

    const QString name = tableWidget->item(row, 0)->text();
    const QString containerId = tableWidget->item(row, 9)->text();

    QMenu menu;
    QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit Lambda");
    editAction->setToolTip("Edit the lambda details.");

    QAction *logsAction = menu.addAction(IconUtils::GetIcon("logs"), "Show the lambda logs");
    logsAction->setToolTip("Show the lambda logs");
    if (containerId.isEmpty()) {
        logsAction->setDisabled(true);
    }

    menu.addSeparator();

    QAction *enableAction = menu.addAction(IconUtils::GetIcon("enabled"), "Enable Lambda");
    enableAction->setToolTip("Enable the lambda.");

    QAction *disableAction = menu.addAction(IconUtils::GetIcon("disabled"), "Disable Lambda");
    disableAction->setToolTip("Disable the lambda.");

    menu.addSeparator();

    QAction *startAction = menu.addAction(IconUtils::GetIcon("start"), "Start Lambda");
    startAction->setToolTip("Start the lambda");

    QAction *stopAction = menu.addAction(IconUtils::GetIcon("stop"), "Stop Lambda");
    stopAction->setToolTip("Stop the lambda");

    QAction *restartAction = menu.addAction(IconUtils::GetIcon("restart"), "Restart Lambda");
    restartAction->setToolTip("Restart the lambda");

    menu.addSeparator();

    QAction *rebuildAction = menu.addAction(IconUtils::GetIcon("rebuild"), "Rebuild Lambda");
    rebuildAction->setToolTip("Rebuild the lambda by creating a new image and container.");

    QAction *uploadAction = menu.addAction(IconUtils::GetIcon("upload"), "Upload Lambda Code");
    uploadAction->setToolTip("Upload new lambda code");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Lambda");
    deleteAction->setToolTip("Delete the Topic");

    if (const QAction *selectedAction = menu.exec(tableWidget->viewport()->mapToGlobal(pos));
        selectedAction == editAction) {
        //LambdaEditDialog dialog(name);
        //dialog.exec();
    } else if (selectedAction == logsAction) {
        auto *dialog = new LambdaLogsDialog(name, containerId);
        dialog->setModal(false);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->show();
    } else if (selectedAction == startAction) {
        // _lambdaService->StartLambda(name);
    } else if (selectedAction == enableAction) {
        //_lambdaService->EnableLambda(name);
    } else if (selectedAction == disableAction) {
        //_lambdaService->DisableLambda(name);
    } else if (selectedAction == stopAction) {
        //_lambdaService->StopLambda(name);
    } else if (selectedAction == restartAction) {
        //_lambdaService->RestartLambda(name);
    } else if (selectedAction == rebuildAction) {
        //_lambdaService->RebuildLambda(name);
    } else if (selectedAction == uploadAction) {
        //LambdaUploadCodeDialog dialog(name);
        //dialog.exec();
    } else if (selectedAction == deleteAction) {
        //_lambdaService->DeleteLambda(name);
    } else if (selectedAction == editAction) {
        //LambdaEditDialog dialog(name);
        //dialog.exec();
    }
    LoadContent();
    StartAutoUpdate();
}
