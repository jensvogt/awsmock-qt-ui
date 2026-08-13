#pragma once

// Qt includes
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

// Awsmock
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
