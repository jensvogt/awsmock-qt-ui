#ifndef AWSMOCK_QT_UI_SQS_LIST_QUEUE_DEFAULT_ATTRIBUTES_RESPONSE_H
#define AWSMOCK_QT_UI_SQS_LIST_QUEUE_DEFAULT_ATTRIBUTES_RESPONSE_H

// Qt includes
#include <QJsonArray>
#include <QJsonDocument>

// Awsmock includes
#include <dto/sqs/SQSQueueLambdaTriggerCounter.h>
#include <dto/sqs/SQSMessageAttribute.h>

struct SQSListQueueDefaultAttributesResponse {
    long total{};

    QMap<QString, MessageAttribute> defaultAttributesCounters;

    void FromJson(const QJsonDocument &jsonDoc) {

        for (QJsonObject jObject = jsonDoc["attributeCounters"].toObject(); const auto &key: jObject.keys()) {
            MessageAttribute defaultAttribute;
            defaultAttribute.FromJson(jObject[key].toObject());
            defaultAttributesCounters[key] = defaultAttribute;
        }
    }
};

#endif // AWSMOCK_QT_UI_SQS_LIST_QUEUE_DEFAULT_ATTRIBUTES_RESPONSE_H
