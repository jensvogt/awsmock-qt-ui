//
// Created by vogje01 on 2/20/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ReplaceWordDialog.h" resolved

#include <components/ReplaceWordDialog.h>
#include "ui_ReplaceWordDialog.h"


ReplaceWordDialog::ReplaceWordDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::ReplaceWordDialog) {

    // Setup UI components
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &ReplaceWordDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &ReplaceWordDialog::HandleReject);

    // Connect text fields
    connect(_ui->searchEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        _searchText = text;
    });
    connect(_ui->replacementEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        _replacementText = text;
    });
}

ReplaceWordDialog::~ReplaceWordDialog() {
    delete _ui;
}

void ReplaceWordDialog::HandleAccept() {
    accept();
}

void ReplaceWordDialog::HandleReject() {
    reject();
}
