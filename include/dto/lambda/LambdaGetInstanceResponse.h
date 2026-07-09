#pragma once

// Qt includes
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include <dto/lambda/LambdaInstanceCounter.h>

struct LambdaGetInstanceResponse {

    LambdaInstanceCounter lambdaInstanceCounters;

    void FromJson(const QJsonDocument &jsonDoc) {
        lambdaInstanceCounters.FromJson(jsonDoc["instanceCounter"].toObject());
    }
};
