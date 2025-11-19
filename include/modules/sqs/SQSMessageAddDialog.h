//
// Created by vogje01 on 11/9/25.
//

#ifndef AWSMOCK_QT_UI_SQS_MESSAGE_ADD_DIALOG_H
#define AWSMOCK_QT_UI_SQS_MESSAGE_ADD_DIALOG_H

#include <utility>

#include <QDialog>
#include <QFileDialog>

#include <utils/BaseDialog.h>
#include <utils/IconUtils.h>
#include <modules/sqs/SQSMessageList.h>
#include <dto/sqs/SQSSendMessageRequest.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class SQSMessageAddDialog;
}

QT_END_NAMESPACE

class SQSMessageAddDialog final : public BaseDialog {
    Q_OBJECT

public:
    explicit SQSMessageAddDialog(QString queueUrl, QWidget *parent = nullptr);

    ~SQSMessageAddDialog() override;

    void HandleAccept() const;

    void HandleSendMessageSignal(const SQSSendMessageResponse &response);

    void HandleReject();

    void HandleBrowseButton() const;

    void HandlePrettyButton(bool checked) const;

    void HandleAddAttributeButton() const;

private:
    /**
     * @brief UI components
     */
    Ui::SQSMessageAddDialog *_ui;

    /**
     * @brief SQS REST service
     */
    SQSService *_sqsService{};

    /**
     * @brief Queue URL
     */
    QString _queueUrl;
};


#endif //AWSMOCK_QT_UI_SQS_MESSAGE_ADD_DIALOG_H
