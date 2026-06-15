#pragma once

// Qt includes
#include <QJsonDocument>

// Awsmock includes
#include <qjsonvalue.h>
#include <dto/kms/KeyCounter.h>

struct KMSGetKeyCounterResponse {

    KeyCounter keyCounter;

    void FromJson(const QJsonDocument &jsonDoc) {
        keyCounter.FromJson(jsonDoc["keyCounter"].toObject());
        keyCounter.region = jsonDoc["region"].toString();
    }
};
