#ifndef AWSMOCK_QT_UI_S3_LIST_OBJECT_VERSION_COUNTERS_RESPONSE_H
#define AWSMOCK_QT_UI_S3_LIST_OBJECT_VERSION_COUNTERS_RESPONSE_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>

struct S3ObjectVersionCounter {
    QString oid;

    QString key;

    QString versionId;

    QString storageClass;

    long size{};

    bool isLatest{};

    QDateTime created;

    QDateTime modified;

    void FromJson(const QJsonObject &jsonObject) {
        oid = jsonObject["oid"].toString();
        key = jsonObject["key"].toString();
        versionId = jsonObject["versionId"].toString();
        storageClass = jsonObject["storageClass"].toString();
        size = jsonObject["size"].toInteger();
        isLatest = jsonObject["isLatest"].toBool();
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODate);
    }
};

struct S3ListObjectVersionCountersResponse {
    long total{};

    QList<S3ObjectVersionCounter> versionCounters;

    void FromJson(const QJsonDocument &jsonDoc) {

        total = jsonDoc.object()["total"].toInt();

        for (QJsonArray jArray = jsonDoc["versionCounters"].toArray(); const auto &element: jArray) {
            S3ObjectVersionCounter versionCounter;
            versionCounter.FromJson(element.toObject());
            versionCounters.append(versionCounter);
        }
    }
};

#endif // AWSMOCK_QT_UI_S3_LIST_OBJECT_VERSION_COUNTERS_RESPONSE_H
