#ifndef AWSMOCK_QT_UI_APPLICATION_CREATE_REQUEST_H
#define AWSMOCK_QT_UI_APPLICATION_CREATE_REQUEST_H

#include <QJsonObject>
#include <QJsonDocument>

#include <dto/application/Application.h>

struct ApplicationCreateRequest {

    QString region;

    Application application;

    QString applicationCode;

    [[nodiscard]] QString ToJson() const {

        QJsonObject jRequest;
        jRequest["region"] = region;
        jRequest["application"] = application.ToJsonObject();
        jRequest["code"] = applicationCode;
        const QJsonDocument requestDoc(jRequest);
        return requestDoc.toJson();
    }
};

#endif // AWSMOCK_QT_UI_APPLICATION_CREATE_REQUEST_H
