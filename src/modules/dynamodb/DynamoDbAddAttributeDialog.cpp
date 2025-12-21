
#include <modules/dynamodb/DynamoDbAddAttributeDialog.h>
#include "ui_DynamoDbAddAttributeDialog.h"

DynamoDbAddAttributeDialog::DynamoDbAddAttributeDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::DynamoDbAddAttributeDialog) {

    // Setup UI
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &DynamoDbAddAttributeDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &DynamoDbAddAttributeDialog::HandleReject);

    // Attribute name
    connect(_ui->nameEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        _attribute.attributeName = text;
    });

    // Attribute types combo box
    _attribute.attributeType = STRING_ATTRIBUTE_TYPE;
    _ui->typeCombo->addItems(DynamoDbAttributeTypeStringList());
    connect(_ui->typeCombo, &QComboBox::currentIndexChanged, this, [this](const int index) {
        _attribute.attributeType = DynamoDbAttributeTypeFromIndex(index);
    });
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
