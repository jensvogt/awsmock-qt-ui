//
// Created by vogje01 on 11/8/25.
//

#include <modules/lambda/LambdaUploadCodeDialog.h>
#include "ui_LambdaUploadCodeDialog.h"

LambdaUploadCodeDialog::LambdaUploadCodeDialog(const QString &lambdaName, const QString &lambdaArn, QWidget *parent) : QDialog(parent), _ui(new Ui::LambdaUploadCodeDialog), _lambdaName(lambdaName), _lambdaArn(lambdaArn) {
    // Connect lambda service
    _lambdaService = new LambdaService();

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &LambdaUploadCodeDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &LambdaUploadCodeDialog::HandleReject);

    // Connect browse button
    _ui->browseButton->setIcon(IconUtils::GetIcon("search"));
    connect(_ui->browseButton, &QPushButton::clicked, this, &LambdaUploadCodeDialog::HandleBrowse);

    // Set name
    _ui->nameEdit->setText(lambdaName);

    // Enable Drop Events for this widget
    setAcceptDrops(true);
}

LambdaUploadCodeDialog::~LambdaUploadCodeDialog() {
    delete _ui;
}

void LambdaUploadCodeDialog::dragEnterEvent(QDragEnterEvent *event) {
    // Check if the data being dragged contains file URLs
    if (event->mimeData()->hasUrls()) {
        // Accept the proposed action (copy, move, or link)
        event->acceptProposedAction();
        _ui->dropLabel->setStyleSheet("QLabel { color: #007bff; font-size: 16px; padding: 10px; border: 2px dashed #007bff; background-color: #e6f3ff; }");
    } else {
        // Reject the event if it's not file URLs
        event->ignore();
        _ui->dropLabel->setStyleSheet("QLabel { color: #333; font-size: 16px; padding: 10px; border: 2px dashed #999; }");
    }
}

void LambdaUploadCodeDialog::dropEvent(QDropEvent *event) {
    // Ensure the data is file URLs
    if (event->mimeData()->hasUrls()) {
        for (QList<QUrl> urls = event->mimeData()->urls(); const QUrl &url: urls) {
            // Check if the URL is a local file
            if (url.isLocalFile()) {
                // Get the local file
                QString localFile = url.toLocalFile();
                _fileInfo = QFileInfo(localFile);

                // Extract the version from the filename
                QString version = FileUtils::ExtractVersionFromFileName(_fileInfo.fileName());
                _ui->filenameEdit->setText(_fileInfo.fileName());
                _ui->versionEdit->setText(version);
            }
        }

        // Mark the event as accepted
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void LambdaUploadCodeDialog::HandleBrowse() {
    // Create a QFileDialog set to select existing files
    const QString filter = "All Files (*.*)";
    const auto defaultDir = Configuration::instance().GetValue<QString>("ui.default-directory", "/usr/local/awsmock-qt-ui");

    if (const QString filePath = QFileDialog::getOpenFileName(nullptr, "Open lambda code file", defaultDir, filter); !filePath.isEmpty()) {
        const QString version = FileUtils::ExtractVersionFromFileName(filePath);
        _ui->filenameEdit->setText(filePath);
        _ui->versionEdit->setText(version);
        _fileInfo = QFileInfo(filePath);
    }
}

void LambdaUploadCodeDialog::HandleAccept() {
    QFile file(_fileInfo.absoluteFilePath());
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << file.errorString();
        return;
    }

    const QByteArray binaryData = file.readAll();
    file.close();

    LambdaUploadRequest request;
    request.lambdaName = _lambdaName;
    request.lambdaArn = _lambdaArn;
    request.version = _ui->versionEdit->text();
    request.lambdaCode = binaryData.toBase64();
    request.archive = _fileInfo.fileName();
    _lambdaService->UploadLambdaCode(request);

    accept();
}


void LambdaUploadCodeDialog::HandleReject() {
    accept();
}
