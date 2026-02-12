//
// Created by vogje01 on 11/8/25.
//

#include <modules/ftpclient/FTPClientDialog.h>
#include "ui_FTPClientDialog.h"

FTPClientDialog::FTPClientDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::FTPClientDialog) {

    // Setup UI
    _ui->setupUi(this);

    // Logging panel
    SetupLogPanel();

    // Connect button box
    _ui->buttonBox->button(QDialogButtonBox::Close)->setText("Close");
    _ui->buttonBox->button(QDialogButtonBox::Close)->setIcon(IconUtils::GetIcon("exit"));
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &FTPClientDialog::HandleReject);

    // Local folder tree
    _localFolderTree = new FTPFileTree(nullptr);
    _localFolderTree->HideColumns({1, 2, 3, 4, 5, 6, 7, 8});
    _ui->horizontalSplitter1->addWidget(_localFolderTree);

    // FTP folder tree
    _ftpFolderTree = new FTPFileTree(nullptr);
    _ftpFolderTree->HideColumns({1, 2, 3, 4, 5, 6, 7, 8});
    _ui->horizontalSplitter1->addWidget(_ftpFolderTree);

    connect(_ftpFolderTree, &FTPFileTree::FolderSelectedSignal, this, &FTPClientDialog::TargetFolderSelectionChanged);
    connect(_ftpFolderTree, &FTPFileTree::TargetTreeFileRenameSignal, this, &FTPClientDialog::TargetTreeFileRename);
    connect(_ftpFolderTree, &FTPFileTree::TargetTreeFileDeleteSignal, this, &FTPClientDialog::TargetTreeFileDelete);
    connect(_ftpFolderTree, &FTPFileTree::TargetTreeDirectoryRename, this, &FTPClientDialog::TargetTreeDirectoryRename);
    connect(_ftpFolderTree, &FTPFileTree::TargetTreeDirectoryDelete, this, &FTPClientDialog::TargetTreeDirectoryDelete);
    connect(_ftpFolderTree->_fileTreeView, &DroppableTreeView::FileDropped, this, &FTPClientDialog::TargetTreeFileDropped);

    /*
            // Add context menu
            _targetTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(_targetTreeView, &QTableView::customContextMenuRequested, this, &FTPClientDialog::ShowFileContextMenu);
        */

    // FTP client thread
    _ftpClientThread = new FTPClientThread();
    connect(_ftpClientThread, &FTPClientThread::emitFileListItem, this, &FTPClientDialog::ReceiveTargetListItem);
    connect(_ftpClientThread, &FTPClientThread::emitSuccess, this, &FTPClientDialog::ConnectionSucceeded);
    connect(_ftpClientThread, &FTPClientThread::finished, _ftpClientThread, &FTPClientThread::stop);
    //connect(_ftpClientThread, &FTPClientThread::emitClearList, this, &FTPClientDialog::TargetTreeClear);
    connect(_ftpClientThread->curClient->infoThread, &InfoThread::emitInfo, this, &FTPClientDialog::LogInfoMessage);

    // Source tree view
    SetupSourceTreeView();

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
}

FTPClientDialog::~FTPClientDialog() {
    delete _ftpClientThread;
    delete _ui;
}

void FTPClientDialog::SetupLogPanel() {
    // Log data model
    _logDataModel = new QStandardItemModel(_ui->logList);
    _ui->logList->setModel(_logDataModel);

    _ui->logScrollButton->setText(nullptr);
    _ui->logScrollButton->setIcon(IconUtils::GetIcon("scroll"));
    connect(_ui->logScrollButton, &QPushButton::toggled, this, [this](bool value) {
        _logScrolling = value;
    });
}

void FTPClientDialog::ConnectionSucceeded() {
    if (!_connected) {
        _connected = true;
        _ui->connectButton->setText("Disconnect");
    } else {
        _connected = false;
        _ui->connectButton->setText("Connect");
    }
}

void FTPClientDialog::LogInfoMessage(const QString &message) const {
    _logDataModel->appendRow(new QStandardItem(message));

    // Auto-scroll to bottom
    if (_logScrolling) {
        _ui->logList->scrollToBottom();
    }
}

