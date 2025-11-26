//
// Created by vogje01 on 11/9/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ApplicationEnvironmentEditDialog.h" resolved

#include <modules/application/ApplicationEnvironmentEditDialog.h>
#include "ui_ApplicationEnvironmentEditDialog.h"

ApplicationEnvironmentEditDialog::ApplicationEnvironmentEditDialog(const QString &key, const QString &value, const bool add, QWidget *parent) : QDialog(parent), _ui(new Ui::ApplicationEnvironmentEditDialog), _key(key), _value(value) {
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &ApplicationEnvironmentEditDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &ApplicationEnvironmentEditDialog::HandleReject);

    _ui->keyEdit->setText(key);
    _ui->valueEdit->setText(value);
    connect(_ui->keyEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        _key = text;
    });
    connect(_ui->valueEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        _value = text;
    });
    _ui->keyEdit->setDisabled(!add);
}

ApplicationEnvironmentEditDialog::~ApplicationEnvironmentEditDialog() {
    delete _ui;
}

void ApplicationEnvironmentEditDialog::HandleAccept() {
    accept();
}

void ApplicationEnvironmentEditDialog::HandleReject() {
    accept();
}

QString ApplicationEnvironmentEditDialog::GetKey() {
    return _key;
}

QString ApplicationEnvironmentEditDialog::GetValue() {
    return _value;
}
