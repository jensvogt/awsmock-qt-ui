#pragma once

// Qt includes
#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>

// Awsmock includes
#include <dto/application/ApplicationCounter.h>

struct RestApiCounter {

    QString name;

    QString apiKeySource;

    QVector<QString> binaryMediaTypes;

    bool enabled;

    QString description;

    QDateTime created;

    QDateTime modified;

    void fromJson(const QJsonObject &jsonObject) {

        name = jsonObject["name"].toString();
        apiKeySource = jsonObject["apiKeySource"].toString();
        description = jsonObject["description"].toString();
        enabled = jsonObject["enabled"].toBool();
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODate);

        for (QJsonArray jArray = jsonObject["binaryMediaTypes"].toArray(); const auto &element: jArray) {
            binaryMediaTypes.append(element.toString());
        }
    }

    QJsonObject toJson() const {
        QJsonObject jsonObject;
        jsonObject["name"] = name;
        jsonObject["apiKeySource"] = apiKeySource;
        jsonObject["description"] = description;
        jsonObject["enabled"] = enabled;
        jsonObject["created"] = created.toString(Qt::ISODate);
        jsonObject["modified"] = modified.toString(Qt::ISODate);
        return jsonObject;
    }
};