void FTPClientDialog::SetupSourceTreeView() {
    /*QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    fileDialog->setOption(QFileDialog::DontUseNativeDialog, true);

    // Hide filename line edit and file contentType combobox
    for (QLineEdit *le: fileDialog->findChildren<QLineEdit *>()) le->hide();
    for (QLabel *le: fileDialog->findChildren<QLabel *>()) le->hide();
    for (QComboBox *cb: fileDialog->findChildren<QComboBox *>()) cb->hide();
    for (QDialogButtonBox *bb: fileDialog->findChildren<QDialogButtonBox *>()) bb->hide();
    for (QPushButton *bb: fileDialog->findChildren<QPushButton *>()) bb->hide();
    _ui->horizontallySplitter->replaceWidget(0, fileDialog);*/
}

void FTPClientDialog::ReceiveTargetListItem(const FileInfo &fileInfo, QStandardItem *parent) const {
    _ftpFolderTree->AddItem(fileInfo, parent);
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
    if (!_ftpClientThread->isRunning()) {
        if (!_connected) {
            const QString ip_addr = _ui->serverEdit->text();
            const QString username = _ui->userEdit->text();
            const QString password = _ui->passwordEdit->text();
            _ftpClientThread->curClient->login(ip_addr, username, password);
            _ftpClientThread->task = TConnect;
            _ftpClientThread->parent = _ftpFolderTree->GetRootItem();
            _ftpClientThread->start();
        } else {
            _ftpClientThread->task = TDisconnect;
            _ftpClientThread->start();
            _connected = false;
            _ftpFolderTree->Clear();
            _ui->connectButton->setText("Connect");
        }
    }

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
}

void FTPClientDialog::TargetFolderSelectionChanged(const QString &absPath, QStandardItem *parent) const {
    if (!_ftpClientThread->isRunning()) {
        _ftpClientThread->arglist[0] = absPath.toStdString();
        _ftpClientThread->parent = parent;
        _ftpClientThread->task = TCd;
        _ftpClientThread->start();
    }
}

void FTPClientDialog::TargetTreeFileDropped(const QString &filePath) const {
    _ftpClientThread->task = TUp;
    _ftpClientThread->arglist[0] = filePath.toStdString();
    _ftpClientThread->start();
}

void FTPClientDialog::TargetTreeFileDelete(const QString &filePath) const {
    _ftpClientThread->task = TDele;
    _ftpClientThread->arglist[0] = filePath.toStdString();
    _ftpClientThread->start();
}

void FTPClientDialog::TargetTreeFileRename(const QString &filePath) {
    bool ok;
    const QString newFilePath = QInputDialog::getText(this, "New File Name", "Enter new name:", QLineEdit::Normal, "", &ok);

    if (!ok || newFilePath.isEmpty()) {
        return;
    }
    _ftpClientThread->task = TRename;
    _ftpClientThread->arglist[0] = filePath.toStdString();
    _ftpClientThread->arglist[1] = newFilePath.toStdString();
    _ftpClientThread->start();
}

void FTPClientDialog::TargetTreeAddDirectory() {
    bool ok;
    const QString newDirectory = QInputDialog::getText(this, "Create Directory", "Enter directory name:", QLineEdit::Normal, "", &ok);

    if (!ok || newDirectory.isEmpty()) {
        return;
    }
    _ftpClientThread->task = TMkd;
    _ftpClientThread->arglist[0] = newDirectory.toStdString();
    _ftpClientThread->start();
}

void FTPClientDialog::TargetTreeDirectoryDelete(const QString &filePath) const {
    _ftpClientThread->task = TRmd;
    _ftpClientThread->arglist[0] = filePath.toStdString();
    _ftpClientThread->start();
}

void FTPClientDialog::TargetTreeDirectoryRename(const QString &filePath) {
    bool ok;
    const QString newFilePath = QInputDialog::getText(this, "New Directory Name", "Enter new name:", QLineEdit::Normal, "", &ok);

    if (!ok || newFilePath.isEmpty()) {
        return;
    }
    _ftpClientThread->task = TRename;
    _ftpClientThread->arglist[0] = filePath.toStdString();
    _ftpClientThread->arglist[1] = newFilePath.toStdString();
    _ftpClientThread->start();
}

void FTPClientDialog::HandleReject() {
    accept();
}
