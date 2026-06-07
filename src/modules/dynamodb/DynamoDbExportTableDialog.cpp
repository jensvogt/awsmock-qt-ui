//
// Created by vogje01 on 3/28/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DynamoDbExportTableDialog.h" resolved

#include <modules/dynamodb/DynamoDbExportTableDialog.h>
#include "ui_DynamoDbExportTableDialog.h"

DynamoDbExportTableDialog::DynamoDbExportTableDialog(const QString &tableName, QWidget *parent) : QDialog(parent), _ui(new Ui::DynamoDbExportTableDialog), _tableName(tableName) {

    // Connect service
    _dynamodbService = new DynamoDbService();
    connect(_dynamodbService, &DynamoDbService::ExportItemsSignal, this, &DynamoDbExportTableDialog::HandleExportItems);

    // Setup UI
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &DynamoDbExportTableDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &DynamoDbExportTableDialog::HandleReject);

    // Table name
    _ui->tableNameEdit->setText(tableName);

    // Browse button
    _ui->browseButton->setText(nullptr);
    _ui->browseButton->setIcon(IconUtils::GetIcon("search"));
    connect(_ui->browseButton, &QPushButton::clicked, this, &DynamoDbExportTableDialog::SelectFilename);

    // Load item data
    LoadItemData();
}

DynamoDbExportTableDialog::~DynamoDbExportTableDialog() {
    delete _ui;
}

void DynamoDbExportTableDialog::HandleAccept() {
    if (_ui->fileNameEdit->text().isEmpty()) {
        logError << "File name cannot be empty!";
        QMessageBox::critical(nullptr, "Error", "File name cannot be empty!");
    } else if (_ui->tableNameEdit->text().isEmpty()) {
        logError << "Table name cannot be empty!";
        QMessageBox::critical(nullptr, "Error", "Table name cannot be empty!");
    } else {
        if (QFile file(_file); file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            const long count = file.write(_ui->itemDataEdit->GetText().toUtf8());
            logInfo << "Tabel items exported, tableName: " << _tableName << ", file: " << _file << ", fileSize: " << count;
            file.close();
            accept();
        } else {
            logError << "Could not open file for writing:" << _file;
            QMessageBox::critical(nullptr, "Error", "Could not open file for writing:" + _file);
        }
    }
}

void DynamoDbExportTableDialog::HandleReject() {
    reject();
}

void DynamoDbExportTableDialog::LoadItemData() const {
    _dynamodbService->ExportItems(_tableName);
}

void DynamoDbExportTableDialog::HandleExportItems(const QString &exportItems) const {
    _ui->itemDataEdit->SetText(exportItems);
}

void DynamoDbExportTableDialog::SelectFilename() {

    // Create a QFileDialog set to select existing files
    const auto filter = "JSON Files (*.json);All Files (*.*)";
    const auto defaultDir = Configuration::instance().GetValue<QString>("ui.default-directory.ExportDynamoDbItems", "/usr/local/awsmock-qt-ui");

    if (const QString filePath = QFileDialog::getSaveFileName(nullptr, "Open JSON Configuration File", defaultDir, filter); !filePath.isEmpty()) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadWrite)) {
            QMessageBox::critical(nullptr, "Error", "Could not open file:" + filePath);
            return;
        }
        file.close();
        _file = filePath;
        _ui->fileNameEdit->setText(file.fileName());
        Configuration::instance().SetValue<QString>("ui.default-directory.ExportDynamoDbItems", QFileInfo(filePath).absolutePath());
    }
}
