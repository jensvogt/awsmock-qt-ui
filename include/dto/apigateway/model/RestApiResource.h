//
// Created by vogje01 on 6/11/26.
//
#pragma once

// Qt includes
#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>

// Awsmock includes
#include <dto/apigateway/model/RestApiResourceMethod.h>

struct RestApiResource {
    /**
     * @brief Resource ID
     */
    QString id;

    /**
     * @brief Parent ID
     */
    QString parentId;

    /**
     * @brief Resource path
     */
    QString path;

    /**
     * @brief Path part
     */
    QString pathPart;

    /**
     * @brief Full invoke URL for this resource
     */
    QString url;

    /**
     * @brief Resource methods
     */
    std::map<QString, RestApiResourceMethod> resourceMethods;

    /**
     * Creation date
     */
    QDateTime created;

    /**
     * Last modification date
     */
    QDateTime modified;

    void fromJson(const QJsonObject &jsonObject) {
        id = jsonObject["id"].toString();
        parentId = jsonObject["parentId"].toString();
        path = jsonObject["path"].toString();
        pathPart = jsonObject["pathPart"].toString();
        url = jsonObject["url"].toString();
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODateWithMs);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODateWithMs);

        const QJsonObject methods = jsonObject["resourceMethods"].toObject();
        for (const QString &key: methods.keys()) {
            RestApiResourceMethod method;
            method.fromJson(methods[key].toObject());
            resourceMethods[key] = method;
        }
    }

    [[nodiscard]]
    QJsonObject toJson() const {
        QJsonObject jsonObject;
        jsonObject["id"] = id;
        jsonObject["parentId"] = parentId;
        jsonObject["path"] = path;
        jsonObject["pathPart"] = pathPart;
        jsonObject["url"] = url;
        jsonObject["created"] = created.toString(Qt::ISODateWithMs);
        jsonObject["modified"] = modified.toString(Qt::ISODateWithMs);
        QJsonObject methods;
        for (const auto &[key, method]: resourceMethods) {
            methods[key] = method.toJson();
        }
        jsonObject["resourceMethods"] = methods;
        return jsonObject;
    }
};
