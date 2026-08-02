//
// Created by jensv on 01/08/2026.
//

#include <modules/sqs/SQSQueueLambdaTriggerDialog.h>
#include "ui_SQSQueueLambdaTriggerDialog.h"

SQSQueueLambdaTriggerDialog::SQSQueueLambdaTriggerDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::SQSQueueLambdaTriggerDialog) {

    // Lambda REST service
    _lambdaService = new LambdaService();

    // Setup UI components
    _ui->setupUi(this);

    // Connect OK/CANCEL buttons
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &SQSQueueLambdaTriggerDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &SQSQueueLambdaTriggerDialog::HandleReject);

    // Enabled checkbox
    _ui->enabledCheckBox->setChecked(_enabled);

    // Fill in the lambda ARN combo box
    _lambdaService->ListLambdaArns();
    connect(_lambdaService, &LambdaService::ListLambdaArnsSignal, this, [this](const LambdaListArnsResponse &response) {
        _ui->lambdaCombo->addItems(response.lambdaArns);
    });
}

SQSQueueLambdaTriggerDialog::~SQSQueueLambdaTriggerDialog() {
    delete _lambdaService;
    delete _ui;
}

void SQSQueueLambdaTriggerDialog::HandleAccept() {
    if (_ui->lambdaCombo->currentText().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a lambda function!");
        return;
    }
    _lambdaArn = _ui->lambdaCombo->currentText();
    _enabled = _ui->enabledCheckBox->isChecked();
    accept();
}

void SQSQueueLambdaTriggerDialog::HandleReject() {
    reject();
}
