//
// Created by vogje01 on 1/17/26.
//

#include <utils/ShowInfrastructure.h>
#include "ui_ShowInfrastructure.h"

ShowInfrastructure::ShowInfrastructure(QWidget *parent) : ::BaseDialog(parent), _ui(new Ui::ShowInfrastructure) {

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
    connect(_ui->refreshButton, &QPushButton::clicked, this, &ShowInfrastructure::ReadData);

    // Save
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
}

ShowInfrastructure::~ShowInfrastructure() {
    delete _ui;
}

void ShowInfrastructure::HandleAccept() {
    accept();
}

void ShowInfrastructure::HandleReject() {
    accept();
}

void ShowInfrastructure::SearchFile() {
    // Create a QFileDialog set to select existing files
    const auto filter = "JSON Files (*.json);;All Files (*.*)";
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

void ShowInfrastructure::SaveData() const {

    if (_ui->infrastructureText->document()->isModified()) {

        if (!_currentFile->open(QIODevice::ReadWrite)) {
            QMessageBox::critical(nullptr, "Error", "Could not open file: " + _currentFile->fileName());
            return;
        }

        const QByteArray jsonData = _ui->infrastructureText->toPlainText().toUtf8();
        _currentFile->write(jsonData);
        qDebug() << jsonData;
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
