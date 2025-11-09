//
// Created by vogje01 on 11/9/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ApplicationTagDialog.h" resolved

#include <modules/application/ApplicationTagDialog.h>
#include "ui_ApplicationTagDialog.h"


ApplicationTagDialog::ApplicationTagDialog(const QString &key, const QString &value, const bool add, QWidget *parent) : QDialog(parent), _ui(new Ui::ApplicationTagDialog) {
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &ApplicationTagDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &ApplicationTagDialog::HandleReject);

    _ui->keyEdit->setText(key);
    _ui->valueEdit->setText(value);
    connect(_ui->keyEdit, &QLineEdit::textChanged, this, [&](const QString &text) {
        _key = text;
    });
    connect(_ui->valueEdit, &QLineEdit::textChanged, this, [&](const QString &text) {
        _value = text;
    });
    _ui->keyEdit->setDisabled(!add);
}

ApplicationTagDialog::~ApplicationTagDialog() {
    delete _ui;
}


void ApplicationTagDialog::HandleAccept() {
    accept();
}

void ApplicationTagDialog::HandleReject() {
    accept();
}

QString ApplicationTagDialog::GetKey() {
    return _key;
}

QString ApplicationTagDialog::GetValue() {
    return _value;
}
