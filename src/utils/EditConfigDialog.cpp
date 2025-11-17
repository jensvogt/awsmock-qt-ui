//
// Created by vogje01 on 11/17/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_EditConfigDialog.h" resolved

#include "../../include/utils/EditConfigDialog.h"
#include "ui_EditConfigDialog.h"
#include "utils/Configuration.h"


EditConfigDialog::EditConfigDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::EditConfigDialog) {

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &EditConfigDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &EditConfigDialog::HandleReject);

    // Client settings
    _ui->regionEdit->setText(Configuration::instance().GetRegion());
    _ui->baseUrlEdit->setText(Configuration::instance().GetBaseUrl());

    // FTP settings
    _ui->ftpUserEdit->setText(Configuration::instance().GetDefaultFtpUser());
    _ui->ftpPasswordEdit->setText(Configuration::instance().GetDefaultFtpPassword());

    // Default tab
    _ui->tabWidget->setCurrentIndex(0);
}

EditConfigDialog::~EditConfigDialog() {
    delete _ui;
}

void EditConfigDialog::HandleAccept() {
    accept();
}

void EditConfigDialog::HandleReject() {
    accept();
}
