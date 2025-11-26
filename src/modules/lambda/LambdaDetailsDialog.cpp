//
// Created by vogje01 on 11/25/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LambdaDetailsDialog.h" resolved

#include <modules/lambda/LambdaDetailsDialog.h>
#include "ui_LambdaDetailsDialog.h"

LambdaDetailsDialog::LambdaDetailsDialog(const QString &lambdaArn, QWidget *parent) : QDialog(parent), _ui(new Ui::LambdaDetailsDialog), _lambdaArn(lambdaArn) {
    _lambdaService = new LambdaService();

    _lambdaService->GetLambda(lambdaArn);
    connect(_lambdaService, &LambdaService::GetLambdaDetailsSignal, this, &LambdaDetailsDialog::UpdateLambda);

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &LambdaDetailsDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &LambdaDetailsDialog::HandleReject);

    // Connect refresh button
    _ui->refreshButton->setText(nullptr);
    _ui->refreshButton->setIcon(IconUtils::GetIcon("refresh"));
    connect(_ui->refreshButton, &QPushButton::clicked, this, [this]() {
        _lambdaService->GetLambda(_lambdaArn);
    });
}

LambdaDetailsDialog::~LambdaDetailsDialog() {
    delete _ui;
}

void LambdaDetailsDialog::UpdateLambda(const LambdaGetResponse &lambdaGetResponse) const {
    _ui->regionEdit->setText(lambdaGetResponse.region);
    _ui->nameEdit->setText(lambdaGetResponse.lambdaName);
    _ui->arnEdit->setText(lambdaGetResponse.lambdaArn);
    _ui->handlerEdit->setText(lambdaGetResponse.handler);
    _ui->roleEdit->setText(lambdaGetResponse.role);
    _ui->userEdit->setText(lambdaGetResponse.user);
    _ui->runtimeEdit->setText(lambdaGetResponse.runtime);
    _ui->versionEdit->setText(lambdaGetResponse.version);
    _ui->sizeEdit->setText(QString::number(lambdaGetResponse.size));
    _ui->concurrencyEdit->setText(QString::number(lambdaGetResponse.concurrency));
    _ui->instancesEdit->setText(QString::number(lambdaGetResponse.instances));
    _ui->invocationsEdit->setText(QString::number(lambdaGetResponse.invocations));
    _ui->avgExecutionEdit->setText(QString::number(lambdaGetResponse.averageRuntime));
    _ui->zipFileEdit->setText(lambdaGetResponse.zipFile);
    _ui->statusEdit->setText(lambdaGetResponse.state);
}

void LambdaDetailsDialog::HandleAccept() {
    accept();
}

void LambdaDetailsDialog::HandleReject() {
    accept();
}
