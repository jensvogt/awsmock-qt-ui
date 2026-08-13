#pragma once

// Qt includes
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

// Awsmock includes
#include <dto/application/ApplicationCounter.h>

struct ApplicationListResponse {

    long total{};

    QList<ApplicationCounter> applicationCounters;

    void FromJson(const QJsonDocument &jsonDoc) {

        total = jsonDoc.object()["total"].toInt();

        for (QJsonArray jArray = jsonDoc["applications"].toArray(); const auto &element: jArray) {
            ApplicationCounter applicationCounter;
            applicationCounter.FromJson(element.toObject());
            applicationCounters.append(applicationCounter);
        }
    }
};
