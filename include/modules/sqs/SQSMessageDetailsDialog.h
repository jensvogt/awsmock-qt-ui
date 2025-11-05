#ifndef SQS_MESSAGE_DETAILS_DIALOG_H
#define SQS_MESSAGE_DETAILS_DIALOG_H

#include <QDialog>

#include <modules/sqs/SQSService.h>

namespace Ui {
class SQSMessageDetailsDialog;
}

class SQSMessageDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SQSMessageDetailsDialog(const QString &messageId, QWidget *parent = nullptr);
    ~SQSMessageDetailsDialog();

private slots:

    void on_prettyPushButton_toggled(bool checked);

private:
    void UpdateMessageDetails(const SQSGetMessageDetailsResponse &response);

    Ui::SQSMessageDetailsDialog *_ui;
    QString _messageId;
    SQSService* _sqsService;
    bool _changed=false;
};

#endif // SQS_MESSAGE_DETAILS_DIALOG_H
