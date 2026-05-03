#include <modules/lambda/LambdaList.h>

#include "components/Toast.h"

LambdaList::LambdaList(const QString &title, QWidget *parent) : BasePage(parent) {

    // Set region
    _region = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");

    // Connect service
    _lambdaService = new LambdaService();
    connect(_lambdaService, &LambdaService::ReloadLambdas, this, &LambdaList::LoadContent);
    connect(_lambdaService, &LambdaService::ReloadLambdasSignal, this, &LambdaList::HandleListLambdasSignal);

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

    // Table
    const QStringList headers = QStringList() = {
                                    tr("Name"), tr("Version"), tr("Enabled"), tr("Status"), tr("Instances"),
                                    tr("Invocations"), tr("Avg. Execution Time"), tr("Created"), tr("Modified"),
                                    tr("ContainerId"), tr("ARN")
                                };

    _tableView = new PageableTable(this);
    _tableView->SetHeaderNames(headers);
    _tableView->SetResizeModes({
        QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents,
        QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents
    });
    _tableView->SetHiddenColumns({9, 10});
    _tableView->SetSorting(0, "function", 1);

    // Connect double-click
    connect(_tableView, &PageableTable::DoubleClicked, this, [this](const QModelIndex &index) {

        // Extract ARN
        const auto arn = _tableView->GetValue<QString>(index, 10);

        LambdaDetailsDialog dialog(arn);
        dialog.exec();
    });

    // Add context menu
    connect(_tableView, &PageableTable::ContextMenuRequested, this, &LambdaList::ShowContextMenu);

    // Add details shortcut
    connect(_tableView, &PageableTable::ShowDetailsSignal, this, [this](const QModelIndex &index) {
        const auto arn = _tableView->GetValue<QString>(index, 10);
        LambdaDetailsDialog dialog(arn);
        dialog.exec();
    });

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(_tableView, 2);
}

LambdaList::~LambdaList() = default;

void LambdaList::LoadContent() {
    _lambdaService->ListLambdas(_tableView->GetPrefix(), _tableView->GetPageSize(), _tableView->GetPageIndex(), _tableView->GetSortAttribute(), _tableView->GetSortDirection());
}

void LambdaList::HandleListLambdasSignal(const LambdaListResponse &listLambdaResponse) const {
    _tableView->SaveSelection();
    _tableView->setUpdatesEnabled(false);
    _tableView->ClearContent();
    _tableView->SetTotalSize(listLambdaResponse.total);
    for (auto r = 0, c = 0; r < listLambdaResponse.lambdaCounters.count(); r++, c = 0) {
        _tableView->SetColumn(r, c++, listLambdaResponse.lambdaCounters.at(r).name);
        _tableView->SetColumn(r, c++, listLambdaResponse.lambdaCounters.at(r).version);
        _tableView->SetColumn(r, c++, listLambdaResponse.lambdaCounters.at(r).enabled, IconUtils::GetIcon("enabled"), IconUtils::GetIcon("disabled"));
        _tableView->SetColumn(r, c++, listLambdaResponse.lambdaCounters.at(r).state == "Active", IconUtils::GetIcon("running"), IconUtils::GetIcon("stopped"));
        _tableView->SetColumn(r, c++, listLambdaResponse.lambdaCounters.at(r).instances);
        _tableView->SetColumn(r, c++, listLambdaResponse.lambdaCounters.at(r).invocations);
        _tableView->SetColumn(r, c++, listLambdaResponse.lambdaCounters.at(r).averageRuntime);
        _tableView->SetColumn(r, c++, listLambdaResponse.lambdaCounters.at(r).created);
        _tableView->SetColumn(r, c++, listLambdaResponse.lambdaCounters.at(r).modified);
        _tableView->SetHiddenColumn(r, c++, listLambdaResponse.lambdaCounters.at(r).containerId);
        _tableView->SetHiddenColumn(r, c++, listLambdaResponse.lambdaCounters.at(r).arn);
    }
    _tableView->setUpdatesEnabled(true);
    _tableView->UpdateSorting();
    _tableView->RestoreSelection();
}

