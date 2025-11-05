#ifndef GET_TOPIC_DETAILS_RESPONSE_H
#define GET_TOPIC_DETAILS_RESPONSE_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

struct SNSGetTopicDetailsResponse {

public:

    QString topicName;

    QString topicArn;

    long messageCount;

    long size;

    QString owner;

    QDateTime created;

    QDateTime modified;

    void FromJson(QJsonDocument jsonDoc) {
        topicName = jsonDoc["topicName"].toString();
        topicArn = jsonDoc["topicArn"].toString();
        messageCount = jsonDoc["messageCount"].toInteger();
        size = jsonDoc["size"].toInteger();
        owner = jsonDoc["delay"].toString();
        created = QDateTime::fromString(jsonDoc["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonDoc["modified"].toString(), Qt::ISODate);
    }
};

#endif // GET_TOPIC_DETAILS_RESPONSE_H
