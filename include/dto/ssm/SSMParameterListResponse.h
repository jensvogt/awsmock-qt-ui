#pragma once

// Qt includes
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

// Awsmock includes
#include <dto/ssm/SSMParameterCounter.h>

struct SSMParameterListResponse {

    long total;

    QList<SSMParameterCounter> parameterCounters;

    void FromJson(const QJsonDocument &jsonDoc) {

        total = jsonDoc.object()["total"].toInt();

        for (QJsonArray jArray = jsonDoc["parameterCounters"].toArray(); const auto &element: jArray) {
            SSMParameterCounter parameterCounter;
            parameterCounter.FromJson(element.toObject());
            parameterCounters.append(parameterCounter);
        }
    }
};
