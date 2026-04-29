//
// Created by jensv on 19/02/2026.
//

#include <modules/sns/SNSTopicDefaultAttributeDialog.h>
#include "ui_SNSTopicDefaultAttributeDialog.h"

SNSTopicDefaultAttributeDialog::SNSTopicDefaultAttributeDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::SNSTopicDefaultAttributeDialog) {

    // Initialization
    Initialize();
}

SNSTopicDefaultAttributeDialog::SNSTopicDefaultAttributeDialog(const QString &key, const QString &value, QWidget *parent) : QDialog(parent), _ui(new Ui::SNSTopicDefaultAttributeDialog) {

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

SNSTopicDefaultAttributeDialog::~SNSTopicDefaultAttributeDialog() {
    delete _ui;
}

void SNSTopicDefaultAttributeDialog::Initialize() {

    // Setup UI components
    _ui->setupUi(this);

    // Connect OK/CANCEL buttons
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &SNSTopicDefaultAttributeDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &SNSTopicDefaultAttributeDialog::HandleReject);

    // Connect text fields
    connect(_ui->keyEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        _key = text;
    });
    connect(_ui->valueEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        _value = text;
    });
}

void SNSTopicDefaultAttributeDialog::HandleAccept() {
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

void SNSTopicDefaultAttributeDialog::HandleReject() {
    accept();
}
