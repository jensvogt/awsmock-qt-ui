//
// Created by jensv on 08/12/2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DockerStatsDIalog.h" resolved

#include <modules/docker/DockerStatsDialog.h>
#include "ui_DockerStatsDialog.h"

DockerStatsDialog::DockerStatsDialog(QWidget *parent) : BaseDialog(parent), _ui(new Ui::DockerStatsDialog) {

    // Application service
    _applicationService = new ApplicationService();

    // Connect service
    _containerService = new DockerService();
    connect(_containerService, &DockerService::ReloadDockerContainerSignal, this, &DockerStatsDialog::LoadContainers);
    connect(_containerService, &DockerService::ReloadDockerStatsSignal, this, &DockerStatsDialog::LoadContainerStatsContent);

    // Setup UI components
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &DockerStatsDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &DockerStatsDialog::HandleReject);

    // Toolbar refresh action
    _ui->refreshButton->setText(nullptr);
    _ui->refreshButton->setIcon(IconUtils::GetIcon("refresh"));
    _ui->refreshButton->setToolTip("Refresh the container list");
    connect(_ui->refreshButton, &QPushButton::clicked, this, [this]() {
        if (!_containerIds.empty()) {
            _containerService->ListDockerStats(_containerIds);
        }
    });

    // Prefix edit
    _ui->prefixEdit->setPlaceholderText("Prefix");
    _ui->prefixEdit->setEnabled(true);
    connect(_ui->prefixEdit, &QLineEdit::textChanged, this, [this]() {
        _ui->prefixClearButton->setDisabled(false);
        _proxyModel->setFilterColumn(0);
        _proxyModel->setFilterPrefix(_ui->prefixEdit->text());
    });

    // Prefix clear button
    _ui->prefixClearButton->setDisabled(true);
    _ui->prefixClearButton->setText(nullptr);
    _ui->prefixClearButton->setIcon(IconUtils::GetIcon("clear"));
    _ui->prefixClearButton->setToolTip("Clear the prefix field");
    connect(_ui->prefixClearButton, &QPushButton::clicked, this, [this]() {
        _proxyModel->clearFilter();
        _ui->prefixClearButton->setDisabled(true);
    });

    const QStringList headers = QStringList() = {
                                    tr("Name"), tr("ContainerId"), tr("CPU [%]"), tr("Memory [MB]"), tr("Memory [%]"), tr("Limit [MB]")
                                };

    // Table
    _dataModel = new QStandardItemModel(this);
    _dataModel->setHorizontalHeaderLabels(headers);
    _dataModel->setColumnCount(headers.count());

    // Proxy model for prefix filtering
    _proxyModel = new PrefixFilterProxyModel(this);
    _proxyModel->setSourceModel(_dataModel);
    _ui->statsTable->setModel(_proxyModel);

    _ui->statsTable->setShowGrid(true);
    _ui->statsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->statsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->statsTable->setSortingEnabled(true);
    _ui->statsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->statsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    _ui->statsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    _ui->statsTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    _ui->statsTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    _ui->statsTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    _ui->statsTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);

    // Add context menu
    _ui->statsTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_ui->statsTable, &QTableWidget::customContextMenuRequested, this, &DockerStatsDialog::ShowContextMenu);

    // Load content
    _containerService->ListDockerContainer(_prefixValue);

    // List containers
    StartAutoUpdate();
}

DockerStatsDialog::~DockerStatsDialog() {
    delete _ui;
}

void DockerStatsDialog::HandleAccept() {
    accept();
}

void DockerStatsDialog::HandleReject() {
    accept();
}

void DockerStatsDialog::LoadContent() {
    if (!_containerIds.empty()) {
        _containerService->ListDockerStats(_containerIds);
    }
}

void DockerStatsDialog::LoadContainers(const DockerContainersResponse &dockerContainersResponse) {
    const int selectedRow = _ui->statsTable->selectionModel()->currentIndex().row();
    _ui->statsTable->setSortingEnabled(false);
    for (auto r = 0, c = 0; r < dockerContainersResponse.containers.count(); r++, c = 0) {
        _containerIds.append(dockerContainersResponse.containers.at(r).id);
        SetColumn(_dataModel, r, c++, dockerContainersResponse.containers.at(r).GetPrincipalName());
        SetColumn(_dataModel, r, c++, dockerContainersResponse.containers.at(r).id);
        SetColumn(_dataModel, r, c++, "--");
        SetColumn(_dataModel, r, c++, "--");
        SetColumn(_dataModel, r, c++, "--");
    }

    // Load Statistics
    _containerService->ListDockerStats(_containerIds);

    // Reset selection
    _ui->statsTable->setSortingEnabled(true);
    //    _ui->statsTable->sortItems(_sortColumn, _sortOrder);
    _ui->statsTable->selectRow(selectedRow);

    _containerService->ListDockerStats(_containerIds);

    NotifyStatusBar();
}

