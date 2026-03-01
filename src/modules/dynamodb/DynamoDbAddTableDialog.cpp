
#include <modules/dynamodb/DynamoDbAddTableDialog.h>
#include "ui_DynamoDbAddTableDialog.h"

DynamoDbAddTableDialog::DynamoDbAddTableDialog(QWidget *parent) : BaseDialog(parent), _ui(new Ui::DynamoDbAddTableDialog) {

    // Connect service
    _dynamoDbService = new DynamoDbService();

    // Setup UI
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &DynamoDbAddTableDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &DynamoDbAddTableDialog::HandleReject);

    // Set defaults
    _ui->regionEdit->setText(Configuration::instance().GetValue<QString>("aws.region", nullptr));
    _ui->readCapacityEdit->setText(QString::number(1));
    _ui->writeCapacityEdit->setText(QString::number(1));

    // Setup tabs
    SetupAttributeTab();
    SetupKeySchemaTab();

    // Default tab
    _ui->tabWidget->setCurrentIndex(0);
}

DynamoDbAddTableDialog::~DynamoDbAddTableDialog() {
    delete _ui;
}

void DynamoDbAddTableDialog::LoadContent() {
}

void DynamoDbAddTableDialog::SetupAttributeTab() {

    // Add button
    _ui->attributeAddButton->setText(nullptr);
    _ui->attributeAddButton->setIcon(IconUtils::GetIcon("add"));
    connect(_ui->attributeAddButton, &QPushButton::clicked, [this]() {

        // Initialize dialog
        DynamoDbAddAttributeDialog dialog;
        dialog.exec();

        // Get results
        const DynamoDbAttribute attribute = dialog.GetAttribute();

        // Update attribute
        _createRequest.attributes.append(attribute);

        // Update table
        const int row = _attributeDataModel->rowCount();
        SetColumn(_attributeDataModel, row, 0, attribute.attributeName);
        SetColumn(_attributeDataModel, row, 1, DynamoDbAttributeTypeToString(attribute.attributeType));
    });

    // Table headers
    const QStringList headers = QStringList() = {tr("Name"), tr("Type")};

    // Table
    _attributeDataModel = new QStandardItemModel(this);
    _attributeDataModel->setHorizontalHeaderLabels(headers);
    _attributeDataModel->setColumnCount(static_cast<int>(headers.count()));

    // Proxy model for prefix filtering
    _attributeProxyModel = new PrefixFilterProxyModel(this);
    _attributeProxyModel->setSourceModel(_attributeDataModel);
    _ui->attributeTable->setModel(_attributeProxyModel);

    _ui->attributeTable->setShowGrid(true);
    _ui->attributeTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->attributeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->attributeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->attributeTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch); // name
    _ui->attributeTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents); //status

    // Enable sorting, default on and sorted ascending by name
    _ui->attributeTable->setSortingEnabled(true);
    _ui->attributeTable->sortByColumn(0, Qt::AscendingOrder);
}

void DynamoDbAddTableDialog::SetupKeySchemaTab() {
    _ui->keySchemaAddButton->setText(nullptr);
    _ui->keySchemaAddButton->setIcon(IconUtils::GetIcon("add"));

    // Add button
    _ui->keySchemaAddButton->setText(nullptr);
    _ui->keySchemaAddButton->setIcon(IconUtils::GetIcon("add"));
    connect(_ui->keySchemaAddButton, &QPushButton::clicked, [this]() {

        // Initialize dialog
        DynamoDbAddKeySchemaDialog dialog;
        dialog.exec();

        // Get results
        const DynamoDbKeySchema keySchema = dialog.GetKeySchema();

        // Update attribute
        _createRequest.keySchema.append(keySchema);

        // Update table
        const int row = _keySchemaDataModel->rowCount();
        SetColumn(_keySchemaDataModel, row, 0, keySchema.attributeName);
        SetColumn(_keySchemaDataModel, row, 1, DynamoDbKeyTypeToString(keySchema.keyType));
    });

    // Key schema table headers
    const QStringList headers = QStringList() = {tr("Name"), tr("Type")};

    // Table
    _keySchemaDataModel = new QStandardItemModel(this);
    _keySchemaDataModel->setHorizontalHeaderLabels(headers);
    _keySchemaDataModel->setColumnCount(static_cast<int>(headers.count()));

    // Proxy model for prefix filtering
    _keySchemaProxyModel = new PrefixFilterProxyModel(this);
    _keySchemaProxyModel->setSourceModel(_keySchemaDataModel);
    _ui->keySchemaTable->setModel(_keySchemaProxyModel);

    _ui->keySchemaTable->setShowGrid(true);
    _ui->keySchemaTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->keySchemaTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->keySchemaTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->keySchemaTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents); // name
    _ui->keySchemaTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch); //status

    // Enable sorting, default on and sorted ascending by name
    _ui->keySchemaTable->setSortingEnabled(true);
    _ui->keySchemaTable->sortByColumn(0, Qt::AscendingOrder);
}

void DynamoDbAddTableDialog::HandleAccept() {
    _createRequest.tableName = _ui->nameEdit->text();
    _createRequest.region = _ui->regionEdit->text();
    _createRequest.provisionedThroughput.readCapacity = _ui->readCapacityEdit->text().toInt();
    _createRequest.provisionedThroughput.writeCapacity = _ui->writeCapacityEdit->text().toInt();
    accept();
}


void DynamoDbAddTableDialog::HandleReject() {
    accept();
}
