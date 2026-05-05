//
// Created by jensv on 05/05/2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SQSMessageWindow.h" resolved

#include <modules/sns/SNSMessageWindow.h>
#include "ui_SNSMessageWindow.h"

SNSMessageWindow::SNSMessageWindow(const QString &messageId, QWidget *parent) : BaseDialog(parent), _ui(new Ui::SNSMessageWindow), _messageId(messageId) {

    // SNS service
    _snsService = new SNSService;
    connect(_snsService, &SNSService::GetMessageDetailsSignal, this, &SNSMessageWindow::UpdateContent);

    // Setup UI components
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &SNSMessageWindow::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &SNSMessageWindow::HandleReject);

    // Set button
    _ui->refreshButton->setText(nullptr);
    _ui->refreshButton->setIcon(IconUtils::GetIcon("refresh"));
    connect(_ui->refreshButton, &QPushButton::clicked, this, &SNSMessageWindow::LoadContent);

    // Status
    _ui->statusLabel->setText("Initialized");

    // Load content
    SNSMessageWindow::LoadContent();
}

void SNSMessageWindow::LoadContent() {
    _snsService->GetSnsMessageDetails(_messageId);
}

void SNSMessageWindow::SetLastUpdate() const {
    const QString message = "Last update: " + DateTimeUtils::GetLogTimeFormat(QDateTime::currentDateTime());
    _ui->statusLabel->setText(message);
}

void SNSMessageWindow::UpdateContent(const SNSGetMessageDetailsResponse &response) const {

    // Set test
    _ui->textEditor->SetText(response.message);

    // Status label
    SetLastUpdate();
}

SNSMessageWindow::~SNSMessageWindow() {
    delete _snsService;
    delete _ui;
}

void SNSMessageWindow::HandleAccept() {
    accept();
}

void SNSMessageWindow::HandleReject() {
    reject();
}
