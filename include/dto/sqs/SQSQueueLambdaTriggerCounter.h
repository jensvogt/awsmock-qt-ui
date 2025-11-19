#ifndef AWSMOCK_QT_UI_SQS_QUEUE_LAMBDA_TRIGGER_COUNTER_H
#define AWSMOCK_QT_UI_SQS_QUEUE_LAMBDA_TRIGGER_COUNTER_H

#include <QDateTime>
#include <QJsonObject>

struct SQSQueueLambdaTriggerCounter {
    QString uuid;

    QString arn;

    bool enabled = false;

    void FromJson(const QJsonObject &jsonObject) {
        uuid = jsonObject["uuid"].toString();
        arn = jsonObject["arn"].toString();
        enabled = jsonObject["enabled"].toBool();
    }
};


#endif // AWSMOCK_QT_UI_SQS_QUEUE_LAMBDA_TRIGGER_COUNTER_H
