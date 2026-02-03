//
// Created by vogje01 on 2/2/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SSMParameterAddTagDialog.h" resolved

#include <modules/ssm/SSMParameterTagDialog.h>
#include "ui_SSMParameterTagDialog.h"

SSMParameterTagDialog::SSMParameterTagDialog(QWidget *parent) : BaseDialog(parent), _ui(new Ui::SSMParameterTagDialog) {

    // Initialize
    Initialize();

    // Set window title
    setWindowTitle("Add SSM Parameter Tag");

    // Key/value fields
    connect(_ui->keyEdit, &QLineEdit::textChanged, this, [this]() {
        _changed = true;
        _key = _ui->keyEdit->text();
    });
    connect(_ui->valueEdit, &QLineEdit::textChanged, this, [this]() {
        _changed = true;
        _value = _ui->valueEdit->text();
    });
}

SSMParameterTagDialog::SSMParameterTagDialog(const QString &key, const QString &value, QWidget *parent) : BaseDialog(parent), _ui(new Ui::SSMParameterTagDialog) {
    // Initialize
    Initialize();

    // Set window title
    setWindowTitle("Edit SSM Parameter Tag");

    // Save key
    _key = key;

    // Key/value fields
    _ui->keyEdit->setText(key);
    _ui->keyEdit->setDisabled(true);
    _ui->valueEdit->setText(value);
    connect(_ui->valueEdit, &QLineEdit::textChanged, this, [this]() {
        _changed = true;
        _value = _ui->valueEdit->text();
    });
}

void SSMParameterTagDialog::Initialize() {
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &SSMParameterTagDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &SSMParameterTagDialog::HandleReject);
}

SSMParameterTagDialog::~SSMParameterTagDialog() {
    delete _ui;
}

void SSMParameterTagDialog::HandleAccept() {
    accept();
}

void SSMParameterTagDialog::HandleReject() {
    accept();
}
