#ifndef AWSMOCK_QT_UI_KMS_GET_KEY_COUNTER_RESPONSE_H
#define AWSMOCK_QT_UI_KMS_GET_KEY_COUNTER_RESPONSE_H

// Qt includes
#include <QJsonDocument>

// Awsmock includes
#include <qjsonvalue.h>
#include <dto/kms/KeyCounter.h>

struct KMSGetKeyCounterResponse {

    KeyCounter keyCounter;

    void FromJson(const QJsonDocument &jsonDoc) {
        keyCounter.FromJson(jsonDoc["keyCounter"].toObject());
    }
};

#endif // AWSMOCK_QT_UI_KMS_GET_KEY_COUNTER_RESPONSE_H
