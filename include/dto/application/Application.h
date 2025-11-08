#ifndef AWSMOCK_QT_UI_APPLICATION_H
#define AWSMOCK_QT_UI_APPLICATION_H

#include <QDateTime>
#include <QJsonObject>

#include "utils/JsonUtils.h"

struct Application {

    QString region;

    QString name;

    QString runtime;

    QString runType;

    QString version;

    QString archive;

    long privatePort = -1;

    long publicPort = -1;

    QString imageId;

    QString containerId;

    QString containerName;

    QString status;

    bool enabled;

    QDateTime lastStarted;

    QDateTime created;

    QDateTime modified;

    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject jsonObject;
        jsonObject["name"] = name;
        jsonObject["runtime"] = runtime;
        jsonObject["runType"] = runType;
        jsonObject["version"] = version;
        jsonObject["archive"] = archive;
        jsonObject["privatePort"] = static_cast<qint64>(privatePort);
        jsonObject["publicPort"] = static_cast<qint64>(publicPort);
        JsonUtils::WriteJsonString(jsonObject);
        return jsonObject;
    }

    void FromJson(const QJsonObject &jsonObject) {
        region = jsonObject["region"].toString();
        name = jsonObject["name"].toString();
        runtime = jsonObject["runtime"].toString();
        runType = jsonObject["runType"].toString();
        privatePort = jsonObject["privatePort"].toInt();
        publicPort = jsonObject["publicPort"].toInt();
        archive = jsonObject["archive"].toString();
        version = jsonObject["version"].toString();
        imageId = jsonObject["imageId"].toString();
        containerId = jsonObject["containerId"].toString();
        containerName = jsonObject["containerName"].toString();
        status = jsonObject["status"].toString();
        enabled = jsonObject["enabled"].toBool();
        lastStarted = QDateTime::fromString(jsonObject["lastStarted"].toString(), Qt::ISODate);
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODate);
    }
};

#endif // AWSMOCK_QT_UI_APPLICATION_H
