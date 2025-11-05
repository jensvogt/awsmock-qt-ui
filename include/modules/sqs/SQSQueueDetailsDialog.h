#ifndef SQS_QUEUE_DETAILS_DIALOG_H
#define SQS_QUEUE_DETAILS_DIALOG_H

#include <QDialog>

#include <modules/sqs/SQSService.h>

namespace Ui {
class SQSQueueDetailsDialog;
}

class SQSQueueDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SQSQueueDetailsDialog(const QString &queueArn, QWidget *parent = nullptr);
    ~SQSQueueDetailsDialog();

private slots:
    void on_sqsQueueDetailsButtonBox_accepted();

    void on_sqsQueueDetailsButtonBox_rejected();

private:
    void UpdateQueueDetails(const SQSGetQueueDetailsResponse &response);

    Ui::SQSQueueDetailsDialog *ui;
    QString queueArn;
    SQSService* sqsService;
    bool changed=false;
};

#endif // SQS_QUEUE_DETAILS_DIALOG_H
