#ifndef SNS_TOPIC_DETAILS_DIALOG_H
#define SNS_TOPIC_DETAILS_DIALOG_H

#include <QDialog>

#include <modules/sns/SNSService.h>

namespace Ui {
class SNSTopicDetailsDialog;
}

class SNSTopicDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SNSTopicDetailsDialog(const QString &topicArn, QWidget *parent = nullptr);
    ~SNSTopicDetailsDialog() override;

private slots:
    void on_snsTopicDetailsButtonBox_accepted() const;

private:
    void UpdateTopicDetails(const SNSGetTopicDetailsResponse &response) const;

    Ui::SNSTopicDetailsDialog *ui;
    QString topicArn;
    SNSService* snsService;
    bool changed = false;
};

#endif // SNS_TOPIC_DETAILS_DIALOG_H
