#include <modules/sns/SNSTopicDetailsDialog.h>
#include "ui_SNSTopicDetailsDialog.h"

SNSTopicDetailsDialog::SNSTopicDetailsDialog(const QString &topicArn, QWidget *parent): QDialog(parent), ui(new Ui::SNSTopicDetailsDialog), topicArn(topicArn) {

    ui->setupUi(this);

    snsService = new SNSService();

    snsService->GetTopicDetails(topicArn);
    connect(snsService, &SNSService::GetTopicDetailsSignal, this, &SNSTopicDetailsDialog::UpdateTopicDetails);
}

SNSTopicDetailsDialog::~SNSTopicDetailsDialog() {
    delete ui;
}

void SNSTopicDetailsDialog::UpdateTopicDetails(const SNSGetTopicDetailsResponse &response) const {

    ui->topicNameEdit->setText(response.topicName);
    ui->topicArnEdit->setText(response.topicArn);
    ui->messageCountEdit->setText(QString::number(response.messageCount));
    ui->messageSizeEdit->setText(QString::number(response.size/1024));
    ui->createdEdit->setText(response.created.toString());
    ui->modifiedEdit->setText(response.modified.toString());
}

void SNSTopicDetailsDialog::on_snsTopicDetailsButtonBox_accepted() const {
    if(this->changed) {
        /*UpdateQueueRequest updateQueueRequest;
            updateQueueRequest.queueArn = queueArn;
            updateQueueRequest.delay = ui->delayEdit->text().toLong();
            updateQueueRequest.retentionPeriod = ui->retentionPeriodEdit->text().toLong();
            updateQueueRequest.visibilityTimeout = ui->visibilityEdit->text().toLong();
            sqsService->UpdateQueue(updateQueueRequest);*/
    }
}

