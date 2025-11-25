#ifndef AWSMOCK_QT_UI_LAMBDA_LIST_RESPONSE_H
#define AWSMOCK_QT_UI_LAMBDA_LIST_RESPONSE_H

#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <dto/sns/SNSTopicCounter.h>

#include <dto/lambda/LambdaCounter.h>

struct LambdaListResponse {
    long total{};

    QList<LambdaCounter> lambdaCounters;

    void FromJson(const QJsonDocument &jsonDoc) {
        for (QJsonArray jArray = jsonDoc["lambdas"].toArray(); const auto &element: jArray) {
            LambdaCounter lambdaCounter;
            lambdaCounter.FromJson(element.toObject());
            lambdaCounters.append(lambdaCounter);
        }
    }
};

#endif // AWSMOCK_QT_UI_LAMBDA_LIST_RESPONSE_H
