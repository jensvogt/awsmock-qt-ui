//
// Created by vogje01 on 2/15/26.
//

#include <modules/cognito/CognitoAddUserpoolDialog.h>
#include "ui_CognitoAddUserpoolDialog.h"

CognitoAddUserpoolDialog::CognitoAddUserpoolDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::CognitoAddUserpoolDialog) {

    // Setup UI components
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &CognitoAddUserpoolDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &CognitoAddUserpoolDialog::HandleReject);

    // Add name line edit
    connect(_ui->userpoolNameEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        _userpoolName = text;
    });
}

CognitoAddUserpoolDialog::~CognitoAddUserpoolDialog() {
    delete _ui;
}

void CognitoAddUserpoolDialog::HandleAccept() {
    accept();
}

void CognitoAddUserpoolDialog::HandleReject() {
    accept();
}
