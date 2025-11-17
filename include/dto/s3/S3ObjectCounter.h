//
// Created by vogje01 on 11/13/25.
//

#ifndef AWSMOCK_QT_UI_S3_OBJECT_COUNTER_H
#define AWSMOCK_QT_UI_S3_OBJECT_COUNTER_H

#include <QJsonObject>
#include <QJsonArray>

#include <utils/JsonUtils.h>

struct S3ObjectCounter {
    QString oid;

    QString region;

    QString bucket;

    QString key;

    QString contentType;

    long size;

    QDateTime created;

    QDateTime modified;

    void FromJson(const QJsonObject &jsonObject) {
        JsonUtils::WriteJsonString(jsonObject);
        oid = jsonObject["oid"].toString();
        region = jsonObject["region"].toString();
        bucket = jsonObject["bucketName"].toString();
        key = jsonObject["key"].toString();
        contentType = jsonObject["contentType"].toString();
        size = jsonObject["size"].toInteger();
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODate);
    }
};
#endif //AWSMOCK_QT_UI_S3_OBJECT_COUNTER_H
