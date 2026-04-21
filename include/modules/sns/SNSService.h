#ifndef SNS_SERVICE_H
#define SNS_SERVICE_H

// Qt includes

#include <QElapsedTimer>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QObject>
#include <QUrlQuery>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

// AwsMOck include
#include <dto/sns/SNSGetMessageDetailsResponse.h>
#include <dto/sns/SNSGetTopicDetailsResponse.h>
#include <dto/sns/SNSListMessagesResult.h>
#include <dto/sns/SNSListQueueDefaultAttribtesResponse.h>
#include <dto/sns/SNSListTopicAttributesResponse.h>
#include <dto/sns/SNSListTopicResult.h>
#include <dto/sns/SNSListTopicSubscriptionsResponse.h>
#include <dto/sns/SNSListTopicTagsResponse.h>
#include <dto/sns/SNSSendMessageRequest.h>
#include <dto/sns/SNSSendMessageResponse.h>
#include <utils/BaseService.h>
#include <utils/Configuration.h>
#include <utils/EventBus.h>
#include <utils/Logging.h>
#include <utils/RestManager.h>

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
     * @param region
     * @param topicName name of the topic
     */
    void AddTopic(const QString &region, const QString &topicName);

    /**
     * @brief List SNS Topics
     *
     * @param prefix topic name prefix
     * @param pageSize
     * @param pageIndex
     * @param sortAttribute
     * @param sortDirection
     * @param pageSize
     * @param pageIndex
     * @param sortAttribute
     * @param sortDirection
     */
    void ListTopics(const QString &prefix, long pageSize, long pageIndex, const QString &sortAttribute, int sortDirection);

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
     * @param pageSize
     * @param pageIndex
     * @param sortAttribute
     * @param sortDirection
     * @param pageSize
     * @param pageIndex
     * @param sortAttribute
     * @param sortDirection
     */
    void ListMessages(const QString &topicArn, const QString &prefix, long pageSize, long pageIndex, const QString &sortAttribute, int sortDirection);

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

    void ResetMessageCounters();

    /**
     * @brief Get the default topic message attributes.
     *
     * @param topicArn AWS topic ARN
     * @param prefix name prefix
     */
    void ListQueueDefaultAttributes(const QString &topicArn, const QString &prefix);

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
    /**
     * @brief Signaled, when a topic arrived
     *
     * @param listTopicResult topic list response
     */
    void ListTopicSignal(const SNSListTopicResult &listTopicResult);

    /**
     * @brief Signaled, when a topic details arrived.
     *
     * @param response SNS topic details response
     */
    void GetTopicDetailsSignal(const SNSGetTopicDetailsResponse &response);

    /**
     * @brief Signaled, when topic attributes list arrived.
     *
     * @param listTopicAttributesResponse list topic attributes response
     */
    void ListTopicAttributesSignal(const ListTopicAttributesResponse &listTopicAttributesResponse);

    /**
     * @brief Signaled, when topic tag list arrived.
     *
     * @param listTopicTagsResponse list topic tag response
     */
    void ListTopicTagsSignal(const ListTopicTagsResponse &listTopicTagsResponse);

    /**
     * @brief Signaled, when topic subscription list arrived.
     *
     * @param listTopicSubscriptionResponse list topic subscriptions response
     */
    void ListTopicSubscriptionsSignal(const ListTopicSubscriptionsResponse &listTopicSubscriptionResponse);

    /**
     * @brief Signaled when a message list is available
     *
     * @param listMessagesResult list message response
     */
    void ListMessagesSignal(const SNSListMessagesResult &listMessagesResult);

    /**
     * @brief Signaled when a message details arrived
     *
     * @param response message details response
     */
    void GetMessageDetailsSignal(const SNSGetMessageDetailsResponse &response);

    /**
     * @brief Signaled when the message list should be reloaded
     */
    void ReloadMessagesSignal();

    /**
     * @brief Signaled when a message is sent
     *
     * @param response send message response
     */
    void SendMessagesSignal(const SNSSendMessageResponse &response);

    /**
     * @brief Signaled when new default attributes arrived
     *
     * @param listQueueDefaultAttributesResponse default attribute list response
     */
    void ListTopicDefaultAttributesSignal(const SNSListQueueDefaultAttributesResponse &listQueueDefaultAttributesResponse);

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
