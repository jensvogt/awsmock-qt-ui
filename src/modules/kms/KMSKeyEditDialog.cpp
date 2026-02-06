//
// Created by vogje01 on 2/6/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_KMSKeyEditDIalog.h" resolved

#include <modules/kms/KMSKeyEditDialog.h>
#include "ui_KMSKeyEditDialog.h"


KMSKeyEditDialog::KMSKeyEditDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::KMSKeyEditDialog) {
    _ui->setupUi(this);
}

KMSKeyEditDialog::~KMSKeyEditDialog() {
    delete _ui;
}
