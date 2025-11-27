//
// Created by vogje01 on 11/27/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LambdaEnvironmentAddDialog.h" resolved

#include "../../../include/modules/lambda/LambdaEnvironmentAddDialog.h"
#include "ui_LambdaEnvironmentAddDialog.h"


LambdaEnvironmentAddDialog::LambdaEnvironmentAddDialog(QWidget *parent) : QDialog(parent), ui(new Ui::LambdaEnvironmentAddDialog) {
    ui->setupUi(this);
}

LambdaEnvironmentAddDialog::~LambdaEnvironmentAddDialog() {
    delete ui;
}
