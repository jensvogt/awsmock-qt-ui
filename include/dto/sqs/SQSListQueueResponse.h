#ifndef AWSMOCK_QT_UI_SQS_LIST_QUEUE_RESPONSE_H
#define AWSMOCK_QT_UI_SQS_LIST_QUEUE_RESPONSE_H

#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <dto/sqs/SQSQueueCounter.h>

struct SQSQueueListResponse {

    long total{};

    QList<SQSQueueCounter> queueCounters;

    void FromJson(const QJsonDocument &jsonDoc) {

        QJsonArray jArray = jsonDoc["queueCounters"].toArray();

        for(const auto &element : jArray) {
            SQSQueueCounter queueCounter;
            queueCounter.FromJson(element.toObject());
            queueCounters.append(queueCounter);
        }
    }
};

#endif // AWSMOCK_QT_UI_SQS_LIST_QUEUE_RESPONSE_H
