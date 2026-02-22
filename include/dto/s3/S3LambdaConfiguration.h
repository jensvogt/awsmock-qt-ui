//
// Created by vogje01 on 2/21/26.
//

#ifndef AWSMOCK_QT_UI_S3_LAMBDA_CONFIGURATION_H
#define AWSMOCK_QT_UI_S3_LAMBDA_CONFIGURATION_H

// Awsmock includes
#include <qxmlstream.h>
#include <dto/s3/S3FilterRule.h>

enum class S3NotificationEventType;

struct S3LambdaConfiguration {

    /**
    * ID, optional, if empty, a random ID will be generated
    */
    QString id;

    /**
     * Lambda ARN
     */
    QString lambdaArn;

    /**
     * S3 filter rules
     */
    QVector<S3FilterRule> filterRules;

    /**
     * Notification events
     */
    QVector<S3NotificationEventType> events;
};

#endif //AWSMOCK_QT_UI_S3_LAMBDA_CONFIGURATION_H
