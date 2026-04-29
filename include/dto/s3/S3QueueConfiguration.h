//
// Created by vogje01 on 2/21/26.
//

#ifndef AWSMOCK_QT_UI_S3_QUEUE_CONFIGURATION_H
#define AWSMOCK_QT_UI_S3_QUEUE_CONFIGURATION_H

// Awsmock includes
#include <dto/s3/S3FilterRule.h>
#include <dto/s3/S3NotificationEventType.h>

struct S3QueueConfiguration {

    /**
     * ID, optional, if empty, a random ID will be generated
     */
    QString id;

    /**
     * Queue ARN
     */
    QString queueArn;

    /**
     * S3 filter rules
     */
    QVector<S3FilterRule> filterRules;

    /**
     * Notification events
     */
    QVector<S3NotificationEventType> events;
};

#endif // AWSMOCK_QT_UI_S3_QUEUE_CONFIGURATION_H
