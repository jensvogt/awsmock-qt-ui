#pragma once

#include <QJsonArray>
#include <QJsonDocument>

#include <dto/lambda/LambdaInstanceCounter.h>

struct LambdaListEnvironmentResponse {
    long total{};

    QMap<QString, QString> environmentCounters;

    void FromJson(const QJsonDocument &jsonDoc) {
        for (const auto &jElement: jsonDoc["environmentCounters"].toArray()) {
            environmentCounters[jElement.toObject()["key"].toString()] = jElement.toObject()["value"].toString();
        }
    }
};
