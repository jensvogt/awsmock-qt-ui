#ifndef LIST_MESSAGE_RESULT_H
#define LIST_MESSAGE_RESULT_H


#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <dto/sns/SNSMessageCounter.h>

struct SNSListMessagesResult {

    long total{};

    QList<SNSMessageCounter> messageCounters;

    void FromJson(const QJsonDocument& jsonDoc) {

        QJsonArray jArray = jsonDoc["messages"].toArray();

        for(const auto &element:jArray) {
            SNSMessageCounter messageCounter;
            messageCounter.FromJson(element.toObject());
            messageCounters.append(messageCounter);
        }
    }
};

#endif // LIST_MESSAGE_RESULT_H
