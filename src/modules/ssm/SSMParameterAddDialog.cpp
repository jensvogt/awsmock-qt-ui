//
// Created by vogje01 on 12/19/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SSMParameterAddDialog.h" resolved

#include <modules/ssm/SSMParameterAddDialog.h>
#include "ui_SSMParameterAddDialog.h"

SSMParameterAddDialog::SSMParameterAddDialog(QWidget *parent) : BaseDialog(parent), _ui(new Ui::SSMParameterAddDialog) {

    // Connect service
    _ssmService = new SSMService();
    _kmsService = new KMSService();
    _kmsService->ListKmsKeys();
    connect(_kmsService, &KMSService::ListKeysSignal, this, [this](const KMSListKeysResponse &kmsResponse) {
        QStringList keys;
        for (const auto &key: kmsResponse.keyCounters) {
            keys.append(key.arn);
        }
        _ui->kmsKeyCombo->addItems(keys);
        _ui->kmsKeyCombo->setCurrentIndex(0);
    });

    // Initialize UI components
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &SSMParameterAddDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &SSMParameterAddDialog::HandleReject);

    // Region edit
    _ui->regionEdit->setText(Configuration::instance().GetValue("aws.region", "eu-central-1"));
    _ui->nameEdit->setText(Configuration::instance().GetValue("aws.name", ""));
}

SSMParameterAddDialog::~SSMParameterAddDialog() {
    delete _ui;
}

void SSMParameterAddDialog::HandleAccept() {
    SSMParameterCounter parameter;
    parameter.name = _ui->nameEdit->text();
    parameter.parameterValue = _ui->valueEdit->text();
    parameter.kmsKeyArn = _ui->kmsKeyCombo->currentText();
    parameter.region = _ui->regionEdit->text();
    parameter.description = _ui->descriptionEdit->toPlainText();
    _ssmService->CreateParameter(parameter);
    accept();
}

void SSMParameterAddDialog::HandleReject() {
    accept();
}

void SSMParameterAddDialog::LoadContent() {

}
