#ifndef SNS_SERVICE_H
#define SNS_SERVICE_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include <dto/sns/SNSGetMessageDetailsResponse.h>
#include <dto/sns/SNSGetTopicDetailsResponse.h>
#include <dto/sns/SNSListMessagesResult.h>
#include <dto/sns/SNSListTopicResult.h>
#include <utils/Configuration.h>
#include <utils/RestManager.h>

class SNSService : public QObject {
    Q_OBJECT

public:
    /**
     * @brief SNSService
     */
    SNSService();

    /**
     * @brief Add topic
     *
     * @param region AWS region
     * @param topicName name of the topic
     */
    void AddTopic(const QString &region, const QString &topicName);

    /**
     * @brief List SNS Topics
     *
     * @param prefix topic name prefix
     */
    void ListTopics(const QString &prefix);

    /**
     * @brief List SNS messages
     *
     * @param topicArn topic ARN
     * @param prefix topic name prefix
     */
    void ListMessages(const QString &topicArn, const QString &prefix);

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
     * @brief Delete SNS message
     *
     * @param topicArn topic ARN
     * @param messageId message ID
     */
    void DeleteMessage(const QString &topicArn, const QString &messageId);

signals:
    void ListTopicSignal(const SNSListTopicResult &listTopicResult);

    void GetTopicDetailsSignal(const SNSGetTopicDetailsResponse &response);

    void ListMessagesSignal(const SNSListMessagesResult &listMessagesResult);

    void GetMessageDetailsSignal(const SNSGetMessageDetailsResponse &response);

    void ReloadMessagesSignal();

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
