#ifndef AWSMOCK_QT_UI_KMS_LIST_KEYS_RESPONSE_H
#define AWSMOCK_QT_UI_KMS_LIST_KEYS_RESPONSE_H

// Qt includes
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

// Awsmock includes
#include <dto/kms/KeyCounter.h>

struct KMSListKeysResponse {

    long total{};

    QList<KeyCounter> keyCounters;

    void FromJson(const QJsonDocument &jsonDoc) {

        total = jsonDoc.object()["total"].toInt();

        for (QJsonArray jArray = jsonDoc["keyCounters"].toArray(); const auto &element: jArray) {
            KeyCounter keyCounter;
            keyCounter.FromJson(element.toObject());
            keyCounters.append(keyCounter);
        }
    }
};

#endif // AWSMOCK_QT_UI_KMS_LIST_KEYS_RESPONSE_H
