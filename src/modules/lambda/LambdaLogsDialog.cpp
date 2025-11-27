//
// Created by vogje01 on 11/27/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LambdaLOgsDialog.h" resolved

#include <modules/lambda/LambdaLogsDialog.h>
#include "ui_LambdaLogsDialog.h"

LambdaLogsDialog::LambdaLogsDialog(const QString &name, const QString &containerId, QWidget *parent) : QDialog(parent), _ui(new Ui::LambdaLogsDialog) {

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &LambdaLogsDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &LambdaLogsDialog::HandleReject);

}

LambdaLogsDialog::~LambdaLogsDialog() {
    delete _ui;
}


void LambdaLogsDialog::HandleAccept() {
    accept();
}

void LambdaLogsDialog::HandleReject() {
    accept();
}
