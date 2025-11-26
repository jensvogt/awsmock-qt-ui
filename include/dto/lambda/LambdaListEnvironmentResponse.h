#ifndef AWSMOCK_QT_UI_LAMBDA_LIST_LAMBDA_ENVIRONMENT_RESPONSE_H
#define AWSMOCK_QT_UI_LAMBDA_LIST_LAMBDA_ENVIRONMENT_RESPONSE_H

#include <QJsonObject>
#include <QJsonDocument>

#include <dto/lambda/LambdaInstanceCounter.h>

struct LambdaListEnvironmentResponse {
    long total{};

    QMap<QString, QString> environmentCounters;

    std::vector<std::pair<std::string, std::string> > environmentCounters;

    void FromJson(const QJsonDocument &jsonDoc) {
        for (const auto jKey: jsonDoc["environmentCounters"].toObject().keys()) {
            LambdaInstanceCounter lambdaCounter;
            lambdaCounter.FromJson(element.toObject());
            lambdaInstanceCounters.append(lambdaCounter);
        }
    }
};

#endif // AWSMOCK_QT_UI_LAMBDA_LIST_LAMBDA_ENVIRONMENT_RESPONSE_H
