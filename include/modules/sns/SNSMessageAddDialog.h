//
// Created by vogje01 on 11/9/25.
//

#ifndef AWSMOCK_QT_UI_SNS_MESSAGE_ADD_DIALOG_H
#define AWSMOCK_QT_UI_SNS_MESSAGE_ADD_DIALOG_H

// C++ includes
#include <utility>

// Qt includes
#include <QDialog>
#include <QFileDialog>
#include <QFormLayout>
#include <QLineEdit>

// AwsMock includes
#include <dto/sns/SNSSendMessageRequest.h>
#include <dto/sns/SNSSendMessageResponse.h>
#include <dto/sqs/SQSListQueueDefaultAttribtesResponse.h>
#include <modules/sns/SNSService.h>
#include <utils/BaseDialog.h>
#include <utils/IconUtils.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class SNSMessageAddDialog;
}

QT_END_NAMESPACE

class SNSMessageAddDialog final : public BaseDialog {
    Q_OBJECT

public:
    explicit SNSMessageAddDialog(QString topicArn, QWidget *parent = nullptr);

    ~SNSMessageAddDialog() override;

    void HandleAccept();

    void HandleReject();

    void HandleSendMessageSignal(const SNSSendMessageResponse &response);

    void HandleBrowseButton() const;

    void HandlePrettyButton(bool checked) const;

    void HandleAddAttributeButton() const;

    void SetupRequest();

    QString getMessageId() {
        return _messageId;
    }
    
    void LoadContent() override {
    }

private:
    /**
     * @brief UI components
     */
    Ui::SNSMessageAddDialog *_ui;

    /**
     * @brief SNS REST service
     */
    SNSService *_snsService{};

    /**
     * @brief Queue URL
     */
    QString _topicArn;

    /**
     * @brief SNS message request
     */
    SNSSendMessageRequest _request;

    /**
     * @brief Message ID of the message being sent
     */
    QString _messageId;
};


#endif //AWSMOCK_QT_UI_SNS_MESSAGE_ADD_DIALOG_H