void DockerStatsDialog::LoadContainerStatsContent(const DockerStatsResponse &dockerStatsResponse) {

    for (int r = 0, c = 2; r < dockerStatsResponse.containerStats.count(); r++, c = 2) {
        SetColumn(_dataModel, r, c++, GetCpuPercent(dockerStatsResponse.containerStats.at(r)));
        SetColumn(_dataModel, r, c++, dockerStatsResponse.containerStats.at(r).GetTotalMemory());
        SetColumn(_dataModel, r, c++, dockerStatsResponse.containerStats.at(r).GetPercentMemory());
        SetColumn(_dataModel, r, c++, dockerStatsResponse.containerStats.at(r).memoryStat.limit / (1024 * 1024));
    }
}

double DockerStatsDialog::GetCpuPercent(const ContainerStat &containerStats) {
    double cpuPercent = 0.0;
    if (_oldCpuTotal[containerStats.containerId] > 0 && _oldCpuSystem[containerStats.containerId] > 0) {
        const double delta = static_cast<double>(containerStats.cpuStats.cpuUsage.totalUsage - _oldCpuTotal[containerStats.containerId]);
        const double deltaSystem = static_cast<double>(containerStats.cpuStats.cpuUsage.systemUsage - _oldCpuSystem[containerStats.containerId]);
        if (deltaSystem > 0 && delta > 0) {
            cpuPercent = delta / deltaSystem / containerStats.cpuStats.onlineCpus * 100;
        } else {
            cpuPercent = 0.0;
        }
    }
    _oldCpuTotal[containerStats.containerId] = containerStats.cpuStats.cpuUsage.totalUsage;
    _oldCpuSystem[containerStats.containerId] = containerStats.cpuStats.cpuUsage.systemUsage;
    return cpuPercent;
}

void DockerStatsDialog::ShowContextMenu(const QPoint &pos) {
    StopAutoUpdate();

    // Cell index
    const QModelIndex index = _ui->statsTable->indexAt(pos);
    if (!index.isValid()) return;

    const int row = index.row();

    const QString containerName = _dataModel->item(row, 0)->text();
    const QString containerId = _dataModel->item(row, 1)->text();

    QMenu menu;
    /*QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit Application");
    editAction->setToolTip("Edit the application details.");*/

    QAction *logsAction = menu.addAction(IconUtils::GetIcon("logs"), "Show the container logs");
    logsAction->setToolTip("Show the container logs");
    if (containerId.isEmpty()) {
        logsAction->setDisabled(true);
    }

    menu.addSeparator();

    /*    QAction *enableAction = menu.addAction(IconUtils::GetIcon("enabled"), "Enable Application");
        enableAction->setToolTip("Enable the application.");

        QAction *disableAction = menu.addAction(IconUtils::GetIcon("disabled"), "Disable Application");
        disableAction->setToolTip("Disable the application.");

        menu.addSeparator();*/

    QAction *startAction = menu.addAction(IconUtils::GetIcon("start"), "Start Container");
    startAction->setToolTip("Start the container");

    QAction *stopAction = menu.addAction(IconUtils::GetIcon("stop"), "Stop Container");
    stopAction->setToolTip("Stop the container");

    QAction *restartAction = menu.addAction(IconUtils::GetIcon("restart"), "Restart Container");
    restartAction->setToolTip("Restart the container");

    menu.addSeparator();

    QAction *rebuildAction = menu.addAction(IconUtils::GetIcon("rebuild"), "Rebuild Container");
    rebuildAction->setToolTip("Rebuild the container by creating a new container from the provided image.");

    //QAction *uploadAction = menu.addAction(IconUtils::GetIcon("upload"), "Upload Application Code");
    //uploadAction->setToolTip("Upload new application code");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Container");
    deleteAction->setToolTip("Delete the container");
    const QAction *selectedAction = menu.exec(_ui->statsTable->viewport()->mapToGlobal(pos));

    /*if (const QAction *selectedAction = menu.exec(_ui->statsTable->viewport()->mapToGlobal(pos));
        selectedAction == editAction) {
        ApplicationEditDialog dialog(containerName);
        dialog.exec();
    } else*/
    if (selectedAction == logsAction) {
        auto *dialog = new ApplicationLogsDialog(containerName, containerId);
        dialog->setModal(false);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->show();
    } /*else if (selectedAction == startAction) {
        _applicationService->StartApplication(containerName);
    } else if (selectedAction == enableAction) {
        _applicationService->EnableApplication(containerName);
    } else if (selectedAction == disableAction) {
        _applicationService->DisableApplication(containerName);
    } else if (selectedAction == stopAction) {
        _applicationService->StopApplication(containerName);
    } else if (selectedAction == restartAction) {
        _applicationService->RestartApplication(containerName);
    } else if (selectedAction == rebuildAction) {
        _applicationService->RebuildApplication(containerName);
    } else if (selectedAction == uploadAction) {
        ApplicationUploadCodeDialog dialog(containerName);
        dialog.exec();
    } else if (selectedAction == deleteAction) {
        _applicationService->DeleteApplication(containerName);
    } else if (selectedAction == editAction) {
        ApplicationEditDialog dialog(containerName);
        dialog.exec();
    }*/
    LoadContent();
    StartAutoUpdate();
}
