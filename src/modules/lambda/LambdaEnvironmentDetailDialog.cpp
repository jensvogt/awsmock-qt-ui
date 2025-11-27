//
// Created by vogje01 on 11/27/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LambdaEnvironmentAddDialog.h" resolved

#include <modules/lambda/LambdaEnvironmentDetailDialog.h>
#include "ui_LambdaEnvironmentDetailDialog.h"


LambdaEnvironmentDetailDialog::LambdaEnvironmentDetailDialog(const QString &key, const QString &value, QWidget *parent) : QDialog(parent), _ui(new Ui::LambdaEnvironmentDetailDialog) {

    // Setup dialog
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &LambdaEnvironmentDetailDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &LambdaEnvironmentDetailDialog::HandleReject);

    connect(_ui->keyEdit, &QLineEdit::textChanged, this, [this]() {
        _key = _ui->keyEdit->text();
    });
    connect(_ui->valueEdit, &QLineEdit::textChanged, this, [this]() {
        _value = _ui->valueEdit->text();
    });
}

LambdaEnvironmentDetailDialog::~LambdaEnvironmentDetailDialog() {
    delete _ui;
}

void LambdaEnvironmentDetailDialog::HandleAccept() {
    accept();
}

void LambdaEnvironmentDetailDialog::HandleReject() {
    accept();
}

QString LambdaEnvironmentDetailDialog::GetKey() {
    return _key;
}

QString LambdaEnvironmentDetailDialog::GetValue() {
    return _value;
}
