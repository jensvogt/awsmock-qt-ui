#ifndef SNSTOPICCOUNTER_H
#define SNSTOPICCOUNTER_H

#include <QDateTime>
#include <QJsonObject>

struct SNSTopicCounter {

    QString topicArn;

    QString topicName;

    long retentionPeriod;

    long maxMessageSize;

    long messages;

    long messagesSend;

    long messagesResend;

    long size;

    QDateTime created;

    QDateTime modified;

    void FromJson(const QJsonObject &jsonObject) {
        topicArn = jsonObject["topicArn"].toString();
        topicName = jsonObject["topicName"].toString();
        retentionPeriod = jsonObject["retentionPeriod"].toInteger();
        maxMessageSize = jsonObject["maxMessageSize"].toInteger();
        messages = jsonObject["messages"].toInteger();
        messagesSend = jsonObject["messagesSend"].toInteger();
        messagesResend = jsonObject["messagesResend"].toInteger();
        size = jsonObject["size"].toInteger();
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODate);
    }
};


#endif // SNSTOPICCOUNTER_H
