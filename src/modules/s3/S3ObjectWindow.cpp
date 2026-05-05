//
// Created by jensv on 05/05/2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SQSMessageWindow.h" resolved

#include <modules/s3/S3ObjectWindow.h>
#include "ui_S3ObjectWindow.h"

S3ObjectWindow::S3ObjectWindow(QString objectId, QWidget *parent) : BaseDialog(parent), _ui(new Ui::S3ObjectWindow), _objectId(std::move(objectId)) {

    // S3 service
    _s3Service = new S3Service;
    connect(_s3Service, &S3Service::GetObjectDetailsSignal, this, &S3ObjectWindow::UpdateContent);

    // Setup UI components
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &S3ObjectWindow::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &S3ObjectWindow::HandleReject);

    // Set button
    _ui->refreshButton->setText(nullptr);
    _ui->refreshButton->setIcon(IconUtils::GetIcon("refresh"));
    connect(_ui->refreshButton, &QPushButton::clicked, this, &S3ObjectWindow::LoadContent);

    // Status
    _ui->statusLabel->setText("Initialized");

    // Load content
    S3ObjectWindow::LoadContent();
}

S3ObjectWindow::~S3ObjectWindow() {
    delete _s3Service;
    delete _ui;
}

void S3ObjectWindow::LoadContent() {
    _s3Service->GetObjectDetails(_objectId);
}

void S3ObjectWindow::UpdateContent(const S3GetObjectDetailsResponse &response) const {

    if (response.contentType.startsWith("image")) {

        // Set text
        QPixmap pixmap;
        pixmap.loadFromData(response.body);
        _ui->stackedWidget->setCurrentIndex(1);
        _ui->imageViewer->LoadImage(pixmap);

    } else {

        // Set text
        _ui->stackedWidget->setCurrentIndex(0);
        _ui->textEditor->SetText(response.body);
    }

    // Status label
    SetLastUpdate();
}

void S3ObjectWindow::SetLastUpdate() const {
    const QString message = "Last update: " + DateTimeUtils::GetLogTimeFormat(QDateTime::currentDateTime());
    _ui->statusLabel->setText(message);
}

void S3ObjectWindow::HandleAccept() {
    accept();
}

void S3ObjectWindow::HandleReject() {
    reject();
}
