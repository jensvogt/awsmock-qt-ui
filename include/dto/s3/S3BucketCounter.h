#ifndef AWSMOCK_QT_UI_S3_BUCKET_COUNTER_H
#define AWSMOCK_QT_UI_S3_BUCKET_COUNTER_H

// Qt includes
#include <QDateTime>
#include <QJsonObject>

// Awsmock includes
#include <dto/s3/S3LambdaConfiguration.h>
#include <dto/s3/S3QueueConfiguration.h>
#include <dto/s3/S3TopicConfiguration.h>

struct S3BucketCounter {
    QString region;

    QString bucketName;

    QString bucketArn;

    QString owner;

    long objectCount{};

    long size{};

    S3QueueConfiguration queueConfiguration;

    S3TopicConfiguration topicConfiguration;

    S3LambdaConfiguration lambdaConfiguration;

    QDateTime created;

    QDateTime modified;

    void FromJson(const QJsonObject &jsonObject) {
        region = jsonObject["region"].toString();
        bucketName = jsonObject["bucketName"].toString();
        bucketArn = jsonObject["bucketArn"].toString();
        owner = jsonObject["owner"].toString();
        objectCount = jsonObject["keys"].toInteger();
        size = jsonObject["size"].toInteger();
        queueConfiguration.FromJson(jsonObject["queueConfiguration"].toObject());
        queueConfiguration.FromJson(jsonObject["queueConfiguration"].toObject());
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODate);
    }
};


#endif // AWSMOCK_QT_UI_S3_BUCKET_COUNTER_H
