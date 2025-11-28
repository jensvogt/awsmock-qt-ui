//
// Created by vogje01 on 11/27/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_S3ObjectMetadataDetailsDialog.h" resolved

#include "../../../include/modules/s3/S3ObjectMetadataDetailsDialog.h"
#include "ui_S3ObjectMetadataDetailsDialog.h"


S3ObjectMetadataDetailsDialog::S3ObjectMetadataDetailsDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::S3ObjectMetadataDetailsDialog) {
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &S3ObjectMetadataDetailsDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &S3ObjectMetadataDetailsDialog::HandleReject);

}

S3ObjectMetadataDetailsDialog::~S3ObjectMetadataDetailsDialog() {
    delete _ui;
}

void S3ObjectMetadataDetailsDialog::HandleAccept() {
    accept();
}


void S3ObjectMetadataDetailsDialog::HandleReject() {
    accept();
}
