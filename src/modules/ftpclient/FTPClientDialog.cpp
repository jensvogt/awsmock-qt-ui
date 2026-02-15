//
// Created by vogje01 on 11/8/25.
//

#include <modules/ftpclient/FTPClientDialog.h>
#include "ui_FTPClientDialog.h"

FTPClientDialog::FTPClientDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::FTPClientDialog) {

    // Get local base dir
    _localBaseDir = Configuration::instance().GetValue<QString>("ftp-client.local-base-dir");

    // Setup UI
    _ui->setupUi(this);

    // Logging panel
    SetupLogPanel();

    // Connect button box
    _ui->buttonBox->button(QDialogButtonBox::Close)->setText("Close");
    _ui->buttonBox->button(QDialogButtonBox::Close)->setIcon(IconUtils::GetIcon("exit"));
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &FTPClientDialog::HandleReject);

    // Local folder tree
    _localFolderTree = new LocalFileTree(_localBaseDir, this);
    _ui->horizontalSplitter1->addWidget(_localFolderTree);

    // FTP folder tree
    _ftpFolderTree = new FTPFileTree(this);
    _ui->horizontalSplitter1->addWidget(_ftpFolderTree);

    // Name validator
    const NotEmptyValidator *nameValidator = new NotEmptyValidator(this);
    _ui->serverEdit->setValidator(nameValidator);
    _ui->serverEdit->setPlaceholderText("Server hostname");
    connect(_ui->serverEdit, &QLineEdit::textChanged, this, &FTPClientDialog::UpdateLineEditStyle);

    // Port validator
    const auto portValidator = new QIntValidator(1, 65536, this);
    _ui->portEdit->setValidator(portValidator);
    _ui->portEdit->setPlaceholderText("FTP port (1 - 65536)");
    connect(_ui->portEdit, &QLineEdit::textChanged, this, &FTPClientDialog::UpdateLineEditStyle);

    // User validator
    const NotEmptyValidator *userValidator = new NotEmptyValidator(this);
    _ui->userEdit->setValidator(userValidator);
    connect(_ui->userEdit, &QLineEdit::textChanged, this, &FTPClientDialog::UpdateLineEditStyle);

    // Password validator
    const NotEmptyValidator *passwordValidator = new NotEmptyValidator(this);
    _ui->passwordEdit->setValidator(passwordValidator);
    connect(_ui->passwordEdit, &QLineEdit::textChanged, this, &FTPClientDialog::UpdateLineEditStyle);

    // Connect connect button
    _ui->connectButton->setIcon(IconUtils::GetIcon("connect"));
    connect(_ui->connectButton, &QPushButton::clicked, this, &FTPClientDialog::HandleConnectButton);
    connect(_ftpFolderTree, &FTPFileTree::ConnectionSucceeded, this, [this]() {
        _connected = true;
        _ui->connectButton->setText("Disconnect");
    });

    // Setup verification
    UpdateLineEditStyle(_ui->serverEdit->text());
    UpdateLineEditStyle(_ui->portEdit->text());
    UpdateLineEditStyle(_ui->userEdit->text());
    UpdateLineEditStyle(_ui->passwordEdit->text());

    // Set defaults
    if (!Configuration::instance().GetValue<QString>("ui.default-ftp-user", "").isEmpty()) {
        _ui->userEdit->setText(Configuration::instance().GetValue<QString>("ui.default-ftp-user", ""));
    }
    if (!Configuration::instance().GetValue<QString>("ui.default-ftp-password", "").isEmpty()) {
        _ui->passwordEdit->setText(Configuration::instance().GetValue<QString>("ui.default-ftp-password", ""));
    }

    if (!_localBaseDir.isEmpty()) {
        _localFolderTree->SetBaseDir(_localBaseDir);
    }
}

FTPClientDialog::~FTPClientDialog() {
    delete _ui;
}

