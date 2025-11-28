//
// Created by vogje01 on 11/28/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LambdaResultListDialog.h" resolved

#include <modules/lambda/LambdaResultListDialog.h>
#include "ui_LambdaResultListDialog.h"

LambdaResultListDialog::LambdaResultListDialog(const QString &lambdaArn, QWidget *parent) : BaseDialog(parent), _ui(new Ui::LambdaResultListDialog), _lambdaArn(lambdaArn) {

    // Lambda REST service
    _lambdaService = new LambdaService();

    // Setup UI components
    _ui->setupUi(this);
}

LambdaResultListDialog::~LambdaResultListDialog() {
    delete _ui;
}
