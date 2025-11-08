#ifndef AWSMOCK_QT_UI_SQS_QUEUE_COUNTER_H
#define AWSMOCK_QT_UI_SQS_QUEUE_COUNTER_H

#include <QDateTime>
#include <QJsonObject>

struct SQSQueueCounter {

    QString queueArn;

    QString queueName;

    QString queueUrl;

    long available;

    long invisible;

    long delayed;

    long size;

    bool isDlq{};

    QDateTime created;

    QDateTime modified;

    void FromJson(const QJsonObject &jsonObject) {
        queueArn = jsonObject["queueArn"].toString();
        queueName = jsonObject["queueName"].toString();
        queueUrl = jsonObject["queueUrl"].toString();
        available = jsonObject["available"].toInteger();
        invisible = jsonObject["invisible"].toInteger();
        delayed = jsonObject["delayed"].toInteger();
        size = jsonObject["size"].toInteger();
        isDlq = jsonObject["isDlq"].toBool();
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODate);
    }
};


#endif // AWSMOCK_QT_UI_SQS_QUEUE_COUNTER_H
