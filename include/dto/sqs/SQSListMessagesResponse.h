#ifndef AWSMOCK_QT_UI_SQS_LIST_MESSAGE_RESPONSE_H
#define AWSMOCK_QT_UI_SQS_LIST_MESSAGE_RESPONSE_H

#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <dto/sns/SNSMessageCounter.h>

#include "SQSMessageCounter.h"

struct SQSListMessagesResponse {
    long total{};

    QList<SQSMessageCounter> messageCounters;

    void FromJson(const QJsonDocument &jsonDoc) {
        QJsonArray jArray = jsonDoc["messageCounters"].toArray();

        for (const auto &element: jArray) {
            SQSMessageCounter messageCounter;
            messageCounter.FromJson(element.toObject());
            messageCounters.append(messageCounter);
        }
    }
};

#endif // AWSMOCK_QT_UI_SQS_LIST_MESSAGE_RESPONSE_H
