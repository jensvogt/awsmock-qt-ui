#ifndef AWSMOCK_QT_UI_S3_LIST_OBJECT_RESPONSE_H
#define AWSMOCK_QT_UI_S3_LIST_OBJECT_RESPONSE_H

#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <dto/s3/S3ObjectCounter.h>

struct S3ListObjectsResult {
    long total{};

    QList<S3ObjectCounter> objectCounters;

    void FromJson(const QJsonDocument &jsonDoc) {

        QJsonArray jArray = jsonDoc["objects"].toArray();

        for (const auto &element: jArray) {
            S3ObjectCounter objectCounter;
            objectCounter.FromJson(element.toObject());
            objectCounters.append(objectCounter);
        }
    }
};

#endif // AWSMOCK_QT_UI_S3_LIST_OBJECT_RESPONSE_H
