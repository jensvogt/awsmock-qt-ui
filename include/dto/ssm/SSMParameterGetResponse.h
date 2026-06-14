#pragma once

// Qt includes
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

// Awsmock includes
#include <dto/ssm/SSMParameterCounter.h>

struct SSMParameterGetResponse {
    
    long total{};

    SSMParameterCounter parameter;

    void FromJson(const QJsonDocument &jsonDoc) {
        parameter.FromJson(jsonDoc["Parameter"].toObject());
        parameter.region = jsonDoc["Region"].toString();
    }
};
