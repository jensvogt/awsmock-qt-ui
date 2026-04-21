//
// Created by vogje01 on 11/9/25.
//

#ifndef AWSMOCK_QT_UI_SQS_MESSAGE_ADD_DIALOG_H
#define AWSMOCK_QT_UI_SQS_MESSAGE_ADD_DIALOG_H

// C++ includes
#include <utility>

// Qt includes
#include <QDialog>
#include <QLineEdit>
#include <QFileDialog>
#include <QFormLayout>

// AwsMOck includes
#include <utils/BaseDialog.h>
#include <utils/IconUtils.h>
#include <utils/AwsUtils.h>
#include <dto/sqs/SQSSendMessageRequest.h>
#include <dto/sqs/SQSSendMessageResponse.h>
#include <modules/sqs/SQSService.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class SQSMessageAddDialog;
}

QT_END_NAMESPACE

class SQSMessageAddDialog final : public BaseDialog {
    Q_OBJECT

public:
    explicit SQSMessageAddDialog(const QString &queueUrl, const QString &queueArn, QWidget *parent = nullptr);

    ~SQSMessageAddDialog() override;

    void HandleAccept();

    void HandleSendMessageSignal(const SQSSendMessageResponse &response);

    void HandleReject();

    void HandleBrowseButton() const;

    void HandleAddAttributeButton() const;

    void SetupRequest();

    QString GetMessageId() {
        return _messageId;
    }

    void LoadContent() override {
    };

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

    /**
     * @brief Queue ARN
     */
    QString _queueArn;

    /**
     * @brief QSQ message request
     */
    SQSSendMessageRequest _request;

    /**
     * @brief Message ID
     */
    QString _messageId;
};


#endif //AWSMOCK_QT_UI_SQS_MESSAGE_ADD_DIALOG_H
