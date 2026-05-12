#ifndef AWSMOCK_QT_UI_SQS_SERVICE_H
#define AWSMOCK_QT_UI_SQS_SERVICE_H

// Qt includes
#include <QElapsedTimer>
#include <QMessageBox>
#include <QObject>
#include <QTableWidget>

// AwsMock include
#include <dto/sns/SNSSendMessageResponse.h>
#include <dto/sqs/SQSGetMessageDetailsResponse.h>
#include <dto/sqs/SQSGetQueueDetailsResponse.h>
#include <dto/sqs/SQSListMessagesResponse.h>
#include <dto/sqs/SQSListQueueAttributesResponse.h>
#include <dto/sqs/SQSListQueueDefaultAttribtesResponse.h>
#include <dto/sqs/SQSListQueueDefaultAttribtesResponse.h>
#include <dto/sqs/SQSListQueueLambdaTriggersResponse.h>
#include <dto/sqs/SQSListQueueResponse.h>
#include <dto/sqs/SQSListQueueTagsResponse.h>
#include <dto/sqs/SQSQueueUpdateRequest.h>
#include <dto/sqs/SQSSendMessageRequest.h>
#include <dto/sqs/SQSSendMessageResponse.h>
#include <utils/BasePage.h>
#include <utils/BaseService.h>
#include <utils/Configuration.h>
#include <utils/EventBus.h>
#include <utils/Logging.h>
#include <utils/RestManager.h>

class SQSService final : public BaseService {
    Q_OBJECT

public:
    /**
     * @brief SQSService
     */
    SQSService() = default;

    /**
     * @brief List SQS Queues
     *
     * @param prefix queue name prefix
     * @param pageSize page size
     * @param pageIndex page index
     * @param sortColumn sort column
     * @param sortDirection sort direction
     */
    void ListQueues(const QString &prefix, long pageSize, long pageIndex, const QString &sortColumn, int sortDirection);

    /**
     * @brief List SQS Queues ARNs
     */
    void ListQueueArns();

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
     * @brief Rest all message counters
     */
    void ResetMessageCounters();

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
     * @brief List default attributes
     *
     * @param queueArn queue ARN
     * @param prefix queue ARN
     */
    void ListQueueDefaultAttributes(const QString &queueArn, const QString &prefix);

    /**
     * @brief Adds a new SQS queue default attribute
     * @param queueArn queue ARN
     * @param key attribute key
     * @param attribute
     */
    void AddQueueDefaultAttributes(const QString &queueArn, const QString &key, const MessageAttribute &attribute);

    void UpdateQueueDefaultAttributes(const QString &queueArn, const QString &key, const QString &value, const QString &dataType);

    void DeleteQueueDefaultAttributes(const QString &queueArn, const QString &key);

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
     * @param pageSize
     * @param pageIndex
     * @param sortAttribute
     * @param sortDirection
     * @param pageSize
     * @param pageIndex
     * @param sortAttribute
     * @param sortDirection
     */
    void ListMessages(const QString &queueArn, const QString &prefix, long pageSize, long pageIndex, const QString &sortAttribute, int sortDirection);

    /**
     * @brief Purge all messages
     *
     * @param queueUrl URL of the Queue to purge
     */
    void PurgeAllMessages(const QString &queueUrl);

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
     * @brief Resends a message, which is still in the queue.
     *
     * @param queueArn SQS queue ARN
     * @param messageId message ID
     */
    void ResendMessage(const QString &queueArn, const QString &messageId);

    /**
     * @brief Redrive message
     *
     * @param queueArn queue ARN
     * @param messageId message ID
     */
    void RedriveMessage(const QString &queueArn, const QString &messageId);

    /**
     * @brief Delete a message
     *
     * @param queueUrl message ID
     * @param receiptHandle
     */
    void DeleteMessage(const QString &queueUrl, const QString &receiptHandle);

    /**
     * @brief List all queue tags
     *
     * @param queueUrl queue URL
     * @param prefix tags key prefix
     */
    void ListQueueTags(const QString &queueUrl, const QString &prefix);

    /**
     * @brief Add a queue tag
     *
     * @param queueUrl queue URL
     * @param key tag name
     * @param value tag value
     */
    void TagQueue(const QString &queueUrl, const QString &key, const QString &value);

signals:
    /**
     * @brief Signal is send, when a list queue message arrived.
     *
     * @param listQueueResponse list queue response
     */
    void ListQueuesSignal(const SQSQueueListResponse &listQueueResponse);

    /**
     * @brief Signal is send, when a list of queue ARNs arrived.
     *
     * @param arns string list queue ARNs
     */
    void ListQueueArnsSignal(const QList<QString> &arns);

    /**
     * @brief Signal is send, when a queue details message arrived.
     *
     * @param response queue details response
     */
    void GetQueueDetailsSignal(const SQSGetQueueDetailsResponse &response);

    /**
     * @brief Signal is send, when a list queue attributes message arrived.
     *
     * @param listQueueAttributeResponse list queue attributes response
     */
    void ListQueueAttributesSignal(const SQSQueueAttributeListResponse &listQueueAttributeResponse);

    /**
     * @brief Signaled when new trigger arrived
     *
     * @param listQueueLambdaTriggersResponse trigger list response
     */
    void ListQueueLambdaTriggersSignal(const SQSListQueueLambdaTriggersResponse &listQueueLambdaTriggersResponse);

    /**
     * @brief Signaled when new default attributes arrived
     *
     * @param listQueueDefaultAttributesResponse default attribute list response
     */
    void ListQueueDefaultAttributesSignal(const SQSListQueueDefaultAttributesResponse &listQueueDefaultAttributesResponse);

    /**
     * @brief Signaled when new default attributes should be reloaded
     */
    void ReloadQueueDefaultAttributesSignal();

    /**
     * @brief Signaled when new messages are available
     *
     * @param listMessagesResponse message list response
     */
    void ListMessagesSignal(const SQSListMessagesResponse &listMessagesResponse);

    /**
     * @brief Signaled when messages details are available
     *
     * @param response message response
     */
    void GetSqsMessageDetailsSignal(const SQSGetMessageDetailsResponse &response);

    /**
     * @brief Signaled when the queue list can be reloaded
     */
    void ReloadQueuesSignal();

    /**
     * @brief Signaled when the message list can be reloaded
     */
    void ReloadMessagesSignal();

    /**
     * @brief Send when new messages are available
     *
     * @param response message response
     */
    void SendMessagesSignal(const SQSSendMessageResponse &response);

    /**
     * @brief Sent when the tags list arrived
     */
    void ListQueueTagsSignal(const SQSListQueueTagsResponse &response);

    /**
     * @brief Sent when the tags list should be updated
     */
    void ReloadQueueTagsSignal();

private:
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
