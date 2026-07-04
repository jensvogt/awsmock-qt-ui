//
// Created by vogje01 on 6/27/26.
//

#include <modules/lambda/LambdaInstanceDialog.h>
#include "ui_LambdaInstanceDialog.h"

LambdaInstanceDialog::LambdaInstanceDialog(QString lambdaArn, QString instanceId, QWidget *parent) : BaseDialog(parent), _ui(new Ui::LambdaInstanceDialog), _arn(std::move(lambdaArn)), _instanceId(std::move(instanceId)) {

    // Set up service
    _lambdaService = new LambdaService();
    connect(_lambdaService, &LambdaService::GetLambdaInstanceSignal, this, &LambdaInstanceDialog::handleUpdate);

    // Set up UI
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &LambdaInstanceDialog::handleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &LambdaInstanceDialog::handleReject);

    // Refresh button
    _ui->refreshButton->setText(nullptr);
    _ui->refreshButton->setIcon(IconUtils::GetIcon("refresh"));
    connect(_ui->refreshButton, &QPushButton::clicked, this, &LambdaInstanceDialog::LoadContent);

    // Initial loading
    LambdaInstanceDialog::LoadContent();
}

LambdaInstanceDialog::~LambdaInstanceDialog() {
    delete _ui;
}

void LambdaInstanceDialog::handleAccept() {
    accept();
}

void LambdaInstanceDialog::handleReject() {
    reject();
}

void LambdaInstanceDialog::LoadContent() {
    _lambdaService->GetLambdaInstance(_arn, _instanceId);
}

void LambdaInstanceDialog::handleUpdate(const LambdaGetInstanceResponse &lambdaResponse) const {

    _ui->nameEdit->setText(lambdaResponse.lambdaInstanceCounters.lambdaName);
    _ui->instanceIdEdit->setText(lambdaResponse.lambdaInstanceCounters.instanceId);
    _ui->containerIdEdit->setText(lambdaResponse.lambdaInstanceCounters.containerId);
    _ui->statusEdit->setText(lambdaResponse.lambdaInstanceCounters.status);

    _ui->invocationsEdit->setText(QString::number(lambdaResponse.lambdaInstanceCounters.invocations));
    _ui->duationEdit->setText(QString::number(lambdaResponse.lambdaInstanceCounters.duration));
    _ui->privatePortEdit->setText(QString::number(lambdaResponse.lambdaInstanceCounters.privatePort));
    _ui->publicPortEdit->setText(QString::number(lambdaResponse.lambdaInstanceCounters.publicPort));

    _ui->lastStartedEdit->setText(DateTimeUtils::GetDateTimeFormat(lambdaResponse.lambdaInstanceCounters.lastStart));
    _ui->lastInvocationEdit->setText(DateTimeUtils::GetDateTimeFormat(lambdaResponse.lambdaInstanceCounters.lastInvocation));
    _ui->lastStopEdit->setText(DateTimeUtils::GetDateTimeFormat(lambdaResponse.lambdaInstanceCounters.lastStop));
    _ui->lastUpdateLabel->setText("Last update: " + DateTimeUtils::GetLogTimeFormat(QDateTime::currentDateTime()));
}
