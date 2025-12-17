#ifndef AWSMOCK_QT_UI_LAMBDA_LIST_LAMBDA_ENVIRONMENT_RESPONSE_H
#define AWSMOCK_QT_UI_LAMBDA_LIST_LAMBDA_ENVIRONMENT_RESPONSE_H

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

#endif // AWSMOCK_QT_UI_LAMBDA_LIST_LAMBDA_ENVIRONMENT_RESPONSE_H
