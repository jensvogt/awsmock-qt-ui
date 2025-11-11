#ifndef AWSMOCK_QT_UI_LIST_BUCKET_RESULT_H
#define AWSMOCK_QT_UI_LIST_BUCKET_RESULT_H

#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <dto/s3/S3BucketCounter.h>

struct S3ListBucketResult {
    long total;

    QList<S3BucketCounter> bucketCounters;

    void FromJson(const QJsonDocument &jsonDoc) {

        QJsonArray jArray = jsonDoc["bucketCounters"].toArray();

        for (const auto &element: jArray) {
            S3BucketCounter bucketCounter;
            bucketCounter.FromJson(element.toObject());
            bucketCounters.append(bucketCounter);
        }
    }
};

#endif // AWSMOCK_QT_UI_LIST_BUCKET_RESULT_H
