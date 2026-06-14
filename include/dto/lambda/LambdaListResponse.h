#pragma once

// Qt includes
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

// Awsmock includes
#include <dto/lambda/LambdaCounter.h>

struct LambdaListResponse {
    long total{};

    QList<LambdaCounter> lambdaCounters;

    void FromJson(const QJsonDocument &jsonDoc) {

        total = jsonDoc.object()["total"].toInt();

        for (QJsonArray jArray = jsonDoc["functionCounters"].toArray(); const auto &element: jArray) {
            LambdaCounter lambdaCounter;
            lambdaCounter.FromJson(element.toObject());
            lambdaCounters.append(lambdaCounter);
        }
    }
};
