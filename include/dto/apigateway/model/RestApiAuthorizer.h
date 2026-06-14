// Created by vogje01 on 6/13/26.
#pragma once

// Qt includes
#include <QDateTime>
#include <QJsonObject>

struct RestApiAuthorizer {

    QString id;

    QString name;

    QString type;

    QString authType;

    QString authorizerUri;

    QString identitySource;

    int authorizerResultTtlInSeconds{};

    QDateTime created;

    QDateTime modified;

    void fromJson(const QJsonObject &jsonObject) {
        id = jsonObject["id"].toString();
        name = jsonObject["name"].toString();
        type = jsonObject["type"].toString();
        authType = jsonObject["authType"].toString();
        authorizerUri = jsonObject["authorizerUri"].toString();
        identitySource = jsonObject["identitySource"].toString();
        authorizerResultTtlInSeconds = jsonObject["authorizerResultTtlInSeconds"].toInt();
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODateWithMs);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODateWithMs);
    }

    [[nodiscard]]
    QJsonObject toJson() const {
        QJsonObject jsonObject;
        jsonObject["id"] = id;
        jsonObject["name"] = name;
        jsonObject["type"] = type;
        jsonObject["authType"] = authType;
        jsonObject["authorizerUri"] = authorizerUri;
        jsonObject["identitySource"] = identitySource;
        jsonObject["authorizerResultTtlInSeconds"] = authorizerResultTtlInSeconds;
        jsonObject["created"] = created.toString(Qt::ISODateWithMs);
        jsonObject["modified"] = modified.toString(Qt::ISODateWithMs);
        return jsonObject;
    }
};
