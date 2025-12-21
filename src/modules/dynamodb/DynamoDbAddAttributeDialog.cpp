
#include <modules/dynamodb/DynamoDbAddAttributeDialog.h>
#include "ui_DynamoDbAddAttributeDialog.h"


DynamoDbAddAttributeDialog::DynamoDbAddAttributeDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::DynamoDbAddAttributeDialog) {

    // Setup UI
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &DynamoDbAddAttributeDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &DynamoDbAddAttributeDialog::HandleReject);
}

DynamoDbAddAttributeDialog::~DynamoDbAddAttributeDialog() {
    delete _ui;
}

void DynamoDbAddAttributeDialog::HandleAccept() {
    accept();
}


void DynamoDbAddAttributeDialog::HandleReject() {
    accept();
}
