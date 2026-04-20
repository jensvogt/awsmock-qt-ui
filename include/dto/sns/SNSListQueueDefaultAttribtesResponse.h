#ifndef AWSMOCK_QT_UI_SNS_LIST_QUEUE_DEFAULT_ATTRIBUTES_RESPONSE_H
#define AWSMOCK_QT_UI_SNS_LIST_QUEUE_DEFAULT_ATTRIBUTES_RESPONSE_H

// Qt includes
#include <QJsonArray>
#include <QJsonDocument>

// Awsmock includes
#include <dto/sns/SNSMessageAttribute.h>

struct SNSListQueueDefaultAttributesResponse {
    long total{};

    QMap<QString, SNSMessageAttribute> defaultAttributesCounters;

    void FromJson(const QJsonDocument &jsonDoc) {

        for (QJsonObject jObject = jsonDoc["attributeCounters"].toObject(); const auto &key: jObject.keys()) {
            SNSMessageAttribute defaultAttribute;
            defaultAttribute.FromJson(jObject[key].toObject());
            defaultAttributesCounters[key] = defaultAttribute;
        }
    }
};

#endif // AWSMOCK_QT_UI_SNS_LIST_QUEUE_DEFAULT_ATTRIBUTES_RESPONSE_H
