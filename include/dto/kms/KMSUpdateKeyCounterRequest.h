#ifndef AWSMOCK_QT_UI_KMS_UPDATE_KEY_COUNTER_RESPONSE_H
#define AWSMOCK_QT_UI_KMS_UPDATE_KEY_COUNTER_RESPONSE_H

// Awsmock includes
#include <utils/JsonUtils.h>
#include <dto/kms/KeyCounter.h>

struct KMSUpdateKeyCounterRequest {

    KeyCounter keyCounter;

    void FromJson(const QJsonDocument &jsonDoc) {
        keyCounter.FromJson(jsonDoc["keyCounter"].toObject());
    }

    QString ToJson() const {
        QJsonObject jsonObject;
        jsonObject["keyCounter"] = keyCounter.ToJsonObject();
        return QJsonDocument(jsonObject).toJson();
    }
};

#endif // AWSMOCK_QT_UI_KMS_UPDATE_KEY_COUNTER_RESPONSE_H
