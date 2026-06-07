#pragma once

// Qt includes
#include <QDateTime>
#include <QJsonObject>

// Awsmock includes
#include <dto/apigateway/RestApiCounter.h>

struct RestApiGetResponse {

    RestApiCounter restApiCounter;

    void FromJson(const QJsonDocument &jsonDoc) {
        restApiCounter.fromJson(jsonDoc["restApi"].toObject());
    }
};