void LambdaList::ShowContextMenu(const QPoint &pos) {

    // Cell index
    const QModelIndex index = _tableView->GetIndexFromPosition(pos);

    const auto arn = _tableView->GetValue<QString>(index, 10);
    const auto name = _tableView->GetValue<QString>(index, 0);
    const auto version = _tableView->GetValue<QString>(index, 1);
    const auto containerId = _tableView->GetValue<QString>(index, 9);

    QMenu *menu = new ContextMenu();

    QAction *editAction = menu->addAction(IconUtils::GetIcon("edit"), "Edit Lambda");
    editAction->setToolTip("Edit the lambda details.");

    QAction *logsAction = menu->addAction(IconUtils::GetIcon("logs"), "Show the lambda logs");
    logsAction->setToolTip("Show the lambda logs");
    if (arn.isEmpty()) {
        logsAction->setDisabled(true);
    }

    menu->addSeparator();

    QAction *enableAction = menu->addAction(IconUtils::GetIcon("enabled"), "Enable Lambda");
    enableAction->setToolTip("Enable the lambda.");

    QAction *disableAction = menu->addAction(IconUtils::GetIcon("disabled"), "Disable Lambda");
    disableAction->setToolTip("Disable the lambda.");

    menu->addSeparator();

    QAction *startAction = menu->addAction(IconUtils::GetIcon("start"), "Start Lambda");
    startAction->setToolTip("Start the lambda");

    QAction *stopAction = menu->addAction(IconUtils::GetIcon("stop"), "Stop Lambda");
    stopAction->setToolTip("Stop the lambda");

    QAction *restartAction = menu->addAction(IconUtils::GetIcon("restart"), "Restart Lambda");
    restartAction->setToolTip("Restart the lambda");

    QAction *killAction = menu->addAction(IconUtils::GetIcon("kill"), "Kill Lambda");
    killAction->setToolTip("Kill the lambda docker container");

    menu->addSeparator();

    QAction *rebuildAction = menu->addAction(IconUtils::GetIcon("rebuild"), "Rebuild Lambda");
    rebuildAction->setToolTip("Rebuild the lambda by creating a new image and container.");

    QAction *uploadAction = menu->addAction(IconUtils::GetIcon("upload"), "Upload Lambda Code");
    uploadAction->setToolTip("Upload new lambda code");

    menu->addSeparator();

    QAction *deleteAction = menu->addAction(IconUtils::GetIcon("delete"), "Delete Lambda");
    deleteAction->setToolTip("Delete the lambda function");

    if (const QAction *selectedAction = menu->exec(_tableView->GetGlobalPosition(pos)); selectedAction == editAction) {
        if (LambdaDetailsDialog dialog(arn); dialog.exec() == QFileDialog::Accept) {
            LoadContent();
            new Awsmock::Components::ToastOverlay("Lambda updated! Name: " + name, this);
        }
    } else if (selectedAction == logsAction) {
        auto *dialog = new LambdaResultListDialog(arn);
        dialog->setModal(false);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->show();
    } else if (selectedAction == startAction) {
        if (containerId.isEmpty()) {
            QMessageBox::warning(this, tr("Lambda"), tr("Empty container ID!"));
            return;
        }
        _containerService->StartContainer(containerId);
        LoadContent();
        new Awsmock::Components::ToastOverlay("Lambda started! Name: " + name, this);
    } else if (selectedAction == enableAction) {
        _lambdaService->UpdateLambda(arn, true);
        LoadContent();
        new Awsmock::Components::ToastOverlay("Lambda enabled! Name: " + name, this);
    } else if (selectedAction == disableAction) {
        _lambdaService->UpdateLambda(arn, false);
        LoadContent();
        new Awsmock::Components::ToastOverlay("Lambda disabled! Name: " + name, this);
    } else if (selectedAction == stopAction) {
        if (containerId.isEmpty()) {
            QMessageBox::warning(this, tr("Lambda"), tr("Empty container ID!"));
            return;
        }
        _containerService->StopContainer(containerId);
        LoadContent();
        new Awsmock::Components::ToastOverlay("Lambda stopped! Name: " + name, this);
    } else if (selectedAction == restartAction) {
        if (containerId.isEmpty()) {
            QMessageBox::warning(this, tr("Lambda"), tr("Empty container ID!"));
            return;
        }
        _containerService->RestartContainer(containerId);
        LoadContent();
        new Awsmock::Components::ToastOverlay("Lambda restarted! Name: " + name, this);
    } else if (selectedAction == killAction) {
        if (containerId.isEmpty()) {
            QMessageBox::warning(this, tr("Lambda"), tr("Empty container ID!"));
            return;
        }
        _containerService->KillContainer(containerId);
        LoadContent();
        new Awsmock::Components::ToastOverlay("Lambda killed! Name: " + name, this);
    } else if (selectedAction == rebuildAction) {
        _lambdaService->RebuildLambda(name, version);
        LoadContent();
        new Awsmock::Components::ToastOverlay("Lambda container rebuild! Name: " + name, this);
    } else if (selectedAction == uploadAction) {
        if (LambdaUploadCodeDialog dialog(name, arn); dialog.exec() == QFileDialog::Accept) {
            new Awsmock::Components::ToastOverlay("Lambda code uploaded! Name: " + name, this);
        }
    } else if (selectedAction == deleteAction) {
        _lambdaService->DeleteLambda(name);
        LoadContent();
        new Awsmock::Components::ToastOverlay("Lambda deleted! Name: " + name, this);
    }
}
