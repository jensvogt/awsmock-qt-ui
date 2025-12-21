
#include <modules/dynamodb/DynamoDbAddKeySchemaDialog.h>
#include "ui_DynamoDbAddKeySchemaDialog.h"

DynamoDbAddKeySchemaDialog::DynamoDbAddKeySchemaDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::DynamoDbAddKeySchemaDialog) {

    // Setup UI
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &DynamoDbAddKeySchemaDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &DynamoDbAddKeySchemaDialog::HandleReject);

    // Attribute name
    connect(_ui->nameEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        _keySchema.attributeName = text;
    });

    // Attribute types combo box
    _keySchema.keyType = HASH;
    _ui->typeCombo->addItems(DynamoDbKeyTypeStringList());
    connect(_ui->typeCombo, &QComboBox::currentIndexChanged, this, [this](const int index) {
        _keySchema.keyType = DynamoDbKeySchemaTypeFromIndex(index);
    });
}

DynamoDbAddKeySchemaDialog::~DynamoDbAddKeySchemaDialog() {
    delete _ui;
}

void DynamoDbAddKeySchemaDialog::HandleAccept() {
    accept();
}

void DynamoDbAddKeySchemaDialog::HandleReject() {
    accept();
}
