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

// AwsMock includes
#include <utils/BaseDialog.h>
#include <utils/IconUtils.h>
#include <modules/sns/SNSMessageList.h>
#include <dto/sns/SNSSendMessageRequest.h>
#include <dto/sns/SNSSendMessageResponse.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class SNSMessageAddDialog;
}

QT_END_NAMESPACE

class SNSMessageAddDialog final : public BaseDialog {
    Q_OBJECT

public:
    explicit SNSMessageAddDialog(const QString &topicArn, QWidget *parent = nullptr);

    ~SNSMessageAddDialog() override;

    void HandleAccept() const;

    void HandleSendMessageSignal(const SNSSendMessageResponse &response);

    void HandleReject();

    void HandleBrowseButton() const;

    void HandlePrettyButton(bool checked) const;

    void HandleAddAttributeButton() const;

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
};


#endif //AWSMOCK_QT_UI_SNS_MESSAGE_ADD_DIALOG_H
