#ifndef AWSMOCK_QT_UI_LAMBDA_LIST_LAMBDA_INSTANCES_RESPONSE_H
#define AWSMOCK_QT_UI_LAMBDA_LIST_LAMBDA_INSTANCES_RESPONSE_H

#include <QJsonObject>
#include <QJsonDocument>

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

#endif // AWSMOCK_QT_UI_LAMBDA_LIST_LAMBDA_INSTANCES_RESPONSE_H
