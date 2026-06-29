#pragma once

// Qt includes
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

// Awsmock includes
#include <dto/lambda/LambdaInstanceCounter.h>

struct LambdaListInstancesResponse {
    long total{};

    QList<LambdaInstanceCounter> lambdaInstanceCounters;

    void FromJson(const QJsonDocument &jsonDoc) {
        for (QJsonArray jArray = jsonDoc["instanceCounters"].toArray(); const auto &element: jArray) {
            LambdaInstanceCounter lambdaCounter;
            lambdaCounter.FromJson(element.toObject());
            lambdaInstanceCounters.append(lambdaCounter);
        }
    }
};
