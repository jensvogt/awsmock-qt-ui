#pragma once

// Qt includes
#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>

// Awsmock includes
#include <dto/apigateway/model/RestApiAuthorizer.h>
#include <dto/apigateway/model/RestApiResource.h>

struct RestApiCounter {

    QString id;

    QString name;

    QString apiKeySource;

    QString rootResourceId;

    QVector<QString> binaryMediaTypes;

    QMap<QString, RestApiResource> resources;

    QMap<QString, RestApiAuthorizer> authorizers;

    bool enabled{};

    bool generateDistinct{};

    QString description;

    QString policy;

    QDateTime created;

    QDateTime modified;

    void fromJson(const QJsonObject &jsonObject) {

        id = jsonObject["id"].toString();
        name = jsonObject["name"].toString();
        apiKeySource = jsonObject["apiKeySource"].toString();
        policy = jsonObject["policy"].toString();
        rootResourceId = jsonObject["rootResourceId"].toString();
        description = jsonObject["description"].toString();
        enabled = jsonObject["enabled"].toBool();
        generateDistinct = jsonObject["generateDistinct"].toBool();
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODate);

        for (QJsonArray jArray = jsonObject["binaryMediaTypes"].toArray(); const auto &element: jArray) {
            binaryMediaTypes.append(element.toString());
        }

        for (const QJsonObject jResources = jsonObject["resources"].toObject(); const QString &key: jResources.keys()) {
            RestApiResource resource;
            resource.fromJson(jResources[key].toObject());
            resources[key] = resource;
        }

        for (const QJsonObject jAuthorizers = jsonObject["authorizers"].toObject(); const QString &key: jAuthorizers.keys()) {
            RestApiAuthorizer authorizer;
            authorizer.fromJson(jAuthorizers[key].toObject());
            authorizers[key] = authorizer;
        }
    }

    [[nodiscard]]
    QJsonObject toJson() const {
        QJsonObject jsonObject;
        jsonObject["id"] = id;
        jsonObject["name"] = name;
        jsonObject["rootResourceId"] = rootResourceId;
        jsonObject["apiKeySource"] = apiKeySource;
        jsonObject["description"] = description;
        jsonObject["enabled"] = enabled;
        jsonObject["generateDistinct"] = generateDistinct;
        jsonObject["created"] = created.toString(Qt::ISODate);
        jsonObject["modified"] = modified.toString(Qt::ISODate);
        QJsonObject jResources;
        for (const auto &resource: resources) {
            jResources[resource.id] = resource.toJson();
        }
        jsonObject["resources"] = jResources;
        QJsonObject jAuthorizers;
        for (const auto &authorizer: authorizers) {
            jAuthorizers[authorizer.id] = authorizer.toJson();
        }
        jsonObject["authorizers"] = jAuthorizers;
        return jsonObject;
    }
};
