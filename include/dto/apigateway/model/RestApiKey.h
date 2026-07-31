#pragma once

// Qt includes
#include <QDateTime>
#include <QJsonObject>

struct RestApiKey {

    QString id;

    QString name;

    QString description;

    QString value;

    bool enabled{};

    QDateTime created;

    QDateTime modified;

    void fromJson(const QJsonObject &jsonObject) {
        id = jsonObject["id"].toString();
        name = jsonObject["name"].toString();
        description = jsonObject["description"].toString();
        value = jsonObject["value"].toString();
        enabled = jsonObject["enabled"].toBool();
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODateWithMs);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODateWithMs);
    }

    [[nodiscard]]
    QJsonObject toJson() const {
        QJsonObject jsonObject;
        jsonObject["id"] = id;
        jsonObject["name"] = name;
        jsonObject["description"] = description;
        jsonObject["value"] = value;
        jsonObject["enabled"] = enabled;
        jsonObject["created"] = created.toString(Qt::ISODateWithMs);
        jsonObject["modified"] = modified.toString(Qt::ISODateWithMs);
        return jsonObject;
    }
};
