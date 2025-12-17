//
// Created by vogje01 on 11/27/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_S3ObjectMetadataDetailsDialog.h" resolved

#include <modules/s3/S3ObjectMetadataDialog.h>
#include "ui_S3ObjectMetadataDialog.h"


S3ObjectMetadataDialog::S3ObjectMetadataDialog(QWidget *parent, bool isAdded) : QDialog(parent), _ui(new Ui::S3ObjectMetadataDialog) {
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &S3ObjectMetadataDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &S3ObjectMetadataDialog::HandleReject);

}

S3ObjectMetadataDialog::~S3ObjectMetadataDialog() {
    delete _ui;
}

void S3ObjectMetadataDialog::HandleAccept() {
    accept();
}


void S3ObjectMetadataDialog::HandleReject() {
    accept();
}

QString S3ObjectMetadataDialog::GetKey() const {
    return _ui->nameEdit->text();
}

QString S3ObjectMetadataDialog::GetValue() const {
    return _ui->valueEdit->text();
}
