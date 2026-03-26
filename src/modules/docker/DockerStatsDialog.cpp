
#include <modules/docker/DockerStatsDialog.h>
#include "ui_DockerStatsDialog.h"

DockerStatsDialog::DockerStatsDialog(QWidget *parent) : BaseDialog(parent), _ui(new Ui::DockerStatsDialog) {
    // Application service
    _applicationService = new ApplicationService();

    // Connect service
    _containerService = new DockerService();
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
        LoadContent();
    });

    const QStringList headers = QStringList() = {
                                    tr("Name"), tr("State"), tr("ContainerId"), tr("CPU [%]"), tr("Memory [MB]"), tr("Memory [%]"), tr("Limit [MB]"), tr("running")
                                };

    // Table
    _ui->statsTable->SetHeaderNames(headers);
    _ui->statsTable->SetResizeModes({
        QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents
    });
    _ui->statsTable->SetHiddenColumns({7});
    _ui->statsTable->SetSorting(1, "name", -1);

    // Connect paging changes
    connect(_ui->statsTable, &PageableTable::ReloadTable, this, &DockerStatsDialog::LoadContent);

    // Add context menu
    connect(_ui->statsTable, &PageableTable::ContextMenuRequested, this, &DockerStatsDialog::ShowContextMenu);

    // Start auto updater
    StartAutoUpdate();
}

DockerStatsDialog::~DockerStatsDialog() {
    StopAutoUpdate();
    delete _ui;
}

void DockerStatsDialog::HandleAccept() {
    accept();
}

void DockerStatsDialog::HandleReject() {
    accept();
}

void DockerStatsDialog::LoadContent() {
    _ui->statsTable->SaveSelection();
    _ui->statsTable->Clear();
    _containerService->ListDockerStats();
}

void DockerStatsDialog::LoadContainerStatsContent(const DockerStatsResponse &dockerStatsResponse) {
    _ui->statsTable->SetTotalSize(dockerStatsResponse.total);
    for (auto r = 0, c = 0; r < dockerStatsResponse.containerStats.count(); r++, c = 0) {
        _ui->statsTable->SetColumn(r, c++, dockerStatsResponse.containerStats.at(r).name);
        _ui->statsTable->SetColumn(r, c++, dockerStatsResponse.containerStats.at(r).state.running, IconUtils::GetIcon("running"), IconUtils::GetIcon("stopped"));
        _ui->statsTable->SetColumn(r, c++, dockerStatsResponse.containerStats.at(r).containerId.mid(0, 12));
        if (dockerStatsResponse.containerStats.at(r).state.running) {
            _ui->statsTable->SetColumn(r, c++, GetCpuPercent(dockerStatsResponse.containerStats.at(r)));
            _ui->statsTable->SetColumn(r, c++, dockerStatsResponse.containerStats.at(r).GetTotalMemory());
            _ui->statsTable->SetColumn(r, c++, dockerStatsResponse.containerStats.at(r).GetPercentMemory());
            _ui->statsTable->SetColumn(r, c++, dockerStatsResponse.containerStats.at(r).memoryStat.limit / (1024 * 1024));
        } else {
            _ui->statsTable->SetColumn(r, c++, "--", Qt::AlignRight | Qt::AlignVCenter);
            _ui->statsTable->SetColumn(r, c++, "--", Qt::AlignRight | Qt::AlignVCenter);
            _ui->statsTable->SetColumn(r, c++, "--", Qt::AlignRight | Qt::AlignVCenter);
            _ui->statsTable->SetColumn(r, c++, "--", Qt::AlignRight | Qt::AlignVCenter);
        }
        _ui->statsTable->SetColumn(r, c++, dockerStatsResponse.containerStats.at(r).state.running);
    }
    _ui->statsTable->UpdateSorting();
    _ui->statsTable->RestoreSelection();
}

double DockerStatsDialog::GetCpuPercent(const ContainerStat &containerStats) {
    double cpuPercent = 0.0;
    if (_oldCpuTotal[containerStats.containerId] > 0 && _oldCpuSystem[containerStats.containerId] > 0) {
        const auto delta = static_cast<double>(containerStats.cpuStats.cpuUsage.totalUsage - _oldCpuTotal[containerStats.containerId]);
        if (const auto deltaSystem = static_cast<double>(containerStats.cpuStats.cpuUsage.systemUsage - _oldCpuSystem[containerStats.containerId]); deltaSystem > 0 && delta > 0) {
            cpuPercent = delta / deltaSystem / containerStats.cpuStats.onlineCpus * 100;
        } else {
            cpuPercent = 0.0;
        }
    }
    _oldCpuTotal[containerStats.containerId] = containerStats.cpuStats.cpuUsage.totalUsage;
    _oldCpuSystem[containerStats.containerId] = containerStats.cpuStats.cpuUsage.systemUsage;
    return cpuPercent;
}

void DockerStatsDialog::ShowContextMenu(const QPoint &pos) const {

    // Cell index
    const QModelIndex index = _ui->statsTable->GetIndexFromPosition(pos);

    // Get container
    const auto containerName = _ui->statsTable->GetValue<QString>(index, 0);
    const auto containerId = _ui->statsTable->GetValue<QString>(index, 2);
    const bool running = _ui->statsTable->GetValue<bool>(index, 7);

    QMenu menu;
    QAction *logsAction = menu.addAction(IconUtils::GetIcon("logs"), "Show the container logs");
    logsAction->setToolTip("Show the container logs");
    logsAction->setDisabled(!running);

    menu.addSeparator();

    QAction *startAction = menu.addAction(IconUtils::GetIcon("start"), "Start Container");
    startAction->setToolTip("Start the container");
    startAction->setDisabled(running);

    QAction *stopAction = menu.addAction(IconUtils::GetIcon("stop"), "Stop Container");
    stopAction->setToolTip("Stop the container gracefully");
    stopAction->setDisabled(!running);

    QAction *restartAction = menu.addAction(IconUtils::GetIcon("restart"), "Restart Container");
    restartAction->setToolTip("Restart the container");

    QAction *killAction = menu.addAction(IconUtils::GetIcon("kill"), "Kill Container");
    killAction->setToolTip("Kill the container");
    killAction->setDisabled(!running);

    menu.addSeparator();

    QAction *rebuildAction = menu.addAction(IconUtils::GetIcon("rebuild"), "Rebuild Container");
    rebuildAction->setToolTip("Rebuild the container by creating a new container from the provided image.");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Container");
    deleteAction->setToolTip("Delete the container");

    if (const QAction *selectedAction = menu.exec(_ui->statsTable->GetGlobalPosition(pos)); selectedAction == logsAction) {
        auto *dialog = new ApplicationLogsDialog(containerName, containerId);
        dialog->setModal(false);
        dialog->show();
    } else if (selectedAction == startAction) {
        _containerService->StartContainer(containerId);
    } else if (selectedAction == stopAction) {
        _containerService->StopContainer(containerId);
    } else if (selectedAction == restartAction) {
        _containerService->RestartContainer(containerId);
    } else if (selectedAction == killAction) {
        _containerService->KillContainer(containerId);
    } else if (selectedAction == deleteAction) {
        _containerService->DeleteContainer(containerId);
    }
}

