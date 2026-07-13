#pragma once

// Qt includes
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

struct RestApiKeyCreateRequest {

    QString name;

    QString description;

    QString value;

    bool enabled{true};

    [[nodiscard]]
    QString ToJson() const {
        QJsonObject jRequest;
        jRequest["name"] = name;
        jRequest["description"] = description;
        jRequest["value"] = value;
        jRequest["enabled"] = enabled;
        return QJsonDocument(jRequest).toJson();
    }
};
