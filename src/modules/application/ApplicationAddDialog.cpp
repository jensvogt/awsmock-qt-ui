//
// Created by vogje01 on 11/7/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ApplicationAddDialog.h" resolved

#include <QMessageBox>
#include <modules/application/ApplicationAddDialog.h>

#include "dto/application/ApplicationCreateRequest.h"
#include "modules/application/ApplicationList.h"
#include "modules/application/ApplicationService.h"
#include "utils/FileUtils.h"
#include "utils/IconUtils.h"
#include "utils/JsonUtils.h"

ApplicationAddDialog::ApplicationAddDialog(QWidget *parent) : QDialog(parent) {

    // Connect service
    applicationService = new ApplicationService();

    // 1. Setup UI Components
    setWindowTitle("Add Application");
    setMinimumSize(800, 500);

    // This is the main layout for the dialog
    const auto mainLayout = new QVBoxLayout(this);

    // Name
    nameLabel = new QLabel("Name:", this);
    nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("Name");

    // Regex for the name
    const QRegularExpression nameRegex("\\b[A-Z0-9._+-]+\\b", QRegularExpression::CaseInsensitiveOption);
    const auto nameValidator = new QRegularExpressionValidator(nameRegex, this);
    nameEdit->setValidator(nameValidator);
    connect(nameEdit, &QLineEdit::textChanged, this, &ApplicationAddDialog::UpdateLineEditStyle);

    // Runtime
    runtimeLabel = new QLabel("Runtime:", this);
    runtimeComboBox = new QComboBox(this);
    runtimeComboBox->addItem("Java 11", "JAVA11");
    runtimeComboBox->addItem("Java 17", "JAVA17");
    runtimeComboBox->addItem("Java 21", "JAVA21");
    runtimeComboBox->addItem("Python 3.9", "python3-9");
    runtimeComboBox->addItem("Python 3.10", "python3-10");
    runtimeComboBox->addItem("Python 3.11", "python3-11");
    runtimeComboBox->addItem("Python 3.12", "python3-12");
    runtimeComboBox->addItem("Python 3.13", "python3-13");
    runtimeComboBox->addItem("Node JS 20", "nodejs-20");
    runtimeComboBox->addItem("Node JS 22", "nodejs-22");
    runtimeComboBox->addItem("Go", "go");
    runtimeComboBox->setCurrentIndex(2);

    // Private port
    privatePortLabel = new QLabel("PrivatePort:", this);
    privatePortEdit = new QLineEdit("8080", this);
    privatePortEdit->setPlaceholderText("Private Port");
    // Valid range: 1 to 65536
    const auto privatePortValidator = new QIntValidator(1, 65536, this);
    privatePortEdit->setValidator(privatePortValidator);
    privatePortEdit->setPlaceholderText("Private port (1 - 65536)");
    connect(privatePortEdit, &QLineEdit::textChanged, this, &ApplicationAddDialog::UpdateLineEditStyle);

    // Public port
    publicPortLabel = new QLabel("PublicPort:", this);
    publicPortEdit = new QLineEdit("8080", this);
    publicPortEdit->setPlaceholderText("Public Port");
    // Valid range: 1 to 65536
    const auto publicPortValidator = new QIntValidator(1, 65536, this);
    publicPortEdit->setValidator(publicPortValidator);
    publicPortEdit->setPlaceholderText("Public port (1 - 65536)");
    connect(publicPortEdit, &QLineEdit::textChanged, this, &ApplicationAddDialog::UpdateLineEditStyle);

    // Version
    versionLabel = new QLabel("Version:", this);
    versionEdit = new QLineEdit(this);
    versionEdit->setDisabled(true);
    versionEdit->setPlaceholderText("Version");

    // Version
    fileNameLabel = new QLabel("File:", this);
    fileNameEdit = new QLineEdit(this);
    fileNameEdit->setDisabled(true);
    fileNameEdit->setPlaceholderText("File path");
    // Valid range: 1 to 65536
    const QRegularExpression filenameRegex("\\b[A-Z0-9._+-]+\\b", QRegularExpression::CaseInsensitiveOption);
    const auto filenameValidator = new QRegularExpressionValidator(nameRegex, this);
    fileNameEdit->setValidator(filenameValidator);
    connect(fileNameEdit, &QLineEdit::textChanged, this, &ApplicationAddDialog::UpdateLineEditStyle);

    // A label to give instructions
    statusLabel = new QLabel("Drag the file here.", this);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("QLabel { color: #999; font-size: 16px; padding: 10px; border: 2px dashed #999; }");
    statusLabel->setAlignment(Qt::AlignCenter);

    // Create a button box with standard OK and Close buttons
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Close, this);

    // Create the custom "Open" button
    const auto openButton = new QPushButton(IconUtils::GetIcon("fileexplorer"), "Open", this);

    // Add the "Open" button with ActionRole. This role ensures the button
    // performs an action without automatically closing the dialog.
    buttonBox->addButton(openButton, QDialogButtonBox::ActionRole);

    // Connect the custom Open button's clicked signal to the custom slot
    connect(openButton, &QPushButton::clicked, this, &ApplicationAddDialog::OpenFilesDialog);

    // Connect the standard signals to the dialog's built-in slots.
    // QDialogButtonBox::Ok button fires the accepted() signal -> QDialog::accept()
    //connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    // Connect the Ok button to our custom verification slot
    // We use buttonBox->button(QDialogButtonBox::Ok) because we want to connect
    // the click event, not the standard QDialogButtonBox accepted signal.
    connect(buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &ApplicationAddDialog::VerifyAllInputs);

    // QDialogButtonBox::Close button fires the rejected() signal -> QDialog::reject()
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // Add the button box to the bottom of the layout
    mainLayout->addWidget(buttonBox);

    mainLayout->addWidget(nameLabel);
    mainLayout->addWidget(nameEdit);
    mainLayout->addWidget(runtimeLabel);
    mainLayout->addWidget(runtimeComboBox);
    mainLayout->addWidget(privatePortLabel);
    mainLayout->addWidget(privatePortEdit);
    mainLayout->addWidget(publicPortLabel);
    mainLayout->addWidget(publicPortEdit);
    mainLayout->addWidget(versionLabel);
    mainLayout->addWidget(versionEdit);
    mainLayout->addWidget(fileNameLabel);
    mainLayout->addWidget(fileNameEdit);
    mainLayout->addWidget(statusLabel, 3);

    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);

    // Enable Drop Events for this widget
    setAcceptDrops(true);

    // Apply initial styles
    UpdateLineEditStyle(versionEdit->text());
    UpdateLineEditStyle(fileNameEdit->text());
    UpdateLineEditStyle(privatePortEdit->text());
}

