#ifndef AWSMOCK_QT_UI_APPLICATION_UPDATE_REQUEST_H
#define AWSMOCK_QT_UI_APPLICATION_UPDATE_REQUEST_H

#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <dto/application/Application.h>

struct ApplicationUpdateRequest {

    QString region;

    Application application;

    QString ToJson() const {

        QJsonObject jRequest;
        jRequest["region"] = region;
        jRequest["application"] = application.ToJsonObject();
        const QJsonDocument requestDoc(jRequest);
        return requestDoc.toJson();
    }
};

#endif // AWSMOCK_QT_UI_APPLICATION_UPDATE_REQUEST_H
