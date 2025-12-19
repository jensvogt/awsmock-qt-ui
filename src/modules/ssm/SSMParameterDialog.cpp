//
// Created by vogje01 on 12/19/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SSMParameterDialog.h" resolved

#include <modules/ssm/SSMParameterDialog.h>
#include "ui_SSMParameterDialog.h"

SSMParameterDialog::SSMParameterDialog(const QString &parameterName, QWidget *parent) : BaseDialog(parent), _ui(new Ui::SSMParameterDialog), _parameterName(parameterName) {

    // Connect service
    _ssmService = new SSMService();
    connect(_ssmService, &SSMService::GetParameterSignal, this, &SSMParameterDialog::HandleParameterGetSignal);

    // Initialize UI components
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &SSMParameterDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &SSMParameterDialog::HandleReject);

    SSMParameterDialog::LoadContent();
}

SSMParameterDialog::~SSMParameterDialog() {
    delete _ui;
}

void SSMParameterDialog::HandleAccept() {
    accept();
}

void SSMParameterDialog::HandleReject() {
    accept();
}

void SSMParameterDialog::LoadContent() {
    _ssmService->GetParameter(_parameterName);
}

void SSMParameterDialog::HandleParameterGetSignal(const SSMParameterGetResponse &parameterGetResponse) const {

    _ui->regionEdit->setText(parameterGetResponse.parameter.region);
    _ui->nameEdit->setText(parameterGetResponse.parameter.name);
    _ui->arnEdit->setText(parameterGetResponse.parameter.arn);
    _ui->kmsKeyIdEdit->setText(parameterGetResponse.parameter.kmsKeyArn);
    _ui->createdEdit->setText(parameterGetResponse.parameter.created.toString("yyyy-MM-dd hh:mm:ss"));
    _ui->modifiedEdit->setText(parameterGetResponse.parameter.modified.toString("yyyy-MM-dd hh:mm:ss"));

    // Value field
    _ui->secretStringEdit->setEchoMode(QLineEdit::Password);
    _ui->secretStringEdit->setText(QString(QByteArray::fromBase64(parameterGetResponse.parameter.parameterValue.toUtf8())));
    _ui->showValueButton->setText(nullptr);
    _ui->showValueButton->setIcon(IconUtils::GetIcon("show"));
    connect(_ui->showValueButton, &QPushButton::toggled, [this](const bool checked) {
        if (checked) {
            _ui->secretStringEdit->setEchoMode(QLineEdit::Normal);
        } else {
            _ui->secretStringEdit->setEchoMode(QLineEdit::Password);
        }
    });
}