void FTPClientDialog::SetupLogPanel() {

    // Connect the ingo thread to the log panel
    connect(&FTPInfoThread::instance(), &FTPInfoThread::emitInfo, this, &FTPClientDialog::LogInfoMessage);

    // Log data model
    _logDataModel = new QStandardItemModel(_ui->logList);
    _ui->logList->setModel(_logDataModel);

    _ui->logScrollButton->setText(nullptr);
    _ui->logScrollButton->setIcon(IconUtils::GetIcon("scroll"));
    connect(_ui->logScrollButton, &QPushButton::toggled, this, [this](bool value) {
        _logScrolling = value;
    });
}


void FTPClientDialog::LogInfoMessage(const QString &message) const {
    _logDataModel->appendRow(new QStandardItem(message));

    // Auto-scroll to bottom
    if (_logScrolling) {
        _ui->logList->scrollToBottom();
    }
    _ui->statusLabel->setText("Last update: " + DateTimeUtils::GetLogTimeFormat(QDateTime::currentDateTime()));
}

void FTPClientDialog::UpdateLineEditStyle(const QString &text) const {
    Q_UNUSED(text);
    const auto senderEdit = qobject_cast<QLineEdit *>(sender());
    if (!senderEdit)
        return;

    // Get the validator state
    int pos = 0;
    QString input = senderEdit->text();
    const QValidator::State state = senderEdit->validator()->validate(input, pos);

    // Apply color based on the state
    SetLineEditColor(senderEdit, state);
}

void FTPClientDialog::SetLineEditColor(QLineEdit *lineEdit, const QValidator::State state) {
    // Define colors
    const QString defaultStyle =
            "QLineEdit { border: 1px solid #ccc; background-color: #424242; padding: 2px; border-radius: 4px; }";
    const QString invalidStyle = "QLineEdit { border: 2px solid #dc3545; padding: 2px; border-radius: 4px; }";
    // Red frame/background

    switch (state) {
        case QValidator::Acceptable:
        case QValidator::Intermediate:
            lineEdit->setStyleSheet(defaultStyle);
            break;
        case QValidator::Invalid:
            lineEdit->setStyleSheet(invalidStyle);
            break;
        default:
            lineEdit->setStyleSheet(defaultStyle);
            break;
    }
}

void FTPClientDialog::HandleConnectButton() {

    if (_connected) {

        _ftpFolderTree->Disconnect();
        _ui->connectButton->setText("Connect");
        _connected = false;

    } else {
        // Check name
        int pos = 0;
        QString server = _ui->serverEdit->text();
        if (const QValidator::State serverState = _ui->serverEdit->validator()->validate(server, pos);
            serverState != QValidator::Acceptable) {
            SetLineEditColor(_ui->serverEdit, serverState);
            QMessageBox::warning(this, "Validation Failure", "Server is invalid or incomplete.");
            return;
        }

        QString port = _ui->portEdit->text();
        if (const QValidator::State portState = _ui->portEdit->validator()->validate(port, pos);
            portState != QValidator::Acceptable) {
            SetLineEditColor(_ui->portEdit, portState);
            QMessageBox::warning(this, "Validation Failure", "Port is invalid or incomplete.");
            return;
        }

        QString user = _ui->userEdit->text();
        if (const QValidator::State userState = _ui->userEdit->validator()->validate(user, pos);
            userState != QValidator::Acceptable) {
            SetLineEditColor(_ui->userEdit, userState);
            QMessageBox::warning(this, "Validation Failure", "User is invalid.");
            return;
        }

        QString password = _ui->passwordEdit->text();
        if (const QValidator::State passwordState = _ui->passwordEdit->validator()->validate(password, pos);
            passwordState != QValidator::Acceptable) {
            SetLineEditColor(_ui->passwordEdit, passwordState);
            QMessageBox::warning(this, "Validation Failure", "Password cannot be empty.");
        }

        // All valid, so connect
        _ftpFolderTree->Connect(server, port, user, password);
    }
}

void FTPClientDialog::HandleReject() {
    accept();
}
