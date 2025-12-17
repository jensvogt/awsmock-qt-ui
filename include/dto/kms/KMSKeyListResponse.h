#ifndef AWSMOCK_QT_UI_KMS_LIST_KEYS_RESPONSE_H
#define AWSMOCK_QT_UI_KMS_LIST_KEYS_RESPONSE_H

#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <dto/sqs/SQSQueueCounter.h>
#include <dto/kms/KeyCounter.h>

struct KMSKeyListResponse {

    long total{};

    QList<KeyCounter> keyCounters;

    void FromJson(const QJsonDocument &jsonDoc) {

        for (QJsonArray jArray = jsonDoc["keyCounters"].toArray(); const auto &element: jArray) {
            KeyCounter keyCounter;
            keyCounter.FromJson(element.toObject());
            keyCounters.append(keyCounter);
        }
    }
};

#endif // AWSMOCK_QT_UI_KMS_LIST_KEYS_RESPONSE_H
