//
// Created by jensv on 08/12/2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DockerStatsDIalog.h" resolved

#include <modules/docker/DockerStatsDialog.h>
#include "ui_DockerStatsDialog.h"

DockerStatsDialog::DockerStatsDialog(QWidget *parent) : BaseDialog(parent), _ui(new Ui::DockerStatsDialog) {
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

    const QStringList headers = QStringList() = {
                                    tr("Name"), tr("ContainerId"), tr("CPU [%]"), tr("Memory [MB]"), tr("Memory [%]")
                                };

    // Table
    _ui->statsTable->setColumnCount(static_cast<int>(headers.count()));
    _ui->statsTable->setShowGrid(true);
    _ui->statsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->statsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->statsTable->setHorizontalHeaderLabels(headers);
    _ui->statsTable->setSortingEnabled(true);
    _ui->statsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->statsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    _ui->statsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    _ui->statsTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    _ui->statsTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    _ui->statsTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);

    // List containers
    LoadContent();
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

void DockerStatsDialog::LoadContent() const {
    _containerService->ListDockerContainer("");
}

void DockerStatsDialog::LoadContainers(const DockerContainersResponse &dockerContainersResponse) {
    const int selectedRow = _ui->statsTable->selectionModel()->currentIndex().row();
    _ui->statsTable->setRowCount(0);
    _ui->statsTable->setSortingEnabled(false);
    for (auto r = 0, c = 0; r < dockerContainersResponse.containers.count(); r++, c = 0) {
        _ui->statsTable->insertRow(r);
        _containerIds.append(dockerContainersResponse.containers.at(r).id);
        SetColumn(_ui->statsTable, r, c++, dockerContainersResponse.containers.at(r).GetPrincipalName());
        SetColumn(_ui->statsTable, r, c++, dockerContainersResponse.containers.at(r).id);
        SetColumn(_ui->statsTable, r, c++, "--");
        SetColumn(_ui->statsTable, r, c++, "--");
        SetColumn(_ui->statsTable, r, c++, "--");
    }
    // Load Statistics
    _containerService->ListDockerStats(_containerIds);

    // Reset selection
    _ui->statsTable->setRowCount(static_cast<int>(dockerContainersResponse.containers.count()));
    _ui->statsTable->setSortingEnabled(true);
    _ui->statsTable->sortItems(_sortColumn, _sortOrder);
    _ui->statsTable->selectRow(selectedRow);
    NotifyStatusBar();

    // Load statistics
    QList<QString> containerId;
    for (const auto &container: dockerContainersResponse.containers) {
        containerId.append(container.id);
    }
    _containerService->ListDockerStats(containerId);
}

void DockerStatsDialog::LoadContainerStatsContent(const DockerStatsResponse &dockerStatsResponse) const {
    for (int r = 0, c = 2; r < dockerStatsResponse.containerStats.count(); r++, c = 2) {
        SetColumn(_ui->statsTable, r, c++, dockerStatsResponse.containerStats.at(r).GetCpuPercent());
        SetColumn(_ui->statsTable, r, c++, dockerStatsResponse.containerStats.at(r).GetTotalMemory());
        SetColumn(_ui->statsTable, r, c++, dockerStatsResponse.containerStats.at(r).GetPercentMemory());
    }
}

