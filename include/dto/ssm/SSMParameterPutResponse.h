#pragma once

#include <QJsonObject>
#include <QJsonDocument>

struct SSMParameterPutResponse {

    QString tier;

    long version;

    void FromJson(const QJsonDocument &jsonDoc) {
        tier = jsonDoc["Tier"].toString();
        version = jsonDoc["Version"].toInt();
    }
};
