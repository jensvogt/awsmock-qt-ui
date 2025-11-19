#ifndef SQS_SERVICE_H
#define SQS_SERVICE_H

#include <QMessageBox>
#include <QObject>
#include <QTableWidget>

#include <dto/sqs/SQSGetMessageDetailsResponse.h>
#include <dto/sqs/SQSGetQueueDetailsResponse.h>
#include <dto/sqs/SQSQueueUpdateRequest.h>
#include <utils/BasePage.h>
#include <utils/Configuration.h>
#include <utils/RestManager.h>
#include <dto/sqs/SQSListQueueResponse.h>
#include <dto/sqs/SQSListMessagesResponse.h>
#include <dto/sqs/SQSListQueueAttributesResponse.h>
#include <dto/sqs/SQSListQueueLambdaTriggersResponse.h>
#include <dto/sqs/SQSSendMessageResponse.h>

#include "dto/sqs/SQSSendMessageRequest.h"

class SQSService final : public QObject {
    Q_OBJECT

public:
    /**
     * @brief SQSService
     */
    SQSService();

    /**
     * @brief List SQS Queues
     *
     * @param prefix Queue name prefix
     */
    void ListQueues(const QString &prefix);

    /**
     * @brief Purge Queue
     *
     * @param queueUrl Queue URL
     */
    void PurgeQueue(const QString &queueUrl);

    /**
     * @brief Purge all Queues
     */
    void PurgeAllQueues();

    /**
     * @brief Add Queue
     *
     * @param queueName name of the queue
     */
    void AddQueue(const QString &queueName);

    /**
     * @brief Redrive Queue
     *
     * @param queueArn Queue URL
     */
    void RedriveQueue(const QString &queueArn);

    /**
     * @brief GetQueueDetails Queue
     *
     * @param queueArn Queue ARN
     */
    void GetQueueDetails(const QString &queueArn);

    /**
     * @brief Update a Queue
     *
     * @param updateQueueRequest update request
     */
    void UpdateQueue(const SQSQueueUpdateRequest &updateQueueRequest);

    /**
     * @brief List SQS queue attributes
     *
     * @param queueArn ARN of the Queue
     * @param prefix attribute prefix
     */
    void ListQueueAttributes(const QString &queueArn, const QString &prefix);

    /**
     * @brief List SQS queue lambda triggers
     *
     * @param queueArn queue ARN
     * @param prefix uuid prefix
     */
    void ListQueueLambdaTriggers(const QString &queueArn, const QString &prefix);

    /**
     * @brief Delete Queue
     *
     * @param queueUrl Queue URL
     */
    void DeleteQueue(const QString &queueUrl);

    /**
     * @brief List SQS messages
     *
     * @param queueArn ARN of the Queue
     * @param prefix message id prefix
     */
    void ListMessages(const QString &queueArn, const QString &prefix);

    /**
     * @brief Purge all messages
     *
     * @param QueueUrl URL of the Queue to purge
     */
    void PurgeAllMessages(const QString &QueueUrl);

    /**
     * @brief Get message details response
     *
     * @param messageId message ID
     */
    void GetSqsMessageDetails(const QString &messageId);

    /**
     * @brief Send a new message to a SQS queue
     *
     * @param request send message request
     */
    void SendMessage(const SQSSendMessageRequest &request);

    /**
     * @brief Delete a message
     *
     * @param queueUrl message ID
     * @param receiptHandle
     */
    void DeleteMessage(const QString &queueUrl, const QString &receiptHandle);

signals:
    void ListQueuesSignal(const SQSQueueListResponse &listQueueResponse);

    void GetQueueDetailsSignal(const SQSGetQueueDetailsResponse &response);

    void ListQueueAttributesSignal(const SQSQueueAttributeListResponse &listQueueAttributeResponse);

    void ListQueueLambdaTriggersSignal(const SQSListQueueLambdaTriggersResponse &listQueueLambdaTriggersResponse);

    void ListMessagesSignal(const SQSListMessagesResponse &listMessagesResponse);

    void GetSqsMessageDetailsSignal(const SQSGetMessageDetailsResponse &response);

    void ReloadQueuesSignal();

    void ReloadMessagesSignal();

    void SendMessagesSignal(const SQSSendMessageResponse &response);

private:
    /**
     * @brief Base URL
     */
    QUrl url;

    /**
     * @brief HTTP REST manager
     */
    RestManager _restManager;

    /**
     * @brief Target table widget
     */
    QTableWidget *tableWidget{};
};

#endif // SQS_SERVICE_H
