//
// Created by vogje01 on 11/8/25.
//

#include <modules/application/ApplicationUploadCodeDialog.h>
#include "ui_ApplicationUploadCodeDialog.h"

ApplicationUploadCodeDialog::ApplicationUploadCodeDialog(const QString &name, QWidget *parent) : QDialog(parent), ui(new Ui::ApplicationUploadCodeDialog) {

    // Connect application service
    _applicationService = new ApplicationService();

    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &ApplicationUploadCodeDialog::HandleAccept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &ApplicationUploadCodeDialog::HandleReject);

    // Connect browse button
    connect(ui->browseButton, &QPushButton::clicked, this, &ApplicationUploadCodeDialog::HandleBrowse);

    // Set name
    ui->nameEdit->setText(name);

    // Enable Drop Events for this widget
    setAcceptDrops(true);
}

ApplicationUploadCodeDialog::~ApplicationUploadCodeDialog() {
    delete ui;
}

void ApplicationUploadCodeDialog::dragEnterEvent(QDragEnterEvent *event) {

    // Check if the data being dragged contains file URLs
    if (event->mimeData()->hasUrls()) {
        // Accept the proposed action (copy, move, or link)
        event->acceptProposedAction();
        ui->dropLabel->setStyleSheet("QLabel { color: #007bff; font-size: 16px; padding: 10px; border: 2px dashed #007bff; background-color: #e6f3ff; }");
    } else {
        // Reject the event if it's not file URLs
        event->ignore();
        ui->dropLabel->setStyleSheet("QLabel { color: #333; font-size: 16px; padding: 10px; border: 2px dashed #999; }");
    }
}

void ApplicationUploadCodeDialog::dropEvent(QDropEvent *event) {

    // Ensure the data is file URLs
    if (event->mimeData()->hasUrls()) {

        QList<QUrl> urls = event->mimeData()->urls();
        for (const QUrl &url: urls) {

            // Check if the URL is a local file
            if (url.isLocalFile()) {

                // Get the local file
                QString localFile = url.toLocalFile();
                _fileInfo = QFileInfo(localFile);

                // Extract the version from the filename
                QString version = FileUtils::ExtractVersionFromFileName(_fileInfo.fileName());
                ui->filenameEdit->setText(_fileInfo.fileName());
                ui->versionEdit->setText(version);
            }
        }

        // Mark the event as accepted
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void ApplicationUploadCodeDialog::HandleBrowse() {

    // Create a QFileDialog set to select existing files
    const QString filter = "All Files (*.*)";
    const QString defaultDir = Configuration::instance().GetDefaultDirectory();

    if (const QString filePath = QFileDialog::getOpenFileName(nullptr, "Open application code file", defaultDir, filter); !filePath.isEmpty()) {
        const QString version = FileUtils::ExtractVersionFromFileName(filePath);
        ui->filenameEdit->setText(filePath);
        ui->versionEdit->setText(version);
        _fileInfo = QFileInfo(filePath);
    }
}

void ApplicationUploadCodeDialog::HandleAccept() {

    QFile file(_fileInfo.absoluteFilePath());
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << file.errorString();
        return;
    }

    const QByteArray binaryData = file.readAll();
    file.close();

    ApplicationUploadRequest request;
    request.region = Configuration::instance().GetRegion();
    request.applicationName = ui->nameEdit->text();
    request.version = ui->versionEdit->text();
    request.applicationCode = binaryData.toBase64();
    request.archive = _fileInfo.fileName();
    _applicationService->UploadApplication(request);

    accept();
}


void ApplicationUploadCodeDialog::HandleReject() {
    accept();
}
