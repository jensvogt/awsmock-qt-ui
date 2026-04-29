//
// Created by jensv on 28/04/2026.
//

#ifndef AWSMOCK_QT_UI_SNS_SUBSCRIPTION_DIALOG_H
#define AWSMOCK_QT_UI_SNS_SUBSCRIPTION_DIALOG_H

// Qt includes
#include <qcompleter.h>
#include <QDialog>

// Awsmock includes
#include <modules/sns/SNSService.h>
#include <modules/sqs/SQSService.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class SNSSubscriptionDialog;
}

QT_END_NAMESPACE

class SNSSubscriptionDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor for a subscription
     *
     * @param topicArn topic ARN
     * @param parent parent widget
     */
    explicit SNSSubscriptionDialog(const QString &topicArn, QWidget *parent = nullptr);

    /**
     * @brief Constructor for an existing subscription
     *
     * @param topicArn topic ARN
     * @param subscriptionArn subscription ARN
     * @param parent parent widget
     */
    explicit SNSSubscriptionDialog(const QString &topicArn, const QString &subscriptionArn, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~SNSSubscriptionDialog() override;

    /**
     * Initialization
     */
    void Initialize();

    /**
     * @brief Handle OK button
     */
    void HandleAccept();

    /**
     * @brief Handle CANCEL button
     */
    void HandleReject();

private:
    /**
     * @brief Load SQS queue ARNs
     */
    void LoadSQSQueues();

    /**
     * @brief UI components
     */
    Ui::SNSSubscriptionDialog *_ui;

    /**
     * @brief Topic ARN
     */
    QString _topicArn;

    /**
     * @brief SNS REST service
     */
    SNSService *_snsService{};

    /**
     * @brief SQS REST service
     */
    SQSService *_sqsService{};

    /**
     * @brief Subscription response
     */
    SNSTopicSubscription _snsTopicSubscription{};
};


#endif // AWSMOCK_QT_UI_SNS_SUBSCRIPTION_DIALOG_H
