//
// Created by vogje01 on 11/8/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_FTPUploadDialog.h" resolved

#include <modules/ftp/FTPUploadDialog.h>
#include "ui_FTPUploadDialog.h"

FTPUploadDialog::FTPUploadDialog(QWidget *parent) : QDialog(parent), ui(new Ui::FTPUploadDialog) {

    // Setup UI
    ui->setupUi(this);

    // Connect button box
    ui->buttonBox->button(QDialogButtonBox::Close)->setText("Close");
    ui->buttonBox->button(QDialogButtonBox::Close)->setIcon(IconUtils::GetIcon("exit"));
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &FTPUploadDialog::HandleReject);
    ui->buttonBox->button(QDialogButtonBox::Apply)->setText("Upload");
    ui->buttonBox->button(QDialogButtonBox::Apply)->setIcon(IconUtils::GetIcon("upload"));
    connect(ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &FTPUploadDialog::HandleAccept);

    // Name validator
    const NotEmptyValidator *nameValidator = new NotEmptyValidator(this);
    ui->serverEdit->setValidator(nameValidator);
    ui->serverEdit->setPlaceholderText("Server hostname");
    connect(ui->serverEdit, &QLineEdit::textChanged, this, &FTPUploadDialog::UpdateLineEditStyle);

    // Port validator
    const auto portValidator = new QIntValidator(1, 65536, this);
    ui->portEdit->setValidator(portValidator);
    ui->portEdit->setPlaceholderText("FTP port (1 - 65536)");
    connect(ui->portEdit, &QLineEdit::textChanged, this, &FTPUploadDialog::UpdateLineEditStyle);

    // User validator
    const NotEmptyValidator *userValidator = new NotEmptyValidator(this);
    ui->userEdit->setValidator(userValidator);
    connect(ui->userEdit, &QLineEdit::textChanged, this, &FTPUploadDialog::UpdateLineEditStyle);

    // Password validator
    const NotEmptyValidator *passwordValidator = new NotEmptyValidator(this);
    ui->passwordEdit->setValidator(passwordValidator);
    connect(ui->passwordEdit, &QLineEdit::textChanged, this, &FTPUploadDialog::UpdateLineEditStyle);

    // Source validator
    const NotEmptyValidator *sourceValidator = new NotEmptyValidator(this);
    ui->sourceEdit->setValidator(sourceValidator);
    ui->sourceEdit->setPlaceholderText("Source filename");
    connect(ui->sourceEdit, &QLineEdit::textChanged, this, &FTPUploadDialog::UpdateLineEditStyle);

    // Target validator
    const NotEmptyValidator *targetValidator = new NotEmptyValidator(this);
    ui->targetEdit->setValidator(targetValidator);
    ui->targetEdit->setPlaceholderText("Target filename");
    connect(ui->targetEdit, &QLineEdit::textChanged, this, &FTPUploadDialog::UpdateLineEditStyle);

    // Connect source browse button
    //ui->sourceBrowseButton->setText("Browse");
    ui->sourceBrowseButton->setIcon(IconUtils::GetIcon("browse"));
    ui->targetBrowseButton->setIcon(IconUtils::GetIcon("browse"));
    connect(ui->sourceBrowseButton, &QPushButton::clicked, this, &FTPUploadDialog::BrowseSourceFile);

    // Disable browse buttons
    ui->sourceBrowseButton->setDisabled(true);
    ui->targetBrowseButton->setDisabled(true);
    ui->dropAreaLabel->setDisabled(true);

    // Connect connect button
    ui->connectButton->setIcon(IconUtils::GetIcon("connect"));
    connect(ui->connectButton, &QPushButton::clicked, this, &FTPUploadDialog::VerifyConnectInputs);

    // Setup verification
    UpdateLineEditStyle(ui->serverEdit->text());
    UpdateLineEditStyle(ui->portEdit->text());
    UpdateLineEditStyle(ui->userEdit->text());
    UpdateLineEditStyle(ui->passwordEdit->text());

    // Set defaults
    if (!Configuration::instance().GetValue<QString>("ui.default-ftp-user", "").isEmpty()) {
        ui->userEdit->setText(Configuration::instance().GetValue<QString>("ui.default-ftp-user", ""));
    }
    if (!Configuration::instance().GetValue<QString>("ui.default-ftp-password", "").isEmpty()) {
        ui->passwordEdit->setText(Configuration::instance().GetValue<QString>("ui.default-ftp-password", ""));
    }
    // Enable Drop Events for this widget
    setAcceptDrops(false);
}

FTPUploadDialog::~FTPUploadDialog() {
    delete ui;
}

