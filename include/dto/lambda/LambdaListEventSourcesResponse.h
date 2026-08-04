#pragma once

// Qt includes
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

// Awsmock includes
#include <dto/lambda/LambdaEventSourceCounter.h>

struct LambdaListEventSourcesResponse {
    long total{};

    QList<LambdaEventSourceCounter> eventSourceCounters;

    void FromJson(const QJsonDocument &jsonDoc) {
        for (QJsonArray jArray = jsonDoc["eventSourceCounters"].toArray(); const auto &element: jArray) {
            LambdaEventSourceCounter eventSourceCounter;
            eventSourceCounter.FromJson(element.toObject());
            eventSourceCounters.append(eventSourceCounter);
        }
    }
};
