
#include <modules/sqs/SQSQueueDetailsDialog.h>
#include "ui_SQSQueueDetailsDialog.h"

SQSQueueDetailsDialog::SQSQueueDetailsDialog(const QString &queueArn, QWidget *parent): QDialog(parent), ui(new Ui::SQSQueueDetailsDialog), queueArn(queueArn) {

    ui->setupUi(this);

    sqsService = new SQSService();

    sqsService->GetQueueDetails(queueArn);
    connect(sqsService, &SQSService::GetQueueDetailsSignal, this, &SQSQueueDetailsDialog::UpdateQueueDetails);
}

SQSQueueDetailsDialog::~SQSQueueDetailsDialog() {
    delete ui;
}

void SQSQueueDetailsDialog::UpdateQueueDetails(const SQSGetQueueDetailsResponse &response) {

    ui->queueNameEdit->setText(response.queueName);
    ui->queueArnEdit->setText(response.queueArn);
    ui->queueUrlEdit->setText(response.queueUrl);
    ui->dlqArnEdit->setText(response.dlqArn);
    ui->dlqMaxReceiveEdit->setText(QString::number(response.dlqMaxReceived));
    ui->visibilityEdit->setText(QString::number(response.visibilityTimeout));
    ui->delayEdit->setText(QString::number(response.delay));
    ui->maxMessageSizeEdit->setText(QString::number(response.maxMessageSize/1024));
    ui->availableEdit->setText(QString::number(response.available));
    ui->invisibleEdit->setText(QString::number(response.invisible));
    ui->delayedEdit->setText(QString::number(response.delayed));
    ui->retentionPeriodEdit->setText(QString::number(response.retentionPeriod));
    ui->messageCountEdit->setText(QString::number(response.messageCount));
    ui->messageSizeEdit->setText(QString::number(response.size/1024));
    ui->createdEdit->setText(response.created.toString());
    ui->modifiedEdit->setText(response.modified.toString());

    connect(ui->delayEdit, &QLineEdit::editingFinished, this, [&]() {this->changed=true;});
    connect(ui->retentionPeriodEdit, &QLineEdit::editingFinished, this, [&]() {this->changed=true;});
    connect(ui->visibilityEdit, &QLineEdit::editingFinished, this, [&]() {this->changed=true;});
}

void SQSQueueDetailsDialog::on_sqsQueueDetailsButtonBox_accepted()
{
    if(this->changed) {
        UpdateQueueRequest updateQueueRequest;
        updateQueueRequest.queueArn = queueArn;
        updateQueueRequest.delay = ui->delayEdit->text().toLong();
        updateQueueRequest.retentionPeriod = ui->retentionPeriodEdit->text().toLong();
        updateQueueRequest.visibilityTimeout = ui->visibilityEdit->text().toLong();
        sqsService->UpdateQueue(updateQueueRequest);
    }
    accept();
}


void SQSQueueDetailsDialog::on_sqsQueueDetailsButtonBox_rejected()
{
    accept();
}

