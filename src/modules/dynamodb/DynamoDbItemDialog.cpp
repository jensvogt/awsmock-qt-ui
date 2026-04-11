//
// Created by vogje01 on 4/4/26.
//

#include <modules/dynamodb/DynamoDbItemDialog.h>

#include <utility>
#include "ui_DynamoDbItemDialog.h"

DynamoDbItemDialog::DynamoDbItemDialog(QString tableName, QString partitionKey, QString sortKey, QWidget *parent) : BaseDialog(parent), _ui(new Ui::DynamoDbItemDialog),
                                                                                                                    _tableName(std::move((tableName))), _partitionKey(std::move(partitionKey)), _sortKey(std::move(sortKey)) {

    // Service
    _dynamoDbService = new DynamoDbService();

    // Setup UI
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &DynamoDbItemDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &DynamoDbItemDialog::HandleReject);

    // Refresh button
    _ui->refreshButton->setText(nullptr);
    _ui->refreshButton->setIcon(IconUtils::GetIcon("refresh"));
    connect(_ui->refreshButton, &QPushButton::clicked, this, &DynamoDbItemDialog::LoadContent);

    // Connect update method
    connect(_dynamoDbService, &DynamoDbService::GetItemSignal, this, &DynamoDbItemDialog::UpdateItem);

    // Load data
    DynamoDbItemDialog::LoadContent();

    // Text editor
    _ui->attributeEdit->SetReadOnly(true);
}

void DynamoDbItemDialog::LoadContent() {
    _dynamoDbService->GetItem(_tableName, _partitionKey, _sortKey);
}

void DynamoDbItemDialog::UpdateItem(const DynamoDbGetItemResponse &response) const {

    logDebug << "Loading DynamodbItem, tableName: " << _tableName << ", partitionKey: " << _partitionKey << ", sortKey: " << _sortKey;
    _ui->regionEdit->setText(response.region);
    _ui->tablenameEdit->setText(response.tableName);
    _ui->userEdit->setText(response.user);
    _ui->partitionKeyEdit->setText(response.partitionKey);
    _ui->sortKeyEdit->setText(response.sortKey);
    _ui->createdEdit->setText(response.created.toString());
    _ui->modifiedEdit->setText(response.modified.toString());
    _ui->attributeEdit->SetText(response.attributes);
}

DynamoDbItemDialog::~DynamoDbItemDialog() {
    delete _ui;
}

void DynamoDbItemDialog::HandleAccept() {
    accept();
}

void DynamoDbItemDialog::HandleReject() {
    accept();
}
