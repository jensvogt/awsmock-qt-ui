//
// Created by vogje01 on 2/21/26.
//

#ifndef AWSMOCK_QT_UI_PUT_BUCKET_NOTIFICATION_REQUEST_H
#define AWSMOCK_QT_UI_PUT_BUCKET_NOTIFICATION_REQUEST_H

// Qt includes
#include <QList>
#include <QString>

// Awsmock includes
#include <dto/s3/S3TopicConfiguration.h>
#include <dto/s3/S3QueueConfiguration.h>
#include <dto/s3/S3LambdaConfiguration.h>

struct PutBucketNotificationRequest {

    /**
     * @brief Bucket
     */
    QString bucket;

    /**
     * @brief SNS topic configurations
     */
    QVector<S3TopicConfiguration> topicConfigurations;

    /**
     * @brief SQS queue configurations
     */
    QVector<S3QueueConfiguration> queueConfigurations;

    /**
     * @brief Lambda function configurations
     */
    QVector<S3LambdaConfiguration> lambdaConfigurations;

};

#endif //AWSMOCK_QT_UI_PUT_BUCKET_NOTIFICATION_REQUEST_H
