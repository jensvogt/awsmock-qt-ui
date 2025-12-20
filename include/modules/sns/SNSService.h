#ifndef SNS_SERVICE_H
#define SNS_SERVICE_H

// Qt includes
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QElapsedTimer>
#include <QUrlQuery>

// AwsMOck include
#include <utils/Configuration.h>
#include <utils/RestManager.h>
#include <utils/EventBus.h>
#include <utils/BaseService.h>
#include <dto/sns/SNSGetMessageDetailsResponse.h>
#include <dto/sns/SNSGetTopicDetailsResponse.h>
#include <dto/sns/SNSListMessagesResult.h>
#include <dto/sns/SNSListTopicResult.h>
#include <dto/sns/SNSSendMessageResponse.h>
#include <dto/sns/SNSSendMessageRequest.h>
#include <dto/sns/SNSListTopicAttributeCountersResponse.h>

class SNSService : public BaseService {
    Q_OBJECT

public:
    /**
     * @brief SNSService
     */
    SNSService() = default;

    /**
     * @brief Add topic
     *
     * @param topicName name of the topic
     */
    void AddTopic(const QString &topicName);

    /**
     * @brief List SNS Topics
     *
     * @param prefix topic name prefix
     */
    void ListTopics(const QString &prefix);

    /**
     * @brief Purge all messages
     *
     * @param topicArn ARN of the parent topic
     */
    void PurgeTopic(const QString &topicArn);

    /**
     * @brief Purge all topics
     */
    void PurgeAllTopics();

    /**
     * @brief List SNS topic attributes
     *
     * @param topicArn topic AWS ARN
     */
    void ListTopicAttributes(const QString &topicArn);

    /**
     * @brief List SNS topic tags
     *
     * @param topicArn topic AWS ARN
     */
    void ListTopicTags(const QString &topicArn);

    /**
     * @brief List SNS topic subscriptions
     *
     * @param topicArn topic AWS ARN
     */
    void ListTopicSubscriptions(const QString &topicArn);

    /**
     * @brief List SNS messages
     *
     * @param topicArn topic ARN
     * @param prefix topic name prefix
     */
    void ListMessages(const QString &topicArn, const QString &prefix);

    /**
     * @brief Purge all messages of a topic
     *
     * @param topicArn topic ARN
     */
    void PurgeMessages(const QString &topicArn);

    /**
     * @brief Get topic details
     *
     * @param topicArn topic ARN
     */
    void GetTopicDetails(const QString &topicArn);

    /**
     * @brief Get message details response
     *
     * @param messageId message ID
     */
    void GetSnsMessageDetails(const QString &messageId);

    /**
     * @brief Delete topic
     *
     * @param topicArn topic ARN
     */
    void DeleteTopic(const QString &topicArn);

    /**
     * @brief Send an SNS message
     *
     * @param request send message request
     */
    void SendMessage(const SNSSendMessageRequest &request);

    /**
     * @brief Delete SNS message
     *
     * @param topicArn topic ARN
     * @param messageId message ID
     */
    void DeleteMessage(const QString &topicArn, const QString &messageId);

signals:
    void ListTopicSignal(const SNSListTopicResult &listTopicResult);

    void GetTopicDetailsSignal(const SNSGetTopicDetailsResponse &response);

    /**
     * @brief Signaled, when topic attributes list arrived.
     *
     * @param listTopicAttributesResponse list topic attributes response
     */
    void ListTopicAttributesSignal(const ListTopicAttributesCountersResponse &listTopicAttributesResponse);

    void ListMessagesSignal(const SNSListMessagesResult &listMessagesResult);

    void GetMessageDetailsSignal(const SNSGetMessageDetailsResponse &response);

    void ReloadMessagesSignal();

    void SendMessagesSignal(const SNSSendMessageResponse &response);

private:
    /**
     * @brief HTTP REST manager
     */
    RestManager _restManager;

    /**
     * @brief Base URL
     */
    QUrl url;
};


#endif // SNS_SERVICE_H
