#ifndef AWSMOCK_QT_UI_SNS_TOPIC_COUNTER_H
#define AWSMOCK_QT_UI_SNS_TOPIC_COUNTER_H

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
        retentionPeriod = jsonObject["retentionPeriod"].toInt();
        maxMessageSize = jsonObject["maxMessageSize"].toInt();
        messages = jsonObject["messages"].toInt();
        messagesSend = jsonObject["messagesSend"].toInt();
        messagesResend = jsonObject["messagesResend"].toInt();
        size = jsonObject["size"].toInt();
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODate);
    }
};


#endif // AWSMOCK_QT_UI_SNS_TOPIC_COUNTER_H
