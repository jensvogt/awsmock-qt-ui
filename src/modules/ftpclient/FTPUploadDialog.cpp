//
// Created by vogje01 on 11/8/25.
//

#include <modules/ftpclient/FTPUploadDialog.h>
#include "ui_FTPUploadDialog.h"

FTPUploadDialog::FTPUploadDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::FTPUploadDialog) {

    // FTP client thread
    _ftpClientThread = new FTPClientThread();
    connect(_ftpClientThread, &FTPClientThread::emitFileListItem, this, &FTPUploadDialog::ReceiveTargetListItem);
    connect(_ftpClientThread, &FTPClientThread::emitSuccess, this, &FTPUploadDialog::ConnectionSucceeded);
    connect(_ftpClientThread, &FTPClientThread::finished, _ftpClientThread, &FTPClientThread::stop);
    connect(_ftpClientThread, &FTPClientThread::emitClearList, this, &FTPUploadDialog::TargetTreeClear);
    connect(_ftpClientThread->curClient->infoThread, &InfoThread::emitInfo, this, &FTPUploadDialog::LogInfoMessage);

    // Setup UI
    _ui->setupUi(this);

    // Log data model
    _logDataModel = new QStandardItemModel(_ui->logList);
    _ui->logList->setModel(_logDataModel);

    // Connect button box
    _ui->buttonBox->button(QDialogButtonBox::Close)->setText("Close");
    _ui->buttonBox->button(QDialogButtonBox::Close)->setIcon(IconUtils::GetIcon("exit"));
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &FTPUploadDialog::HandleReject);

    // Target tree view
    _targetTreeModel = new QStandardItemModel(this);
    _targetTreeModel->setHorizontalHeaderLabels({"Name", "Size", "Type", "Last Modified", "Permission", "Owner", "Group"});
    _targetTreeView = new DroppableTreeView(this);
    _targetTreeView->setModel(_targetTreeModel);
    _targetTreeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _targetTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _targetTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
    _targetTreeView->setAcceptDrops(true);
    _targetTreeView->setDropIndicatorShown(true);
    _targetTreeView->setDragEnabled(true); // if you also want dragging
    _targetTreeView->setDragDropMode(QAbstractItemView::DragDrop);

    // Set column width
    _targetTreeView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    _targetTreeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    _targetTreeView->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    _targetTreeView->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    _targetTreeView->header()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    _targetTreeView->header()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    _targetTreeView->header()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    connect(_targetTreeView, &QTreeView::doubleClicked, this, &FTPUploadDialog::TargetTreeItemClicked);
    connect(_targetTreeView, &DroppableTreeView::FileDropped, this, &FTPUploadDialog::TargetTreeFileDropped);
    connect(_targetTreeView, &DroppableTreeView::FileDeleted, this, &FTPUploadDialog::TargetTreeFileDeleted);
    _ui->horizontallySplitter->replaceWidget(1, _targetTreeView);

    // Source tree view
    SetupSourceTreeView();

    // Name validator
    const NotEmptyValidator *nameValidator = new NotEmptyValidator(this);
    _ui->serverEdit->setValidator(nameValidator);
    _ui->serverEdit->setPlaceholderText("Server hostname");
    connect(_ui->serverEdit, &QLineEdit::textChanged, this, &FTPUploadDialog::UpdateLineEditStyle);

    // Port validator
    const auto portValidator = new QIntValidator(1, 65536, this);
    _ui->portEdit->setValidator(portValidator);
    _ui->portEdit->setPlaceholderText("FTP port (1 - 65536)");
    connect(_ui->portEdit, &QLineEdit::textChanged, this, &FTPUploadDialog::UpdateLineEditStyle);

    // User validator
    const NotEmptyValidator *userValidator = new NotEmptyValidator(this);
    _ui->userEdit->setValidator(userValidator);
    connect(_ui->userEdit, &QLineEdit::textChanged, this, &FTPUploadDialog::UpdateLineEditStyle);

    // Password validator
    const NotEmptyValidator *passwordValidator = new NotEmptyValidator(this);
    _ui->passwordEdit->setValidator(passwordValidator);
    connect(_ui->passwordEdit, &QLineEdit::textChanged, this, &FTPUploadDialog::UpdateLineEditStyle);

    // Connect connect button
    _ui->connectButton->setIcon(IconUtils::GetIcon("connect"));
    connect(_ui->connectButton, &QPushButton::clicked, this, &FTPUploadDialog::VerifyConnectInputs);

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

FTPUploadDialog::~FTPUploadDialog() {
    delete _ftpClientThread;
    delete _ui;
}

void FTPUploadDialog::ConnectionSucceeded() {
    if (!_connected) {
        _connected = true;
        _ui->connectButton->setText("Disconnect");
    } else {
        _connected = false;
        _ui->connectButton->setText("Connect");
    }
}

void FTPUploadDialog::LogInfoMessage(const QString &message) const {
    _logDataModel->appendRow(new QStandardItem(message));

    // Auto-scroll to bottom
    // if (_localScrolling) {
    _ui->logList->scrollToBottom();
    //}
}

void FTPUploadDialog::SetupSourceTreeView() {
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

void FTPUploadDialog::ReceiveTargetListItem(const FileInfo &fileInfo) const {
    QList<QStandardItem *> row;
    row << new QStandardItem(fileInfo.name)
            << new QStandardItem(fileInfo.size > 0 ? QString::number(fileInfo.size) : nullptr)
            << new QStandardItem(fileInfo.type)
            << new QStandardItem(fileInfo.timestamp)
            << new QStandardItem(fileInfo.permissions)
            << new QStandardItem(fileInfo.username)
            << new QStandardItem(fileInfo.groupname);
    _targetTreeModel->appendRow(row);
}

void FTPUploadDialog::TargetTreeClear() const {
    _targetTreeModel->removeRows(0, _targetTreeModel->rowCount());
}

void FTPUploadDialog::TargetTreeItemClicked(const QModelIndex &index) const {
    const QStandardItem *item = _targetTreeModel->itemFromIndex(index);
    if (const QStandardItem *typeItem = _targetTreeModel->item(item->row(), 2); typeItem->text() != "directory")
        return;
    const QStandardItem *fileItem = _targetTreeModel->item(item->row(), 0);
    if (!_ftpClientThread->isRunning()) {
        _ftpClientThread->arglist[0] = fileItem->text().toStdString();
        _ftpClientThread->task = TCd;
        _ftpClientThread->start();
    }
}

void FTPUploadDialog::UpdateLineEditStyle(const QString &text) const {
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

void FTPUploadDialog::SetLineEditColor(QLineEdit *lineEdit, const QValidator::State state) {
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

void FTPUploadDialog::VerifyConnectInputs() {

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
        return;
    }
}

void FTPUploadDialog::TargetTreeFileDropped(const QString &filePath) const {
    _ftpClientThread->task = TUp;
    _ftpClientThread->arglist[0] = filePath.toStdString();
    _ftpClientThread->start();
}

void FTPUploadDialog::TargetTreeFileDeleted(const QString &filePath) const {
    _ftpClientThread->task = TDele;
    _ftpClientThread->arglist[0] = filePath.toStdString();
    _ftpClientThread->start();
}

void FTPUploadDialog::HandleReject() {
    accept();
}
