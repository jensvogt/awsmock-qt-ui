//
// Created by vogje01 on 1/17/26.
//

#include <modules/module/ShowInfrastructure.h>
#include "ui_ShowInfrastructure.h"

ShowInfrastructure::ShowInfrastructure(QWidget *parent) : ::BaseDialog(parent), _ui(new Ui::ShowInfrastructure) {

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
    connect(_ui->prettyPrintButton, &QPushButton::toggled, this, &ShowInfrastructure::PrettyPrintClicked);

    // Save locally
    _ui->importButton->setText(nullptr);
    _ui->importButton->setIcon(IconUtils::GetIcon("import"));
    connect(_ui->importButton, &QPushButton::clicked, this, &ShowInfrastructure::ImportData);

    // Save locally
    _ui->saveButton->setText(nullptr);
    _ui->saveButton->setIcon(IconUtils::GetIcon("save"));
    connect(_ui->saveButton, &QPushButton::clicked, this, &ShowInfrastructure::SaveData);

    // Search text field
    connect(_ui->searchEdit, &QLineEdit::textChanged, this, &ShowInfrastructure::FindNext);

    // Clear search button
    _ui->clearSearchButton->setText(nullptr);
    _ui->clearSearchButton->setIcon(IconUtils::GetIcon("clear"));
    connect(_ui->clearSearchButton, &QPushButton::clicked, this, &ShowInfrastructure::ClearSearch);

    // Find next
    const auto nextShortcut = new QShortcut(QKeySequence(Qt::Key_F3), this);
    connect(nextShortcut, &QShortcut::activated, this, &ShowInfrastructure::FindNext);

    // Find previous
    const auto previousShortcut = new QShortcut(QKeySequence(Qt::SHIFT | Qt::Key_F3), this);
    connect(previousShortcut, &QShortcut::activated, this, &ShowInfrastructure::FindPrevious);

    // Get the infrastructure JSON from the server
    _moduleService->GetInfrastructure();
}

ShowInfrastructure::~ShowInfrastructure() {
    delete _ui;
}

void ShowInfrastructure::HandleGetInfrastructure(const QString &infrastructureJson) const {
    _ui->infrastructureText->setPlainText(infrastructureJson);
    if (_ui->prettyPrintButton->isChecked()) {
        PrettyPrintClicked(true);
    }
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

    _ui->infrastructureText->setPlainText(QString(jsonData));
}

void ShowInfrastructure::ImportData() const {
    _moduleService->ImportInfrastructure(_ui->infrastructureText->toPlainText());
    connect(_moduleService, &ModuleService::ImportResponseSignal, this, []() {
        QMessageBox::information(nullptr, "Info", "JSON file imported");
    });
}

void ShowInfrastructure::SaveData() {

    // Create a QFileDialog set to select existing files
    const auto filter = "JSON Files (*.json);All Files (*.*)";
    const auto defaultDir = Configuration::instance().GetValue<QString>("ui.default-directory.ImportInfrastructure", "/usr/local/awsmock-qt-ui");

    if (const QString filePath = QFileDialog::getOpenFileName(nullptr, "Open JSON Configuration File", defaultDir, filter); !filePath.isEmpty()) {

        // Get file
        _currentFile = new QFile(filePath);
        Configuration::instance().SetValue<QString>("ui.default-directory.ImportInfrastructure", QFileInfo(filePath).absolutePath());

        // Open file
        if (!_currentFile->open(QIODevice::ReadWrite)) {
            QMessageBox::critical(nullptr, "Error", "Could not open file: " + _currentFile->fileName());
            return;
        }

        // Save JSON data
        const QByteArray jsonData = _ui->infrastructureText->toPlainText().toUtf8();
        _currentFile->write(jsonData);
        _currentFile->close();
        QMessageBox::information(nullptr, "Information", "Infrastructure saved, file: " + _currentFile->fileName());
    }

}

void ShowInfrastructure::FindNext() const {
    const QString searchText = _ui->searchEdit->text();
    if (_ui->infrastructureText->find(searchText))
        return;

    // Wrap: restart from beginning
    QTextCursor cursor = _ui->infrastructureText->textCursor();
    cursor.movePosition(QTextCursor::Start);
    _ui->infrastructureText->setTextCursor(cursor);

    _ui->infrastructureText->find(searchText);
}

void ShowInfrastructure::FindPrevious() const {
    const QString searchText = _ui->searchEdit->text();
    _ui->infrastructureText->find(searchText, QTextDocument::FindBackward);
}

void ShowInfrastructure::ClearSearch() const {
    _ui->searchEdit->clear();
}

void ShowInfrastructure::PrettyPrintClicked(const bool checked) const {
    if (checked) {
        const QByteArray body = _ui->infrastructureText->toPlainText().toUtf8();
        QJsonParseError error;
        const QJsonDocument jDoc = QJsonDocument::fromJson(body, &error);
        if (error.error == QJsonParseError::NoError) {
            _ui->infrastructureText->clear();
            _ui->infrastructureText->setPlainText(jDoc.toJson(QJsonDocument::Indented));
        } else {
            QMessageBox::warning(nullptr, "Warning", "Invalid file, error: " + error.errorString());
        }
    } else {
        const QByteArray body = _ui->infrastructureText->toPlainText().toUtf8();
        QJsonParseError error;
        const QJsonDocument jDoc = QJsonDocument::fromJson(body, &error);
        if (error.error == QJsonParseError::NoError) {
            _ui->infrastructureText->clear();
            _ui->infrastructureText->setPlainText(jDoc.toJson(QJsonDocument::Compact));
        } else {
            QMessageBox::warning(nullptr, "Warning", "Invalid file, error: " + error.errorString());
        }
    }
    if (!_ui->searchEdit->text().isEmpty()) {
        const QString text = _ui->searchEdit->text();
        QTextCursor cursor(_ui->infrastructureText->document());
        cursor.movePosition(QTextCursor::Start);
        _ui->infrastructureText->setTextCursor(cursor);
        _ui->infrastructureText->find(text, QTextDocument::FindCaseSensitively);
    }
}
