#ifndef AWSMOCK_QT_UI_SSM_PARAMETER_GET_RESULT_H
#define AWSMOCK_QT_UI_SSM_PARAMETER_GET_RESULT_H

#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <dto/ssm/SSMParameterCounter.h>

struct SSMParameterGetResponse {
    long total;

    SSMParameterCounter parameter;

    void FromJson(const QJsonDocument &jsonDoc) {
        parameter.FromJson(jsonDoc["Parameter"].toObject());
    }
};

#endif // AWSMOCK_QT_UI_SSM_PARAMETER_GET_RESULT_H
