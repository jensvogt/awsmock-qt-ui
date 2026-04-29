//
// Created by jensv on 25/04/2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_S3QueueNotificationDialog.h" resolved

#include <modules/s3/S3QueueNotificationDialog.h>
#include "ui_S3QueueNotificationDialog.h"

S3QueueNotificationDialog::S3QueueNotificationDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::S3QueueNotificationDialog) {

    // SQS service
    _sqsService = new SQSService();

    // Setup UI
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &S3QueueNotificationDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &S3QueueNotificationDialog::HandleReject);

    // Fill SQS queue combo box (all)
    _ui->queueArnCombo->setEditable(true);
    _ui->queueArnCombo->setInsertPolicy(QComboBox::NoInsert);
    auto *completer = new QCompleter(_ui->queueArnCombo->model(), this);
    completer->setFilterMode(Qt::MatchContains);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    _ui->queueArnCombo->setCompleter(completer);
    _sqsService->ListQueueArns();
    connect(_sqsService, &SQSService::ListQueueArnsSignal, this, [this](const QList<QString> &response) {
        _ui->queueArnCombo->addItems(response);
        _selectedQueueArn = response[0];
    });
    connect(_ui->queueArnCombo, &QComboBox::currentTextChanged, this, [this](const QString &queueArn) {
        _selectedQueueArn = queueArn;
    });

    // Fill in S3 events
    _ui->eventSelectLists->SetAvailable(_s3Events);
    connect(_ui->eventSelectLists, &Awsmock::Components::SelectLists::SelectChangedSignal, this, [this](const QStringList &s3Events) {
        _selectedS3Event = s3Events;
    });
}

S3QueueNotificationDialog::~S3QueueNotificationDialog() {
    delete _sqsService;
    delete _ui;
}

void S3QueueNotificationDialog::HandleAccept() {
    if (_selectedQueueArn.isEmpty()) {
        QMessageBox::warning(this, tr("S3 Queue Notification Dialog"), tr("Queue ARN cannot be empty!"));
        return;
    }
    if (_selectedS3Event.isEmpty()) {
        QMessageBox::warning(this, tr("S3 Queue Notification Dialog"), tr("S3 event cannot be empty!"));
        return;
    }
    accept();
}

void S3QueueNotificationDialog::HandleReject() {
    reject();
}
