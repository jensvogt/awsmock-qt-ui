//
// Created by vogje01 on 11/27/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LambdaEnvironmentAddDialog.h" resolved

#include <modules/lambda/LambdaEnvironmentDetailDialog.h>
#include "ui_LambdaEnvironmentDetailDialog.h"

LambdaEnvironmentDetailDialog::LambdaEnvironmentDetailDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::LambdaEnvironmentDetailDialog) {
    // Initialize
    Initialize();

    // Key field
    _ui->keyEdit->setText(nullptr);
    _ui->keyEdit->setDisabled(false);
    _ui->valueEdit->setText(nullptr);
    _ui->valueEdit->setDisabled(false);
}

LambdaEnvironmentDetailDialog::LambdaEnvironmentDetailDialog(const QString &key, const QString &value, QWidget *parent) : QDialog(parent), _ui(new Ui::LambdaEnvironmentDetailDialog) {
    // Initialize
    Initialize();

    _key = key;
    _value = value;

    // Key field
    _ui->keyEdit->setText(key);
    _ui->keyEdit->setDisabled(true);
    _ui->valueEdit->setText(value);
}

LambdaEnvironmentDetailDialog::~LambdaEnvironmentDetailDialog() {
    delete _ui;
}

void LambdaEnvironmentDetailDialog::Initialize() {
    // Setup dialog
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &LambdaEnvironmentDetailDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &LambdaEnvironmentDetailDialog::HandleReject);

    // Connect fields
    connect(_ui->keyEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        _key = text;
    });
    connect(_ui->valueEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        _value = text;
    });
}

void LambdaEnvironmentDetailDialog::HandleAccept() {
    accept();
}

void LambdaEnvironmentDetailDialog::HandleReject() {
    reject();
}

QString LambdaEnvironmentDetailDialog::GetKey() {
    return _key;
}

QString LambdaEnvironmentDetailDialog::GetValue() {
    return _value;
}
