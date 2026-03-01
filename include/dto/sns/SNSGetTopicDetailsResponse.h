#ifndef GET_TOPIC_DETAILS_RESPONSE_H
#define GET_TOPIC_DETAILS_RESPONSE_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

struct SNSGetTopicDetailsResponse {

    QString region;

    QString topicName;

    QString topicArn;

    long messageCount;

    long size;

    QString owner;

    QDateTime created;

    QDateTime modified;

    void FromJson(const QJsonDocument &jsonDoc) {
        region = jsonDoc["region"].toString();
        topicName = jsonDoc["topicName"].toString();
        topicArn = jsonDoc["_topicArn"].toString();
        messageCount = jsonDoc["messageCount"].toInteger();
        size = jsonDoc["size"].toInteger();
        owner = jsonDoc["owner"].toString();
        created = QDateTime::fromString(jsonDoc["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonDoc["modified"].toString(), Qt::ISODate);
    }
};

#endif // GET_TOPIC_DETAILS_RESPONSE_H
