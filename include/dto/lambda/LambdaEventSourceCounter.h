#pragma once

#include <QJsonObject>

struct LambdaEventSourceCounter {

    QString type;

    QString functionArn;

    QString eventSourceArn;

    long batchSize{};

    long maximumBatchingWindowInSeconds{};

    QString uuid;

    void FromJson(const QJsonObject &jsonObject) {
        type = jsonObject["Type"].toString();
        functionArn = jsonObject["FunctionArn"].toString();
        eventSourceArn = jsonObject["EventSourceArn"].toString();
        batchSize = jsonObject["BatchSize"].toInteger();
        maximumBatchingWindowInSeconds = jsonObject["MaximumBatchingWindowInSeconds"].toInteger();
        uuid = jsonObject["UUID"].toString();
    }
};
