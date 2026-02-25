//
// Created by vogje01 on 1/17/26.
//

#include <modules/module/ShowInfrastructure.h>
#include "ui_ShowInfrastructure.h"

ShowInfrastructure::ShowInfrastructure(QWidget *parent) : BaseDialog(parent), _ui(new Ui::ShowInfrastructure) {

    _moduleService = new ModuleService();
    connect(_moduleService, &ModuleService::GetInfrastructureSignal, this, &ShowInfrastructure::HandleGetInfrastructure);

    // Setup UI
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &ShowInfrastructure::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &ShowInfrastructure::HandleReject);

    // Search file
    _ui->searchFileButton->setText(nullptr);
    _ui->searchFileButton->setIcon(IconUtils::GetIcon("search"));
    connect(_ui->searchFileButton, &QPushButton::clicked, this, &ShowInfrastructure::SearchFile);

    // Refresh
    _ui->refreshButton->setText(nullptr);
    _ui->refreshButton->setIcon(IconUtils::GetIcon("refresh"));
    connect(_ui->refreshButton, &QPushButton::clicked, this, [this]() {
        _moduleService->GetInfrastructure();
    });

    // Pretty print
    _ui->prettyPrintButton->setText(nullptr);
    _ui->prettyPrintButton->setIcon(IconUtils::GetIcon("pretty"));
    _ui->prettyPrintButton->toggle();
    connect(_ui->prettyPrintButton, &QPushButton::toggled, this, [this](bool checked) {
        _ui->plainTextEditor->SetPrettyPrint(checked);
    });

    // Save locally
    _ui->importButton->setText(nullptr);
    _ui->importButton->setIcon(IconUtils::GetIcon("import"));
    connect(_ui->importButton, &QPushButton::clicked, this, &ShowInfrastructure::ImportData);

    // Save locally
    _ui->saveButton->setText(nullptr);
    _ui->saveButton->setIcon(IconUtils::GetIcon("save"));
    connect(_ui->saveButton, &QPushButton::clicked, this, &ShowInfrastructure::SaveData);

    // Get the infrastructure JSON from the server
    _moduleService->GetInfrastructure();
}

ShowInfrastructure::~ShowInfrastructure() {
    delete _ui;
}

void ShowInfrastructure::HandleGetInfrastructure(const QString &infrastructureJson) const {
    _ui->plainTextEditor->SetText(infrastructureJson);
    _ui->statusLabel->setText("Last update: " + DateTimeUtils::GetLogTimeFormat(QDateTime::currentDateTime()));
}

void ShowInfrastructure::HandleAccept() {
    accept();
}

void ShowInfrastructure::HandleReject() {
    accept();
}

void ShowInfrastructure::SearchFile() {
    // Create a QFileDialog set to select existing files
    const auto filter = "JSON Files (*.json);All Files (*.*)";
    const auto defaultDir = Configuration::instance().GetValue<QString>("ui.default-directory.ImportInfrastructure", "/usr/local/awsmock-qt-ui");

    if (const QString filePath = QFileDialog::getOpenFileName(nullptr, "Open JSON Configuration File", defaultDir, filter); !filePath.isEmpty()) {
        _currentFile = new QFile(filePath);
        ReadData();
        Configuration::instance().SetValue<QString>("ui.default-directory.ImportInfrastructure", QFileInfo(filePath).absolutePath());
    }
}

void ShowInfrastructure::ReadData() const {

    if (!_currentFile->open(QIODevice::ReadOnly)) {
        QMessageBox::critical(nullptr, "Error", "Could not open file: " + _currentFile->fileName());
        return;
    }

    const QByteArray jsonData = _currentFile->readAll();
    _currentFile->close();

    _ui->plainTextEditor->SetText(QString(jsonData));
}

void ShowInfrastructure::ImportData() const {
    _moduleService->ImportInfrastructure(_ui->plainTextEditor->GetText());
    connect(_moduleService, &ModuleService::ImportResponseSignal, this, []() {
        QMessageBox::information(nullptr, "Info", "JSON file imported");
    });
}

void ShowInfrastructure::SaveData() {

    // Create a QFileDialog set to select existing files
    const auto filter = "JSON Files (*.json);All Files (*.*)";
    const auto defaultDir = Configuration::instance().GetValue<QString>("ui.default-directory.ImportInfrastructure", "/usr/local/awsmock-qt-ui");

    if (const QString filePath = QFileDialog::getSaveFileName(nullptr, "Open JSON Configuration File", defaultDir, filter); !filePath.isEmpty()) {

        // Get file
        _currentFile = new QFile(filePath);
        Configuration::instance().SetValue<QString>("ui.default-directory.ImportInfrastructure", QFileInfo(filePath).absolutePath());

        // Open file
        if (!_currentFile->open(QIODevice::ReadWrite)) {
            QMessageBox::critical(nullptr, "Error", "Could not open file: " + _currentFile->fileName());
            return;
        }

        // Save JSON data
        const QByteArray jsonData = _ui->plainTextEditor->GetText().toUtf8();
        _currentFile->write(jsonData);
        _currentFile->close();
        QMessageBox::information(nullptr, "Information", "Infrastructure saved, file: " + _currentFile->fileName());
    }
}

