//
// Created by jensv on 18/02/2026.
//

#include <modules/module/ModuleExportDialog.h>
#include "ui_ModuleExportDialog.h"

QStringList ModuleExportDialog::_excluded = {"gateway", "monitoring"};

ModuleExportDialog::ModuleExportDialog(QWidget *parent) : BaseDialog(parent), _ui(new Ui::ModuleExportDialog) {

    // Modules service
    _moduleService = new ModuleService();

    // Setup IU components
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &ModuleExportDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &ModuleExportDialog::HandleReject);

    // Setup filename
    _ui->filenameEdit->setText(Configuration::instance().GetValue<QString>("ui.default-file.ExportInfrastructure"));
    _ui->browseButton->setText(nullptr);
    _ui->browseButton->setIcon(IconUtils::GetIcon("browse"));
    connect(_ui->browseButton, &QPushButton::clicked, this, &ModuleExportDialog::HandleBrowseButton);

    // Defaults
    _exportFilePath = Configuration::instance().GetValue<QString>("ui.default-file.ExportInfrastructure");

    // Initial load
    ModuleExportDialog::LoadContent();
}

ModuleExportDialog::~ModuleExportDialog() {
    delete _moduleService;
    delete _ui;
}

void ModuleExportDialog::LoadContent() {
    _moduleService->ListModuleNames();
}

void ModuleExportDialog::LoadAvailableModules(const ListModuleNamesResponse &response) const {
    _leftDataModel->removeRows(0, _leftDataModel->rowCount());
    for (auto r = 0; r < response.moduleNames.count(); r++) {
        if (!_excluded.contains(response.moduleNames.at(r))) {
            _leftDataModel->appendRow(new QStandardItem(response.moduleNames.at(r)));
        }
    }
}

void ModuleExportDialog::HandleBrowseButton() {

    // Create a QFileDialog set to select existing files
    const auto filter = "JSON Files (*.json);;All Files (*.*)";
    const auto defaultDir = Configuration::instance().GetValue<QString>("ui.default-directory.ExportInfrastructure", "/usr/local/awsmock-qt-ui");

    if (const QString filePath = QFileDialog::getSaveFileName(nullptr, "Open JSON Configuration File", defaultDir, filter); !filePath.isEmpty()) {
        _exportFilePath = filePath.toUtf8();
        _ui->filenameEdit->setText(filePath);
        Configuration::instance().SetValue<QString>("ui.default-file.ExportInfrastructure", _exportFilePath);
    }
}

void ModuleExportDialog::WriteInfrastructureFile(const QString &infrastructure) const {

    QString output = infrastructure;
    // Pretty print
    if (_prettyPrint) {
        output = JsonUtils::PrettyPrint(infrastructure);
    }

    // Save JSON data
    QFile file(_exportFilePath);
    file.write(output.toUtf8());
    file.close();
}

void ModuleExportDialog::HandleAccept() {

    // Get all export modules
    _selectedModules = _ui->selectModules->GetModules();
    if (_selectedModules.isEmpty()) {
        QMessageBox::warning(nullptr, "Error", "Selected module list cannot be empty!");
        return;
    }
    if (_exportFilePath.isEmpty()) {
        QMessageBox::warning(nullptr, "Error", "Output file cannot be empty!");
        return;
    }
    accept();
}

QString ModuleExportDialog::GetFilePath() const {
    return _exportFilePath;
}

QStringList ModuleExportDialog::GetModules() const {
    return _selectedModules;
}

ExportType ModuleExportDialog::GetExportType() const {
    return _ui->selectModules->GetExportType();
}

void ModuleExportDialog::HandleReject() {
    reject();
}
