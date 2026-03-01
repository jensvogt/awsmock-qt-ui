#ifndef AWSMOCK_QT_UI_LAMBDA_LIST_ARNS_RESPONSE_H
#define AWSMOCK_QT_UI_LAMBDA_LIST_ARNS_RESPONSE_H

// Qt includes
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

// Awsmock includes
#include <dto/lambda/LambdaCounter.h>

struct LambdaListArnsResponse {

    QList<QString> lambdaArns;

    void FromJson(const QJsonDocument &jsonDoc) {

        for (QJsonArray jArray = jsonDoc["lambdaArns"].toArray(); const auto &element: jArray) {
            lambdaArns.append(element.toString());
        }
    }
};

#endif // AWSMOCK_QT_UI_LAMBDA_LIST_ARNS_RESPONSE_H
