#ifndef AWSMOCK_QT_UI_SQS_MESSAGE_COUNTER_H
#define AWSMOCK_QT_UI_SQS_MESSAGE_COUNTER_H

#include <QDateTime>
#include <QJsonObject>

struct SQSMessageCounter {
    QString region;

    QString queueName;

    QString queueUrl;

    QString queueArn;

    QString messageId;

    QString message;

    QString contentType;

    QString messageStatus;

    QString receiptHandle;

    long size{};

    long retries{};

    QDateTime created;

    QDateTime modified;

    void FromJson(const QJsonObject &jsonObject) {
        region = jsonObject["region"].toString();
        queueName = jsonObject["queueName"].toString();
        queueUrl = jsonObject["queueUrl"].toString();
        queueArn = jsonObject["queueArn"].toString();
        messageId = jsonObject["messageId"].toString();
        message = jsonObject["message"].toString();
        messageStatus = jsonObject["messageStatus"].toString();
        receiptHandle = jsonObject["receiptHandle"].toString();
        contentType = jsonObject["contentType"].toString();
        size = jsonObject["size"].toInteger();
        retries = jsonObject["retries"].toInteger();
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODate);
    }
};

#endif // AWSMOCK_QT_UI_SQS_MESSAGE_COUNTER_H
