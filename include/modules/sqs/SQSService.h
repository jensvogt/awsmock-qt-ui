#ifndef SQS_SERVICE_H
#define SQS_SERVICE_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QObject>
#include <QTableWidget>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include <dto/sqs/SQSGetMessageDetailsResponse.h>
#include <dto/sqs/SQSGetQueueDetailsResponse.h>
#include <dto/sqs/UpdateQueueRequest.h>
#include <utils/BasePage.h>
#include <utils/Configuration.h>
#include <utils/RestManager.h>

class SQSService : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief SQSService
     */
    SQSService();

    /**
     * @brief List SQS Queues
     *
     * @param prefix Queuename prefix
     */
    void ListQueues(const QString &prefix, QTableWidget* tableWidget);

    /**
     * @brief HandleListResult
     *
     * @param replay Qt network manager reply
     */
    void HandleListResult(QNetworkReply *reply);

    /**
     * @brief Purge Queue
     *
     * @param QueueUrl Queue URL
     */
    void PurgeQueue(QString QueueUrl);

    /**
     * @brief HandlePurgeResult
     *
     * @param reply Qt network manager reply
     */
    void HandlePurgeResult(QNetworkReply *reply);

    /**
     * @brief Purge all Queues
     */
    void PurgeAllQueues();

    /**
     * @brief HandleAllPurgeResult
     *
     * @param reply Qt network manager reply
     */
    void HandleAllPurgeResult(QNetworkReply *reply);

    /**
     * @brief Add Queue
     */
    void AddQueue(const QString &QueueName);

    /**
     * @brief HandleAddResult
     *
     * @param reply Qt network manager reply
     */
    void HandleAddResult(QNetworkReply *reply);

    /**
     * @brief Redrive Queue
     *
     * @param QueueUrl Queue URL
     */
    void RedriveQueue(QString QueueUrl);

    /**
     * @brief HandleRedriveResult
     *
     * @param reply Qt network manager reply
     */
    void HandleRedriveResult(QNetworkReply *reply);

    /**
     * @brief GetQueueDetails Queue
     *
     * @param QueueArn Queue ARN
     */
    void GetQueueDetails(QString QueueArn);

    /**
     * @brief HandleGetQueueDetailResult
     *
     * @param reply Qt network manager reply
     */
    void HandleGetQueueDetailResult(QNetworkReply *reply);

    /**
     * @brief Update a Queue
     *
     * @param QueueArn Queue Arn
     */
    void UpdateQueue(const UpdateQueueRequest &updateQuueRequest);

    /**
     * @brief HandleUpdateResult
     *
     * @param reply Qt network manager reply
     */
    void HandleUpdateResult(QNetworkReply *reply);

    /**
     * @brief Delete Queue
     *
     * @param QueueUrl Queue URL
     */
    void DeleteQueue(const QString &QueueUrl);

    /**
     * @brief HandleDeleteResult
     *
     * @param reply Qt network manager reply
     */
    void HandleDeleteResult(QNetworkReply *reply);

    /**
     * @brief List SQS messages
     *
     * @param QueueArn ARN of the Queue
     * @param prefix message id prefix
     */
    void ListMessages(const QString &QueueArn, const QString &prefix, QTableWidget* tableWidget);

    /**
     * @brief HandleListResult
     *
     * @param replay Qt network manager reply
     */
    void HandleMessageListResult(QNetworkReply *reply);

    /**
     * @brief Purge all messages
     *
     * @param QueueUrl URL of the Queue to purge
     */
    void PurgeAllMesssages(const QString &QueueUrl);

    /**
     * @brief HandleAllPurgeResult
     *
     * @param reply Qt network manager reply
     */
    void HandleMessageAllPurgeResult(QNetworkReply *reply);

    /**
     * @brief Get message details response
     *
     * @param messageId message ID
     */
    void GetSqsMessageDetails(QString messageId);

signals:
    void LoadContent();
    void GetQueueDetailsSignal(const SQSGetQueueDetailsResponse &response);
    void GetSqsMessageDetailsSignal(const SQSGetMessageDetailsResponse &response);

private:

    /**
     * @brief Network manager
     */
    QNetworkAccessManager *m_netManager;

    /**
     * @brief HTTP REST manager
     */
    RestManager _restManager;

    /**
     * @brief Base URL
     */
    QUrl url;

    /**
     * @brief Target table widget
     */
    QTableWidget* tableWidget{};
};

#endif // SQS_SERVICE_H
