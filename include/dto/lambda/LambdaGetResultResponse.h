#ifndef AWSMOCK_QT_UI_LAMBDA_GET_LAMBDA_RESULT_RESPONSE_H
#define AWSMOCK_QT_UI_LAMBDA_GET_LAMBDA_RESULT_RESPONSE_H

#include <QJsonDocument>

#include <dto/lambda/LambdaResultCounter.h>

struct LambdaGetResultsResponse {

    LambdaResultCounter lambdaLogCounter;

    void FromJson(const QJsonDocument &jsonDoc) {
        lambdaLogCounter.FromJson(jsonDoc["lambdaResultCounter"].toObject());
    }
};

#endif // AWSMOCK_QT_UI_LAMBDA_GET_LAMBDA_RESULT_RESPONSE_H
