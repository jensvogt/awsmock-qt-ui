#ifndef AWSMOCK_QT_UI_APPLICATION_COUNTER_H
#define AWSMOCK_QT_UI_APPLICATION_COUNTER_H

#include <QDateTime>
#include <QJsonObject>

struct ApplicationCounter {
    QString name;

    QString runtime;

    QString runtype;

    int privatePort;

    int publicPort;

    QString version;

    QString status;

    bool enabled;

    QDateTime lastStarted;

    QDateTime created;

    QDateTime modified;

    void FromJson(const QJsonObject &jsonObject) {
        name = jsonObject["name"].toString();
        runtime = jsonObject["runType"].toString();
        runtype = jsonObject["runtime"].toString();
        privatePort = jsonObject["privatePort"].toInt();
        publicPort = jsonObject["publicPort"].toInt();
        version = jsonObject["version"].toString();
        status = jsonObject["status"].toString();
        enabled = jsonObject["enabled"].toBool();
        lastStarted = QDateTime::fromString(jsonObject["lastStarted"].toString(), Qt::ISODate);
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODate);
    }
};


#endif // AWSMOCK_QT_UI_APPLICATION_COUNTER_H
