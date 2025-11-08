//
// Created by vogje01 on 11/7/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ApplicationEditDIalog.h" resolved

#include <modules/application/ApplicationEditDialog.h>
#include "ui_ApplicationEditDialog.h"

ApplicationEditDialog::ApplicationEditDialog(const QString &name, QWidget *parent) : QDialog(parent), _ui(new Ui::ApplicationEditDialog) {

    _applicationService = new ApplicationService();

    _applicationService->GetApplication(name);
    connect(_applicationService, &ApplicationService::GetApplicationDetailsSignal, this, &ApplicationEditDialog::UpdateApplication);

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &ApplicationEditDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &ApplicationEditDialog::HandleReject);

    connect(_ui->enabledCheckBox, &QCheckBox::stateChanged, this, [this]() {
        if (_ui->enabledCheckBox->isChecked()) {
            _applicationService->EnableApplication(_ui->nameEdit->text());
        } else {
            _applicationService->DisableApplication(_ui->nameEdit->text());
        }
    });
}

ApplicationEditDialog::~ApplicationEditDialog() {
    delete _ui;
}

void ApplicationEditDialog::UpdateApplication(const ApplicationGetResponse &applicationGetResponse) {

    _ui->nameEdit->setText(applicationGetResponse.application.name);
    _ui->runtimeEdit->setText(applicationGetResponse.application.runtime);
    _ui->runTypeEdit->setText(applicationGetResponse.application.runType);
    _ui->privatePortEdit->setText(QString::number(applicationGetResponse.application.privatePort));
    _ui->publicPortEdit->setText(QString::number(applicationGetResponse.application.publicPort));
    _ui->archiveEdit->setText(applicationGetResponse.application.archive);
    _ui->versionEdit->setText(applicationGetResponse.application.version);
    _ui->imageIdEdit->setText(applicationGetResponse.application.imageId);
    _ui->containerIdEdit->setText(applicationGetResponse.application.containerId);
    _ui->containerNameEdit->setText(applicationGetResponse.application.containerName);
    //_ui->statusEdit->setText(applicationGetResponse.application.status);
    _ui->enabledCheckBox->setChecked(applicationGetResponse.application.enabled);
    _ui->lastStartedEdit->setText(applicationGetResponse.application.lastStarted.toString("yyyy-MM-dd hh:mm:ss"));
    _ui->createdEdit->setText(applicationGetResponse.application.created.toString("yyyy-MM-dd hh:mm:ss"));
    _ui->modifiedEdit->setText(applicationGetResponse.application.modified.toString("yyyy-MM-dd hh:mm:ss"));

    connect(_ui->enabledCheckBox, &QCheckBox::stateChanged, this, [&]() { this->_changed = true; });
}

void ApplicationEditDialog::HandleAccept() {
    if (this->_changed) {

    }
    accept();
}


void ApplicationEditDialog::HandleReject() {
    accept();
}
