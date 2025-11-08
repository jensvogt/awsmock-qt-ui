#ifndef SNS_MESSAGE_DETAILS_DIALOG_H
#define SNS_MESSAGE_DETAILS_DIALOG_H

#include <QDialog>

#include <modules/sns/SNSService.h>

namespace Ui {
    class SNSMessageDetailsDialog;
}

class SNSMessageDetailsDialog final : public QDialog {
    Q_OBJECT

public:
    explicit SNSMessageDetailsDialog(const QString &messageId, QWidget *parent = nullptr);

    ~SNSMessageDetailsDialog() override;

private slots:
    void on_prettyPushButton_toggled(bool checked) const;

private:
    void UpdateMessageDetails(const SNSGetMessageDetailsResponse &response) const;

    Ui::SNSMessageDetailsDialog *_ui;
    QString _messageId;
    SNSService *_snsService;
    bool _changed = false;
};

#endif // SNS_MESSAGE_DETAILS_DIALOG_H
