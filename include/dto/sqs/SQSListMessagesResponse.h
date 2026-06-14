#pragma once

// Qt includes
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

// Awsmock includes
#include <dto/sqs/SQSMessageCounter.h>

struct SQSListMessagesResponse {
    long total{};

    QList<SQSMessageCounter> messageCounters;

    void FromJson(const QJsonDocument &jsonDoc) {

        total = jsonDoc.object()["total"].toInt();
        for (QJsonArray jArray = jsonDoc["messageCounters"].toArray(); const auto &element: jArray) {
            SQSMessageCounter messageCounter;
            messageCounter.FromJson(element.toObject());
            messageCounters.append(messageCounter);
        }
    }
};
