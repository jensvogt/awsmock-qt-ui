//
// Created by vogje01 on 11/8/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_UploadApplicationCodeDialog.h" resolved

#include <modules/application/UploadApplicationCodeDialog.h>
#include "ui_UploadApplicationCodeDialog.h"


UploadApplicationCodeDialog::UploadApplicationCodeDialog(const QString name, QWidget *parent) : QDialog(parent), ui(new Ui::UploadApplicationCodeDialog) {
    ui->setupUi(this);
}

UploadApplicationCodeDialog::~UploadApplicationCodeDialog() {
    delete ui;
}