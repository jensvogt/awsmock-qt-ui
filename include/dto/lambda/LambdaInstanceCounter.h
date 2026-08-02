#pragma once

#include <QDateTime>
#include <QJsonObject>

struct LambdaInstanceCounter {

    QString lambdaName;

    QString instanceId;

    QString containerId;

    QString status;

    QString runtimeVersion;

    QString hostname;

    int duration;

    int privatePort;

    int publicPort;

    int invocations;

    std::optional<QDateTime> lastStart;

    std::optional<QDateTime> lastInvocation;

    std::optional<QDateTime> lastStop;

    void FromJson(const QJsonObject &jsonObject) {
        lambdaName = jsonObject["lambdaName"].toString();
        instanceId = jsonObject["instanceId"].toString();
        containerId = jsonObject["containerId"].toString();
        status = jsonObject["status"].toString();
        runtimeVersion = jsonObject["runtimeVersion"].toString();
        hostname = jsonObject["hostname"].toString();
        duration = jsonObject["duration"].toInt();
        invocations = jsonObject["invocations"].toInt();
        privatePort = jsonObject["privatePort"].toInt();
        publicPort = jsonObject["publicPort"].toInt();
        if (!jsonObject["lastStart"].isUndefined()) {
            lastStart = QDateTime::fromString(jsonObject["lastStart"].toString(), Qt::ISODate);
        }
        if (!jsonObject["lastInvocation"].isUndefined()) {
            lastInvocation = QDateTime::fromString(jsonObject["lastInvocation"].toString(), Qt::ISODate);
        }
        if (!jsonObject["lastStop"].isUndefined()) {
            lastStop = QDateTime::fromString(jsonObject["lastStop"].toString(), Qt::ISODate);
        }
    }
};
