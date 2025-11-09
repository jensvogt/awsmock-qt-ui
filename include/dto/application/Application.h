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

    QString imageName;

    long imageSize;

    QString imageMd5;

    QString containerId;

    QString containerName;

    QString status;

    QString description;

    bool enabled;

    QMap<QString, QString> environment;

    QMap<QString, QString> tags;

    QList<QString> dependencies;

    QDateTime lastStarted;

    QDateTime created;

    QDateTime modified;

    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject jsonObject;
        jsonObject["region"] = region;
        jsonObject["name"] = name;
        jsonObject["runtime"] = runtime;
        jsonObject["runType"] = runType;
        jsonObject["version"] = version;
        jsonObject["archive"] = archive;
        jsonObject["privatePort"] = static_cast<qint64>(privatePort);
        jsonObject["publicPort"] = static_cast<qint64>(publicPort);
        jsonObject["imageId"] = imageId;
        jsonObject["imageName"] = imageName;
        jsonObject["imageSize"] = static_cast<qint64>(imageSize);
        jsonObject["imageMd5"] = imageMd5;
        jsonObject["containerId"] = containerId;
        jsonObject["containerName"] = containerName;
        jsonObject["status"] = status;
        jsonObject["description"] = description;
        jsonObject["enabled"] = enabled;
        jsonObject["lastStarted"] = lastStarted.toString(Qt::ISODate);
        jsonObject["created"] = lastStarted.toString(Qt::ISODate);
        jsonObject["modified"] = lastStarted.toString(Qt::ISODate);

        // Environment
        if (!environment.isEmpty()) {
            QJsonObject jsonEnvironment;
            for (auto it = environment.cbegin(); it != environment.cend(); ++it) {
                jsonEnvironment.insert(it.key(), it.value());
            }
            jsonObject["environment"] = jsonEnvironment;
        }

        // Tags
        if (!tags.isEmpty()) {
            QJsonObject jsonTags;
            for (auto it = tags.cbegin(); it != tags.cend(); ++it) {
                jsonTags.insert(it.key(), it.value());
            }
            jsonObject["tags"] = jsonTags;
        }

        // Dependencies
        if (!dependencies.isEmpty()) {
            QJsonArray jsonDependencies;
            for (const auto &key: dependencies) {
                jsonDependencies.append(key);
            }
            jsonObject["dependencies"] = jsonDependencies;
        }
        return jsonObject;
    }

    void FromJson(const QJsonObject &jsonObject) {
        region = jsonObject["region"].toString();
        name = jsonObject["name"].toString();
        runtime = jsonObject["runtime"].toString();
        runType = jsonObject["runType"].toString();
        version = jsonObject["version"].toString();
        privatePort = jsonObject["privatePort"].toInt();
        publicPort = jsonObject["publicPort"].toInt();
        archive = jsonObject["archive"].toString();
        imageId = jsonObject["imageId"].toString();
        imageName = jsonObject["imageName"].toString();
        imageSize = jsonObject["imageSize"].toInteger();
        imageMd5 = jsonObject["imageMd5"].toString();
        containerId = jsonObject["containerId"].toString();
        containerName = jsonObject["containerName"].toString();
        status = jsonObject["status"].toString();
        description = jsonObject["description"].toString();
        enabled = jsonObject["enabled"].toBool();
        lastStarted = QDateTime::fromString(jsonObject["lastStarted"].toString(), Qt::ISODate);
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODate);

        // Environment
        if (jsonObject.contains("environment")) {
            for (const auto &key: jsonObject["environment"].toObject().keys()) {
                environment[key] = jsonObject["environment"][key].toString();
            }
        }

        // Tags
        if (jsonObject.contains("tags")) {
            for (const auto &key: jsonObject["tags"].toObject().keys()) {
                tags[key] = jsonObject["tags"][key].toString();
            }
        }

        // Dependencies
        if (jsonObject.contains("dependencies")) {
            for (const auto &key: jsonObject["dependencies"].toArray()) {
                dependencies.append(key.toString());
            }
        }
    }
};

#endif // AWSMOCK_QT_UI_APPLICATION_H
