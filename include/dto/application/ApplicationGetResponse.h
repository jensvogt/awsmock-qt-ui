#pragma once

// QT includes
#include <QJsonObject>
#include <QJsonDocument>

// Awsmock includes
#include <dto/application/Application.h>

struct ApplicationGetResponse {

    Application application;

    void FromJson(const QJsonObject &jsonObject) {
        application.FromJson(jsonObject["application"].toObject());
        application.region = jsonObject["region"].toString();
    }
};
