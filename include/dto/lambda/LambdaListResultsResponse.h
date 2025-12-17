#ifndef AWSMOCK_QT_UI_LAMBDA_LIST_LAMBDA_LOGS_RESPONSE_H
#define AWSMOCK_QT_UI_LAMBDA_LIST_LAMBDA_LOGS_RESPONSE_H

#include <QJsonDocument>
#include <QJsonArray>

#include <dto/lambda/LambdaResultCounter.h>

struct LambdaListResultsResponse {
    long total{};

    QList<LambdaResultCounter> lambdaLogCounters;

    void FromJson(const QJsonDocument &jsonDoc) {
        for (QJsonArray jArray = jsonDoc["lambdaResultCounters"].toArray(); const auto &element: jArray) {
            LambdaResultCounter lambdaCounter;
            lambdaCounter.FromJson(element.toObject());
            lambdaLogCounters.append(lambdaCounter);
        }
    }
};

#endif // AWSMOCK_QT_UI_LAMBDA_LIST_LAMBDA_LOGS_RESPONSE_H