ApplicationAddDialog::~ApplicationAddDialog() = default;

void ApplicationAddDialog::dragEnterEvent(QDragEnterEvent *event) {
    // Check if the data being dragged contains file URLs
    if (event->mimeData()->hasUrls()) {
        // Accept the proposed action (copy, move, or link)
        event->acceptProposedAction();
        statusLabel->setStyleSheet("QLabel { color: #007bff; font-size: 16px; padding: 10px; border: 2px dashed #007bff; background-color: #e6f3ff; }");
    } else {
        // Reject the event if it's not file URLs
        event->ignore();
        statusLabel->setStyleSheet("QLabel { color: #333; font-size: 16px; padding: 10px; border: 2px dashed #999; }");
    }
}

void ApplicationAddDialog::dropEvent(QDropEvent *event) {
    // Ensure the data is file URLs
    if (event->mimeData()->hasUrls()) {
        for (const QUrl &url: event->mimeData()->urls()) {
            // Check if the URL is a local file
            if (url.isLocalFile()) {
                QString localFile = url.toLocalFile();
                QFileInfo fileInfo(localFile);
                QString fileName = fileInfo.fileName();

                QString version = FileUtils::ExtractVersionFromFileName(fileName);
                fileNameEdit->setText(fileName);
                versionEdit->setText(version);
                filePath = fileInfo;
            }
        }

        // Mark the event as accepted
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void ApplicationAddDialog::OpenFilesDialog() {
    // Create a QFileDialog set to select existing files
    QFileDialog dialog(this);
    dialog.setWindowTitle("Select Files to upload");
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter("All Files (*.*)");

    // Show the dialog and check if the user clicked OK
    if (dialog.exec()) {
        for (const QString &f: dialog.selectedFiles()) {
            QString version = FileUtils::ExtractVersionFromFileName(f);
            fileNameEdit->setText(QFileInfo(f).fileName());
            versionEdit->setText(version);
            filePath = QFileInfo(f);
        }
    }
}

void ApplicationAddDialog::SetLineEditColor(QLineEdit *lineEdit, const QValidator::State state) {

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

void ApplicationAddDialog::UpdateLineEditStyle(const QString &text) const {
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

void ApplicationAddDialog::VerifyAllInputs() {
    // Check name
    int pos = 0;
    QString name = nameEdit->text();
    if (name.isEmpty()) {
        SetLineEditColor(nameEdit, QValidator::Invalid);
    }
    const QValidator::State nameState = nameEdit->validator()->validate(name, pos);

    // Check private port
    if (privatePortEdit->text().isEmpty()) {
        SetLineEditColor(privatePortEdit, QValidator::Invalid);
    }
    QString sPrivatePort = privatePortEdit->text();
    const QValidator::State privatePortState = privatePortEdit->validator()->validate(sPrivatePort, pos);

    // Check public port
    if (publicPortEdit->text().isEmpty()) {
        SetLineEditColor(publicPortEdit, QValidator::Invalid);
    }
    QString sPublicPort = publicPortEdit->text();
    const QValidator::State publicPortState = privatePortEdit->validator()->validate(sPublicPort, pos);

    // Check public port
    if (publicPortEdit->text().isEmpty()) {
        SetLineEditColor(publicPortEdit, QValidator::Invalid);
    }
    QString sFilename = fileNameEdit->text();
    const QValidator::State fileNameState = fileNameEdit->validator()->validate(sFilename, pos);
    if (fileNameEdit->text().isEmpty()) {
        SetLineEditColor(fileNameEdit, QValidator::Invalid);
    }

    // Check if all are acceptable (fully valid)
    if (nameState == QValidator::Acceptable && privatePortState == QValidator::Acceptable && publicPortState == QValidator::Acceptable && fileNameState == QValidator::Acceptable) {
        UploadFile(filePath);
        this->accept();
    } else {
        QMessageBox::warning(this, "Validation Failure", "One or more required fields are invalid or incomplete. Please check the highlighted fields.");
    }
}

void ApplicationAddDialog::UploadFile(const QFileInfo &fileInfo) {

    // Create file
    QFile file(fileInfo.absoluteFilePath());

    // 2. Attempt to open the file for reading.
    // IMPORTANT: We omit QIODevice::Text here, as we are reading raw binary data.
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Read failure", "Could not read file.");
        return;
    }

    // 3. Read all content into a QByteArray
    const QByteArray fileData = file.readAll();

    ApplicationCreateRequest request;
    request.region = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");
    request.application.name = nameEdit->text();
    request.application.privatePort = privatePortEdit->text().toInt();
    request.application.publicPort = publicPortEdit->text().toInt();
    request.application.runtime = runtimeComboBox->currentData().toString();
    request.application.runType = "DOCKER";
    request.application.version = versionEdit->text();
    request.application.archive = fileInfo.fileName();
    request.applicationCode = fileData.toBase64();
    applicationService->CreateApplication(request);

    file.close();
}

