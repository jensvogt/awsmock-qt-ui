#ifndef AWSMOCK_QT_UI_S3_BUCKET_COUNTER_H
#define AWSMOCK_QT_UI_S3_BUCKET_COUNTER_H

#include <QDateTime>
#include <QJsonObject>

struct S3BucketCounter {
    QString region;

    QString bucketName;

    QString bucketArn;

    long objectCount;

    long size;

    QDateTime created;

    QDateTime modified;

    void FromJson(const QJsonObject &jsonObject) {
        region = jsonObject["region"].toString();
        bucketName = jsonObject["bucketName"].toString();
        bucketArn = jsonObject["bucketArn"].toString();
        objectCount = jsonObject["keys"].toInteger();
        size = jsonObject["size"].toInteger();
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODate);
    }
};


#endif // AWSMOCK_QT_UI_S3_BUCKET_COUNTER_H
