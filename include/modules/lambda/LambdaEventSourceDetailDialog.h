//
// Created by vogje01 on 11/25/25.
//

#ifndef AWSMOCK_QT_UI_LAMBDA_EVENT_SOURCE_DETAIL_DIALOG_H
#define AWSMOCK_QT_UI_LAMBDA_EVENT_SOURCE_DETAIL_DIALOG_H

#include <QDialog>

// Awsmock includes
#include <modules/s3/S3Service.h>
#include <modules/sns/SNSService.h>
#include <modules/sqs/SQSService.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class LambdaEventSourceDetailDialog;
}

QT_END_NAMESPACE

class LambdaEventSourceDetailDialog final : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Event source edit dialog constructor
     *
     * @param type event source type, e.g. 'SQS', 'S3', 'SNS'
     * @param eventSourceArn ARN of the event source
     * @param batchSize batch size
     * @param maximumBatchingWindowInSeconds maximum batching window in seconds
     * @param uuid event source mapping UUID
     * @param parent parent widget
     */
    LambdaEventSourceDetailDialog(const QString &type, const QString &eventSourceArn, long batchSize, long maximumBatchingWindowInSeconds, QString uuid, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~LambdaEventSourceDetailDialog() override;

    /**
     * @brief Accept dialog
     */
    void HandleAccept();

    /**
     * @brief Reject dialog
     */
    void HandleReject();

    /**
     * @brief Event source type
     */
    QString GetType();

    /**
     * @brief Event source ARN
     */
    QString GetEventSourceArn();

    /**
     * @brief Batch size
     */
    long GetBatchSize();

    /**
     * @brief Maximum batching window in seconds
     */
    long GetMaximumBatchingWindowInSeconds();

    /**
     * @brief Event source mapping UUID
     */
    QString GetUuid();

private:
    /**
     * @brief Fetch the list of event source ARNs matching the given type and fill the ARN combo box
     *
     * @param type event source type, e.g. 'SQS', 'S3', 'SNS'
     */
    void PopulateEventSourceArns(const QString &type);

    /**
     * @brief UI component
     */
    Ui::LambdaEventSourceDetailDialog *_ui;

    /**
     * @brief S3 REST service, used to fetch bucket ARNs
     */
    S3Service *_s3Service;

    /**
     * @brief SQS REST service, used to fetch queue ARNs
     */
    SQSService *_sqsService;

    /**
     * @brief SNS REST service, used to fetch topic ARNs
     */
    SNSService *_snsService;

    /**
     * @brief Event source type
     */
    QString _type;

    /**
     * @brief Event source ARN
     */
    QString _eventSourceArn;

    /**
     * @brief Batch size
     */
    long _batchSize;

    /**
     * @brief Maximum batching window in seconds
     */
    long _maximumBatchingWindowInSeconds;

    /**
     * @brief Event source mapping UUID
     */
    QString _uuid;
};


#endif //AWSMOCK_QT_UI_LAMBDA_EVENT_SOURCE_DETAIL_DIALOG_H
