#ifndef GET_QUEUE_DETAILS_RESPONSE_H
#define GET_QUEUE_DETAILS_RESPONSE_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

struct SQSGetQueueDetailsResponse {
    QString queueName;

    QString queueUrl;

    QString queueArn;

    long messageCount;

    long retentionPeriod;

    long maxMessageSize;

    long visibilityTimeout;

    long size;

    long delay;

    QString owner;

    long available;

    long invisible;

    long delayed;

    QString dlqArn;

    long dlqMaxReceived;

    QDateTime created;

    QDateTime modified;

    void FromJson(const QJsonDocument &jsonDoc) {
        queueName = jsonDoc["queueName"].toString();
        queueUrl = jsonDoc["queueUrl"].toString();
        queueArn = jsonDoc["queueArn"].toString();
        messageCount = jsonDoc["messageCount"].toInteger();
        retentionPeriod = jsonDoc["retentionPeriod"].toInteger();
        maxMessageSize = jsonDoc["maxMessageSize"].toInteger();
        visibilityTimeout = jsonDoc["visibilityTimeout"].toInteger();
        size = jsonDoc["size"].toInteger();
        delay = jsonDoc["delay"].toInteger();
        owner = jsonDoc["delay"].toString();
        available = jsonDoc["available"].toInteger();
        invisible = jsonDoc["invisible"].toInteger();
        delayed = jsonDoc["delayed"].toInteger();
        delayed = jsonDoc["delayed"].toInteger();
        dlqArn = jsonDoc["dlqArn"].toString();
        dlqMaxReceived = jsonDoc["dlqMaxReceive"].toInteger();
        created = QDateTime::fromString(jsonDoc["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonDoc["modified"].toString(), Qt::ISODate);
    }
};

#endif // GET_QUEUE_DETAILS_RESPONSE_H
