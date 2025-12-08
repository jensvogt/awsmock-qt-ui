//
// Created by jensv on 08/12/2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DockerStatsDIalog.h" resolved

#include <modules/docker/DockerStatsDialog.h>
#include "ui_DockerStatsDIalog.h"

DockerStatsDialog::DockerStatsDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::DockerStatsDialog) {
    // Connect service
    _containerService = new DockerService();
    connect(_containerService, &DockerService::ReloadDockerStatsSignal, this, &DockerStatsDialog::LoadContent);

    // Setup UI components
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &DockerStatsDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &DockerStatsDialog::HandleReject);
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

void DockerStatsDialog::LoadContent(const DockerStatsResponse &dockerStatsResponse) {
}

