//
// Created by jensv on 08/12/2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DockerStatsDIalog.h" resolved

#include <modules/docker/DockerStatsDialog.h>
#include "ui_DockerStatsDialog.h"

DockerStatsDialog::DockerStatsDialog(QWidget *parent) : BaseDialog(parent), _ui(new Ui::DockerStatsDialog) {
    // Application service
    _applicationService = new ApplicationService();

    // Event bus connection
    _statusConnection =
            connect(&EventBus::instance(), &EventBus::DockerStatsTimerSignal, [this](const QString &name, const qint64 elapsed) {
                const QString msg = "Last update: " + QDateTime::currentDateTime().toString("hh:mm:ss") + " [" + QString::number(elapsed) + "ms]";
                _ui->statusLabel->setText(msg);
            });

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
        _containerService->ListDockerStats();
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
                                    tr("Name"), tr("State"), tr("ContainerId"), tr("CPU [%]"), tr("Memory [MB]"), tr("Memory [%]"), tr("Limit [MB]"), tr("running")
                                };

    // Table
    _dataModel = new QStandardItemModel(this);
    _dataModel->setHorizontalHeaderLabels(headers);
    _dataModel->setColumnCount(static_cast<int>(headers.count()));

    // Proxy model for prefix filtering
    _proxyModel = new PrefixFilterProxyModel(this);
    _proxyModel->setSourceModel(_dataModel);
    _ui->statsTable->setModel(_proxyModel);

    _ui->statsTable->setShowGrid(true);
    _ui->statsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->statsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->statsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->statsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch); // name
    _ui->statsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents); //status
    _ui->statsTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents); // containerId
    _ui->statsTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents); // %CPU
    _ui->statsTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents); // memory
    _ui->statsTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents); // %memory
    _ui->statsTable->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents); // limit
    _ui->statsTable->setColumnHidden(7, true);

    // Enable sorting, default on and sorted ascending by name
    _ui->statsTable->setSortingEnabled(true);
    _ui->statsTable->sortByColumn(0, Qt::AscendingOrder);

    // Add context menu
    _ui->statsTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_ui->statsTable, &QTableWidget::customContextMenuRequested, this, &DockerStatsDialog::ShowContextMenu);

    // Load content
    _containerService->ListDockerStats();

    // List containers
    StartAutoUpdate();
}

DockerStatsDialog::~DockerStatsDialog() {
    disconnect(_statusConnection);
    delete _ui;
}

void DockerStatsDialog::HandleAccept() {
    accept();
}

void DockerStatsDialog::HandleReject() {
    accept();
}

void DockerStatsDialog::LoadContent() {
    _containerService->ListDockerStats();
}

void DockerStatsDialog::LoadContainerStatsContent(const DockerStatsResponse &dockerStatsResponse) {
    const int selectedRow = _ui->statsTable->selectionModel()->currentIndex().row();
    _ui->statsTable->setSortingEnabled(false);

    for (int r = 0, c = 0; r < dockerStatsResponse.containerStats.count(); r++, c = 0) {
        SetColumn(_dataModel, r, c++, dockerStatsResponse.containerStats.at(r).name);
        SetColumn(_dataModel, r, c++, dockerStatsResponse.containerStats.at(r).state.running, IconUtils::GetIcon("running"), IconUtils::GetIcon("stopped"));
        SetColumn(_dataModel, r, c++, dockerStatsResponse.containerStats.at(r).containerId.mid(0, 12));
        if (dockerStatsResponse.containerStats.at(r).state.running) {
            SetColumn(_dataModel, r, c++, GetCpuPercent(dockerStatsResponse.containerStats.at(r)));
            SetColumn(_dataModel, r, c++, dockerStatsResponse.containerStats.at(r).GetTotalMemory());
            SetColumn(_dataModel, r, c++, dockerStatsResponse.containerStats.at(r).GetPercentMemory());
            SetColumn(_dataModel, r, c++, dockerStatsResponse.containerStats.at(r).memoryStat.limit / (1024 * 1024));
        } else {
            SetColumn(_dataModel, r, c++, "--", Qt::AlignRight | Qt::AlignVCenter);
            SetColumn(_dataModel, r, c++, "--", Qt::AlignRight | Qt::AlignVCenter);
            SetColumn(_dataModel, r, c++, "--", Qt::AlignRight | Qt::AlignVCenter);
            SetColumn(_dataModel, r, c++, "--", Qt::AlignRight | Qt::AlignVCenter);
        }
        SetColumn(_dataModel, r, c++, dockerStatsResponse.containerStats.at(r).state.running);
    }
    // Reset selection
    _ui->statsTable->setSortingEnabled(true);
    _ui->statsTable->sortByColumn(_sortColumn, _sortOrder);
    _ui->statsTable->selectRow(selectedRow);

    NotifyStatusBar();
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

void DockerStatsDialog::ShowContextMenu(const QPoint &pos) {
    StopAutoUpdate();

    // Cell index
    const QModelIndex proxyIndex = _ui->statsTable->indexAt(pos);
    if (!proxyIndex.isValid()) return;

    const QModelIndex sourceIndex = _proxyModel->mapToSource(proxyIndex);

    // Get container
    const QString containerName = _dataModel->item(sourceIndex.row(), 0)->text();
    const QString containerId = _dataModel->item(sourceIndex.row(), 2)->text();
    const bool running = _dataModel->item(sourceIndex.row(), 7)->text() == "1";

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

    if (const QAction *selectedAction = menu.exec(_ui->statsTable->viewport()->mapToGlobal(pos)); selectedAction == logsAction) {
        auto *dialog = new ApplicationLogsDialog(containerName, containerId);
        dialog->setModal(false);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
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
    LoadContent();
    StartAutoUpdate();
}

