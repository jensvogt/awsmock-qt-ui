#ifndef AWSMOCK_QT_UI_SQS_LIST_QUEUE_ATTRIBUTES_RESPONSE_H
#define AWSMOCK_QT_UI_SQS_LIST_QUEUE_ATTRIBUTES_RESPONSE_H

// Qt includes
#include <QJsonArray>
#include <QJsonDocument>

// Awsmock includes
#include <dto/sqs/SQSQueueAttributeCounter.h>

struct SQSQueueAttributeListResponse {
    long total{};

    QList<SQSQueueAttributeCounter> queueAttributeCounters;

    void FromJson(const QJsonDocument &jsonDoc) {

        for (QJsonArray jArray = jsonDoc["attributeCounters"].toArray(); const auto &element: jArray) {
            SQSQueueAttributeCounter queueAttributeCounter;
            queueAttributeCounter.FromJson(element.toObject());
            queueAttributeCounters.append(queueAttributeCounter);
        }
    }
};

#endif // AWSMOCK_QT_UI_SQS_LIST_QUEUE_ATTRIBUTES_RESPONSE_H
