#pragma once

// Qt includes
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>

// Awsmock includes
#include <dto/apigateway/model/UsagePlan.h>

struct UsagePlanListResponse {

    long total{};

    QList<UsagePlan> usagePlans;

    void FromJson(const QJsonDocument &jsonDoc) {

        total = jsonDoc.object()["total"].toInt();

        for (QJsonArray jArray = jsonDoc["usagePlans"].toArray(); const auto &element: jArray) {
            UsagePlan usagePlan;
            usagePlan.fromJson(element.toObject());
            usagePlans.append(usagePlan);
        }
    }
};
