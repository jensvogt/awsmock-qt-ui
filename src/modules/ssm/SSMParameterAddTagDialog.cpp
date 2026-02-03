//
// Created by vogje01 on 2/2/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SSMParameterAddTagDialog.h" resolved

#include <modules/ssm/SSMParameterAddTagDialog.h>
#include "ui_SSMParameterAddTagDialog.h"

SSMParameterAddTagDialog::SSMParameterAddTagDialog(QWidget *parent) : BaseDialog(parent), _ui(new Ui::SSMParameterAddTagDialog) {

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &SSMParameterAddTagDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &SSMParameterAddTagDialog::HandleReject);

    // Key value fields
    connect(_ui->keyEdit, &QLineEdit::textChanged, this, [this]() {
        _changed = true;
        _key = _ui->keyEdit->text();
    });
    connect(_ui->valueEdit, &QLineEdit::textChanged, this, [this]() {
        _changed = true;
        _value = _ui->valueEdit->text();
    });
}

SSMParameterAddTagDialog::~SSMParameterAddTagDialog() {
    delete _ui;
}

void SSMParameterAddTagDialog::HandleAccept() {
    if (_changed) {
        // Update parameter
    }
    accept();
}

void SSMParameterAddTagDialog::HandleReject() {
    accept();
}
