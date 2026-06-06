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

    QDateTime created;

    QDateTime modified;

    void FromJson(const QJsonObject &jsonObject) {

        name = jsonObject["name"].toString();
        apiKeySource = jsonObject["apiKeySource"].toString();
        enabled = jsonObject["enabled"].toBool();
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODate);

        for (QJsonArray jArray = jsonObject["binaryMediaTypes"].toArray(); const auto &element: jArray) {
            binaryMediaTypes.append(element.toString());
        }
    }
};
