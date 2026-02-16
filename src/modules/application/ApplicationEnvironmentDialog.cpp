//
// Created by vogje01 on 11/9/25.
//

#include <modules/application/ApplicationEnvironmentDialog.h>
#include "ui_ApplicationEnvironmentDialog.h"

ApplicationEnvironmentDialog::ApplicationEnvironmentDialog(const QString &key, const QString &value, QWidget *parent) : QDialog(parent), _ui(new Ui::ApplicationEnvironmentDialog), _key(key), _value(value) {

    // Initialize
    Initialize();

    _ui->keyEdit->setText(key);
    _ui->valueEdit->setText(value);
    _ui->keyEdit->setReadOnly(true);
    connect(_ui->keyEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        _key = text;
    });
    connect(_ui->valueEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        _value = text;
    });
    setWindowTitle(QString("Edit Environment"));
}

ApplicationEnvironmentDialog::ApplicationEnvironmentDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::ApplicationEnvironmentDialog) {

    // Initialize
    Initialize();

    connect(_ui->keyEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        _key = text;
    });
    connect(_ui->valueEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        _value = text;
    });
    setWindowTitle(QString("Add Environment"));
}

ApplicationEnvironmentDialog::~ApplicationEnvironmentDialog() {
    delete _ui;
}

void ApplicationEnvironmentDialog::Initialize() {
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &ApplicationEnvironmentDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &ApplicationEnvironmentDialog::HandleReject);
}

void ApplicationEnvironmentDialog::HandleAccept() {
    accept();
}

void ApplicationEnvironmentDialog::HandleReject() {
    accept();
}

QString ApplicationEnvironmentDialog::GetKey() {
    return _key;
}

QString ApplicationEnvironmentDialog::GetValue() {
    return _value;
}
