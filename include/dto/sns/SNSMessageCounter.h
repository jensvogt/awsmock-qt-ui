#ifndef SNS_MESSAGE_COUNTER_H
#define SNS_MESSAGE_COUNTER_H

#include <QDateTime>
#include <QJsonObject>

struct SNSMessageCounter {

    QString topicArn;

    QString messageId;

    QString message;

    QString contentType;

    QString messageStatus;

    long size;

    QDateTime created;

    QDateTime modified;

    QDateTime lastSend;

    void FromJson(const QJsonObject &jsonObject) {
        topicArn = jsonObject["topicArn"].toString();
        messageId = jsonObject["messageId"].toString();
        message = jsonObject["message"].toString();
        messageStatus = jsonObject["messageStatus"].toString();
        contentType = jsonObject["contentType"].toString();
        size = jsonObject["size"].toInteger();
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODate);
        lastSend = QDateTime::fromString(jsonObject["lastSend"].toString(), Qt::ISODate);
    }
};

#endif // SNS_MESSAGE_COUNTER_H
