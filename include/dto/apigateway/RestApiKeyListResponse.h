#pragma once

// Qt includes
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>

// Awsmock includes
#include <dto/apigateway/model/RestApiKey.h>

struct RestApiKeyListResponse {

    long total{};

    QList<RestApiKey> apiKeys;

    void FromJson(const QJsonDocument &jsonDoc) {

        total = jsonDoc.object()["total"].toInt();

        for (QJsonArray jArray = jsonDoc["apiKeys"].toArray(); const auto &element: jArray) {
            RestApiKey apiKey;
            apiKey.fromJson(element.toObject());
            apiKeys.append(apiKey);
        }
    }
};
