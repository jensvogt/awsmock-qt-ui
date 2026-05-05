//
// Created by jensv on 05/05/2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SQSMessageWindow.h" resolved

#include <modules/sqs/SQSMessageWindow.h>
#include "ui_SQSMessageWindow.h"


SQSMessageWindow::SQSMessageWindow(const QString &messageId, QWidget *parent) : BaseDialog(parent), _ui(new Ui::SQSMessageWindow), _messageId(messageId) {

    // SQSservice
    _sqsService = new SQSService;
    connect(_sqsService, &SQSService::GetSqsMessageDetailsSignal, this, &SQSMessageWindow::UpdateContent);

    // Setup UI components
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &SQSMessageWindow::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &SQSMessageWindow::HandleReject);

    // Set button
    _ui->refreshButton->setText(nullptr);
    _ui->refreshButton->setIcon(IconUtils::GetIcon("refresh"));
    connect(_ui->refreshButton, &QPushButton::clicked, this, &SQSMessageWindow::LoadContent);

    // Status
    _ui->statusLabel->setText("Initialized");

    // Load content
    SQSMessageWindow::LoadContent();
}

void SQSMessageWindow::LoadContent() {
    _sqsService->GetSqsMessageDetails(_messageId);
}

void SQSMessageWindow::SetLastUpdate() const {
    const QString message = "Last update: " + DateTimeUtils::GetLogTimeFormat(QDateTime::currentDateTime());
    _ui->statusLabel->setText(message);
}

void SQSMessageWindow::UpdateContent(const SQSGetMessageDetailsResponse &response) const {

    // Set test
    _ui->textEditor->SetText(response.body);

    // Status label
    SetLastUpdate();
}

SQSMessageWindow::~SQSMessageWindow() {
    delete _sqsService;
    delete _ui;
}

void SQSMessageWindow::HandleAccept() {
    accept();
}

void SQSMessageWindow::HandleReject() {
    reject();
}
