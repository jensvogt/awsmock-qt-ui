#pragma once

// Qt includes
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <dto/sqs/SQSQueueCounter.h>

struct SQSQueueListResponse {

    long total{};

    QList<SQSQueueCounter> queueCounters;

    void FromJson(const QJsonDocument &jsonDoc) {

        total = jsonDoc.object()["total"].toInt();
        for (QJsonArray jArray = jsonDoc["queueCounters"].toArray(); const auto &element: jArray) {
            SQSQueueCounter queueCounter;
            queueCounter.FromJson(element.toObject());
            queueCounters.append(queueCounter);
        }
    }
};
