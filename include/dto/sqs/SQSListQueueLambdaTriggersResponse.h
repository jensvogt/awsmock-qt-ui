#pragma once

// Qt includes
#include <QJsonArray>
#include <QJsonDocument>

// Awsmock includes
#include <dto/sqs/SQSQueueLambdaTriggerCounter.h>

struct SQSListQueueLambdaTriggersResponse {
    long total{};

    QList<SQSQueueLambdaTriggerCounter> queueLambdaTriggerCounters;

    void FromJson(const QJsonDocument &jsonDoc) {

        for (QJsonArray jArray = jsonDoc["lambdaTriggerCounters"].toArray(); const auto &element: jArray) {
            SQSQueueLambdaTriggerCounter queueLambdaTriggerCounter;
            queueLambdaTriggerCounter.FromJson(element.toObject());
            queueLambdaTriggerCounters.append(queueLambdaTriggerCounter);
        }
    }
};
