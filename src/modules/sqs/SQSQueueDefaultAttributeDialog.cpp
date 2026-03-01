//
// Created by jensv on 19/02/2026.
//

#include <modules/sqs/SQSQueueDefaultAttributeDialog.h>
#include "ui_SQSQueueDefaultAttributeDialog.h"

SQSQueueDefaultAttributeDialog::SQSQueueDefaultAttributeDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::SQSQueueDefaultAttributeDialog) {

    // Initialization
    Initialize();
}

SQSQueueDefaultAttributeDialog::SQSQueueDefaultAttributeDialog(const QString &key, const QString &value, QWidget *parent) : QDialog(parent), _ui(new Ui::SQSQueueDefaultAttributeDialog) {

    // Initialization
    Initialize();

    // Save
    _key = key;
    _value = value;

    // Set text fields
    _ui->keyEdit->setText(_key);
    _ui->valueEdit->setText(_value);

    // Diable key field
    _ui->keyEdit->setDisabled(true);
}

SQSQueueDefaultAttributeDialog::~SQSQueueDefaultAttributeDialog() {
    delete _ui;
}

void SQSQueueDefaultAttributeDialog::Initialize() {

    // Setup UI components
    _ui->setupUi(this);

    // Connect OK/CANCEL buttons
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &SQSQueueDefaultAttributeDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &SQSQueueDefaultAttributeDialog::HandleReject);

    // Connect text fields
    connect(_ui->keyEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        _key = text;
    });
    connect(_ui->valueEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        _value = text;
    });
}

void SQSQueueDefaultAttributeDialog::HandleAccept() {
    if (_key.isEmpty()) {
        QMessageBox::warning(nullptr, "Error", "Key cannot be empty!");
        return;
    }
    if (_value.isEmpty()) {
        QMessageBox::warning(nullptr, "Error", "Value cannot be empty!");
        return;
    }
    accept();
}

void SQSQueueDefaultAttributeDialog::HandleReject() {
    accept();
}
