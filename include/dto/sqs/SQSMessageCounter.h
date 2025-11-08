#ifndef AWSMOCK_QT_UI_SQS_MESSAGE_COUNTER_H
#define AWSMOCK_QT_UI_SQS_MESSAGE_COUNTER_H

#include <QDateTime>
#include <QJsonObject>

struct SQSMessageCounter {
    QString queueArn;

    QString messageId;

    QString message;

    QString contentType;

    QString messageStatus;

    long size;

    long retries;

    QDateTime created;

    QDateTime modified;

    void FromJson(const QJsonObject &jsonObject) {
        queueArn = jsonObject["queueArn"].toString();
        messageId = jsonObject["messageId"].toString();
        message = jsonObject["message"].toString();
        messageStatus = jsonObject["messageStatus"].toString();
        contentType = jsonObject["contentType"].toString();
        size = jsonObject["size"].toInteger();
        retries = jsonObject["retries"].toInteger();
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODate);
    }
};

#endif // AWSMOCK_QT_UI_SQS_MESSAGE_COUNTER_H
