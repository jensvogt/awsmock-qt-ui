#pragma once

// Qt includes
#include <QDateTime>
#include <QJsonObject>

// Awsmock includes
#include <dto/apigateway/RestApiCounter.h>

struct RestApiListResponse {

    long total{};

    QList<RestApiCounter> restApiCounters;

    void FromJson(const QJsonDocument &jsonDoc) {

        total = jsonDoc.object()["total"].toInt();

        for (QJsonArray jArray = jsonDoc["restApis"].toArray(); const auto &element: jArray) {
            RestApiCounter restApiCounter;
            restApiCounter.FromJson(element.toObject());
            restApiCounters.append(restApiCounter);
        }
    }
};
