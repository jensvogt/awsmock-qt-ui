//
// Created by vogje01 on 11/8/25.
//

#include <modules/ftpclient/FTPClientDialog.h>
#include "ui_FTPClientDialog.h"
#include "components/FTPFileTree.h"

FTPClientDialog::FTPClientDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::FTPClientDialog) {
    // FTP client thread
    _ftpClientThread = new FTPClientThread();
    connect(_ftpClientThread, &FTPClientThread::emitFileListItem, this, &FTPClientDialog::ReceiveTargetListItem);
    connect(_ftpClientThread, &FTPClientThread::emitSuccess, this, &FTPClientDialog::ConnectionSucceeded);
    connect(_ftpClientThread, &FTPClientThread::finished, _ftpClientThread, &FTPClientThread::stop);
    //connect(_ftpClientThread, &FTPClientThread::emitClearList, this, &FTPClientDialog::TargetTreeClear);
    connect(_ftpClientThread->curClient->infoThread, &InfoThread::emitInfo, this, &FTPClientDialog::LogInfoMessage);

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
    _localFolderTree->HideAllColumns();
    _ui->horizontalSplitter1->addWidget(_localFolderTree);

    // FTP folder tree
    _ftpFolderTree = new FTPFileTree(nullptr);
    _ftpFolderTree->HideColumns({1, 2, 3, 4, 5, 6, 7, 8});
    connect(_ftpFolderTree, &FTPFileTree::FolderSelectedSignal, this, [this](const QString &absPath) {
        _ftpFileTree->Clear();
        if (!_ftpClientThread->isRunning()) {
            _ftpClientThread->arglist[0] = absPath.toStdString();
            _ftpClientThread->task = TCd;
            _ftpClientThread->start();
        }
    });
    _ui->horizontalSplitter1->addWidget(_ftpFolderTree);

    // Local file tree
    _localFileTree = new FTPFileTree(nullptr);
    _ui->horizontalSplitter2->addWidget(_localFileTree);

    // FTP folder tree
    _ftpFileTree = new FTPFileTree(nullptr);
    _ui->horizontalSplitter2->addWidget(_ftpFileTree);
    /*
            // Add context menu
            _targetTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(_targetTreeView, &QTableView::customContextMenuRequested, this, &FTPClientDialog::ShowTargetContextMenu);
        */
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
        _targetTreeModel->removeRows(0, _targetTreeModel->rowCount());
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

    // Hide filename line edit and file type combobox
    for (QLineEdit *le: fileDialog->findChildren<QLineEdit *>()) le->hide();
    for (QLabel *le: fileDialog->findChildren<QLabel *>()) le->hide();
    for (QComboBox *cb: fileDialog->findChildren<QComboBox *>()) cb->hide();
    for (QDialogButtonBox *bb: fileDialog->findChildren<QDialogButtonBox *>()) bb->hide();
    for (QPushButton *bb: fileDialog->findChildren<QPushButton *>()) bb->hide();
    _ui->horizontallySplitter->replaceWidget(0, fileDialog);*/
}

void FTPClientDialog::ReceiveTargetListItem(const FileInfo &fileInfo) const {
    if (fileInfo.type == "folder") {
        _ftpFolderTree->AddFolder(fileInfo.name);
    }
    if (fileInfo.type == "file") {
        _ftpFileTree->AddFile(fileInfo.name, fileInfo.size, fileInfo.timestamp, fileInfo.permissions, fileInfo.username, fileInfo.groupname);
    }
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
            _ftpClientThread->start();
        } else {
            _ftpClientThread->task = TDisconnect;
            _ftpClientThread->start();
            _connected = false;
            _ftpFolderTree->Clear();
            _ftpFileTree->Clear();
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

void FTPClientDialog::ShowTargetContextMenu(const QPoint &pos) {
    if (!_connected) {
        return;
    }

    QMenu menu;

    const QModelIndex index = _targetTreeView->indexAt(pos);

    // If not valid, allow only create directory
    if (!index.isValid()) {
        QAction *addDirAction = menu.addAction(IconUtils::GetIcon("add-directory"), "Create directory");
        addDirAction->setToolTip("Create directory");
        if (const auto selectedAction = menu.exec(_targetTreeView->viewport()->mapToGlobal(pos)); selectedAction == addDirAction) {
            TargetTreeAddDirectory();
        }
        return;
    }

    const int row = index.row();

    const QStandardItem *item = _targetTreeModel->itemFromIndex(index);
    const QString name = _targetTreeModel->item(item->row(), 0)->text();
    const QString type = _targetTreeModel->item(item->row(), 2)->text();

    if (type == "file") {
        QAction *renameAction = menu.addAction(IconUtils::GetIcon("rename"), "Rename File");
        renameAction->setToolTip("Rename the file");

        menu.addSeparator();

        QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete File");
        deleteAction->setToolTip("Delete the file");

        const QString filePath = _targetTreeModel->item(row, 0)->text();
        if (const auto selectedAction = menu.exec(_targetTreeView->viewport()->mapToGlobal(pos)); selectedAction == renameAction) {
            TargetTreeFileRename(filePath);
        } else if (selectedAction == deleteAction) {
            TargetTreeFileDelete(filePath);
        }
    } else if (type == "folder") {
        QAction *renameAction = menu.addAction(IconUtils::GetIcon("rename"), "Rename Directory");
        renameAction->setToolTip("Rename the directory");

        menu.addSeparator();

        QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Directory");
        deleteAction->setToolTip("Delete the directory");

        const QString filePath = _targetTreeModel->item(row, 0)->text();
        if (const auto selectedAction = menu.exec(_targetTreeView->viewport()->mapToGlobal(pos)); selectedAction == renameAction) {
            TargetTreeDirectoryRename(filePath);
        } else if (selectedAction == deleteAction) {
            TargetTreeDirectoryDelete(filePath);
        }
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
