#pragma once

// Qt includes
#include <QJsonArray>
#include <QJsonDocument>

// Awsmock includes
#include <dto/sqs/SQSQueueLambdaTriggerCounter.h>
#include <dto/sqs/SQSMessageAttribute.h>

struct SQSListQueueDefaultAttributesResponse {
    long total{};

    QMap<QString, SQSMessageAttribute> defaultAttributesCounters;

    void FromJson(const QJsonDocument &jsonDoc) {

        for (QJsonObject jObject = jsonDoc["attributeCounters"].toObject(); const auto &key: jObject.keys()) {
            SQSMessageAttribute defaultAttribute;
            defaultAttribute.FromJson(jObject[key].toObject());
            defaultAttributesCounters[key] = defaultAttribute;
        }
    }
};
