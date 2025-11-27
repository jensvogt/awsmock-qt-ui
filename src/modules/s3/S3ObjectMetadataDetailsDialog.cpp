//
// Created by vogje01 on 11/27/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_S3ObjectMetadataDetailsDialog.h" resolved

#include "../../../include/modules/s3/S3ObjectMetadataDetailsDialog.h"
#include "ui_S3ObjectMetadataDetailsDialog.h"


S3ObjectMetadataDetailsDialog::S3ObjectMetadataDetailsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::S3ObjectMetadataDetailsDialog) {
    ui->setupUi(this);
}

S3ObjectMetadataDetailsDialog::~S3ObjectMetadataDetailsDialog() {
    delete ui;
}
