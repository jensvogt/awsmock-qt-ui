#ifndef AWSMOCK_QT_UI_SSM_PARAMETER_LIST_RESULT_H
#define AWSMOCK_QT_UI_SSM_PARAMETER_LIST_RESULT_H

#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <dto/ssm/SSMParameterCounter.h>

struct SSMParameterListResponse {
    long total;

    QList<SSMParameterCounter> parameterCounters;

    void FromJson(const QJsonDocument &jsonDoc) {
        for (QJsonArray jArray = jsonDoc["parameterCounters"].toArray(); const auto &element: jArray) {
            SSMParameterCounter parameterCounter;
            parameterCounter.FromJson(element.toObject());
            parameterCounters.append(parameterCounter);
        }
    }
};

#endif // AWSMOCK_QT_UI_SSM_PARAMETER_LIST_RESULT_H
