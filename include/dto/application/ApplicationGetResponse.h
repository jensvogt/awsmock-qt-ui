#ifndef AWSMOCK_QT_UI_APPLICATION_GET_APPLICATION_RESPONSE_H
#define AWSMOCK_QT_UI_APPLICATION_GET_APPLICATION_RESPONSE_H

#include <QJsonObject>
#include <QJsonDocument>

#include <dto/application/Application.h>

struct ApplicationGetResponse {

    QString region;

    Application application;

    void FromJson(const QJsonObject &jsonObject) {
        region = jsonObject["region"].toString();
        application.FromJson(jsonObject["application"].toObject());
    }

};

#endif // AWSMOCK_QT_UI_APPLICATION_GET_APPLICATION_RESPONSE_H
