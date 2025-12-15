#ifndef AWSMOCK_QT_UI_SNS_MESSAGE_DETAILS_DIALOG_H
#define AWSMOCK_QT_UI_SNS_MESSAGE_DETAILS_DIALOG_H

#include <QDialog>
#include <QHeaderView>
#include <QStandardItemModel>

#include <modules/sns/SNSService.h>
#include <utils/IconUtils.h>

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

    /**
     *  @brief Table data model
     */
    QStandardItemModel *_dataModel;
};

#endif // AWSMOCK_QT_UI_SNS_MESSAGE_DETAILS_DIALOG_H
