#pragma once

#include <QDateTime>
#include <QJsonObject>

struct LambdaCounter {
    QString region;

    QString name;

    QString arn;

    QString runtime;

    QString handler;

    QString description;

    int instances;

    int invocations;

    int averageRuntime;

    QString version;

    bool active;

    bool enabled;

    QString containerId;

    QString containerName;

    QString zipFile;

    QDateTime lastStarted;

    QDateTime created;

    QDateTime modified;

    void FromJson(const QJsonObject &jsonObject) {
        region = jsonObject["region"].toString();
        name = jsonObject["functionName"].toString();
        arn = jsonObject["lambdaArn"].toString();
        runtime = jsonObject["runtime"].toString();
        handler = jsonObject["handler"].toString();
        instances = jsonObject["instances"].toInt();
        invocations = jsonObject["invocations"].toInt();
        averageRuntime = jsonObject["averageRuntime"].toInt();
        version = jsonObject["version"].toString();
        active = jsonObject["active"].toBool();
        enabled = jsonObject["enabled"].toBool();
        containerId = jsonObject["containerId"].toString();
        containerName = jsonObject["containerName"].toString();
        lastStarted = QDateTime::fromString(jsonObject["lastStarted"].toString(), Qt::ISODate);
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODate);
    }
};