void FTPUploadDialog::BrowseSourceFile() {

    // Create a QFileDialog set to select existing files
    const auto filter = "All Files (*.*)";
    const auto defaultDir = Configuration::instance().GetValue<QString>("ui.default-directory", "/usr/local/awsmock-qt-ui");

    if (const QString filePath = QFileDialog::getOpenFileName(nullptr, "Open source file", defaultDir, filter); !filePath.isEmpty()) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(nullptr, "Error", "Could not open file:" + filePath);
            return;
        }
        ui->sourceEdit->setText(file.fileName());
        sourceFileInfo = QFileInfo(file.fileName());
        Configuration::instance().SetValue<QString>("ui.default-directory", sourceFileInfo.absolutePath());
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
    const QString defaultStyle = "QLineEdit { border: 1px solid #ccc; background-color: #424242; padding: 2px; border-radius: 4px; }";
    const QString invalidStyle = "QLineEdit { border: 2px solid #dc3545; padding: 2px; border-radius: 4px; }"; // Red frame/background

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

    // Check name
    int pos = 0;
    QString server = ui->serverEdit->text();
    if (const QValidator::State serverState = ui->serverEdit->validator()->validate(server, pos); serverState != QValidator::Acceptable) {
        SetLineEditColor(ui->serverEdit, serverState);
        QMessageBox::warning(this, "Validation Failure", "Server is invalid or incomplete.");
        return;
    }

    QString port = ui->portEdit->text();
    if (const QValidator::State portState = ui->portEdit->validator()->validate(port, pos); portState != QValidator::Acceptable) {
        SetLineEditColor(ui->portEdit, portState);
        QMessageBox::warning(this, "Validation Failure", "Port is invalid or incomplete.");
        return;
    }

    QString user = ui->userEdit->text();
    if (const QValidator::State userState = ui->userEdit->validator()->validate(user, pos); userState != QValidator::Acceptable) {
        SetLineEditColor(ui->userEdit, userState);
        QMessageBox::warning(this, "Validation Failure", "User is invalid.");
        return;
    }

    QString password = ui->passwordEdit->text();
    if (const QValidator::State passwordState = ui->passwordEdit->validator()->validate(password, pos); passwordState != QValidator::Acceptable) {
        SetLineEditColor(ui->passwordEdit, passwordState);
        QMessageBox::warning(this, "Validation Failure", "Password cannot be empty.");
        return;
    }

    // UploadFile(filePath);
    InitFtpClient();
}

void FTPUploadDialog::VerifyFileInputs() {

    // Check name
    int pos = 0;
    QString target = ui->targetEdit->text();
    if (const QValidator::State targetState = ui->targetEdit->validator()->validate(target, pos); targetState != QValidator::Acceptable) {
        SetLineEditColor(ui->targetEdit, targetState);
        QMessageBox::warning(this, "Validation Failure", "Target directory is invalid or incomplete.");
        return;
    }

    QString source = ui->sourceEdit->text();
    if (const QValidator::State sourceState = ui->targetEdit->validator()->validate(source, pos); sourceState != QValidator::Acceptable) {
        SetLineEditColor(ui->sourceEdit, sourceState);
        QMessageBox::warning(this, "Validation Failure", "Source file is invalid or incomplete.");
        return;
    }

    if (const std::string targetFilename = ui->targetEdit->text().toStdString() + "/" + sourceFileInfo.fileName().toStdString(); ftpClient->UploadFile(sourceFileInfo.absoluteFilePath().toStdString(), targetFilename)) {
        QMessageBox::information(this, "Information", "Upload successful.");
    } else {
        QMessageBox::warning(this, "Warning", "Upload not successful.");
    }
}

void FTPUploadDialog::InitFtpClient() {

    ftpClient = new embeddedmz::CFTPClient([](const std::string &strLogMsg) { qDebug() << QString::fromStdString(strLogMsg); });

    const int port = ui->portEdit->text().toInt();
    const std::string stdServer = ui->serverEdit->text().toStdString();
    const std::string stdUser = ui->userEdit->text().toStdString();
    if (const std::string stdPassword = ui->passwordEdit->text().toStdString(); !ftpClient->InitSession(stdServer, port, stdUser, stdPassword)) {
        QMessageBox::warning(nullptr, "Error", ("Could not connect to FTP server: " + stdServer).data());
    }

    // Enable browse buttons
    ui->sourceBrowseButton->setDisabled(false);
    ui->targetBrowseButton->setDisabled(false);
    ui->dropAreaLabel->setDisabled(false);

    // Enable Drop Events for this widget
    setAcceptDrops(true);

    // Set status
    ui->statusText->setText("Connected: " + QString::fromStdString(stdServer) + ":" + QString::number(port));
}

void FTPUploadDialog::dragEnterEvent(QDragEnterEvent *event) {

    // Check if the data being dragged contains file URLs
    if (event->mimeData()->hasUrls()) {
        // Accept the proposed action (copy, move, or link)
        event->acceptProposedAction();
        ui->dropAreaLabel->setStyleSheet("QLabel { color: #007bff; font-size: 16px; padding: 10px; border: 2px dashed #007bff; background-color: #e6f3ff; }");
    } else {
        // Reject the event if it's not file URLs
        event->ignore();
        ui->dropAreaLabel->setStyleSheet("QLabel { color: #333; font-size: 16px; padding: 10px; border: 2px dashed #999; }");
    }
}

void FTPUploadDialog::dropEvent(QDropEvent *event) {
    // Ensure the data is file URLs
    if (event->mimeData()->hasUrls()) {
        for (QList<QUrl> urls = event->mimeData()->urls(); const QUrl &url: urls) {
            // Check if the URL is a local file
            if (url.isLocalFile()) {
                QString localFile = url.toLocalFile();
                sourceFileInfo = QFileInfo(localFile);
                ui->sourceEdit->setText(sourceFileInfo.absoluteFilePath());
            }
        }

        // Mark the event as accepted
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void FTPUploadDialog::HandleAccept() {
    VerifyFileInputs();
}

void FTPUploadDialog::HandleReject() {
    qDebug() << "Rejected";
    accept();
}
