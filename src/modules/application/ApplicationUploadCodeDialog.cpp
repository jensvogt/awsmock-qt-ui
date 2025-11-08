//
// Created by vogje01 on 11/8/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_UploadApplicationCodeDialog.h" resolved

#include <modules/application/ApplicationUploadCodeDialog.h>
#include "ui_ApplicationUploadCodeDialog.h"


ApplicationUploadCodeDialog::ApplicationUploadCodeDialog(const QString &name, QWidget *parent) : QDialog(parent), ui(new Ui::ApplicationUploadCodeDialog) {

    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &ApplicationUploadCodeDialog::HandleAccept);
}

ApplicationUploadCodeDialog::~ApplicationUploadCodeDialog() {
    delete ui;
}


void ApplicationUploadCodeDialog::HandleAccept() {
    accept();
}


void ApplicationUploadCodeDialog::HandleReject() {
    accept();
}
