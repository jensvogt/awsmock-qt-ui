#ifndef LIST_MESSAGE_RESULT_H
#define LIST_MESSAGE_RESULT_H

// Qt includes
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

// Awsmock includes
#include <dto/sns/SNSMessageCounter.h>

struct SNSListMessagesResult {

    long total{};

    QList<SNSMessageCounter> messageCounters;

    void FromJson(const QJsonDocument &jsonDoc) {

        total = jsonDoc.object()["total"].toInt();

        for (QJsonArray jArray = jsonDoc["messages"].toArray(); const auto &element: jArray) {
            SNSMessageCounter messageCounter;
            messageCounter.FromJson(element.toObject());
            messageCounters.append(messageCounter);
        }
    }
};

#endif // LIST_MESSAGE_RESULT_H
