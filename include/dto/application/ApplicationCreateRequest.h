#pragma once

// Qt includes
#include <QJsonObject>
#include <QJsonDocument>

// Awsmock includes
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
