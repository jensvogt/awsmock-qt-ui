//
// Created by vogje01 on 11/13/25.
//

#ifndef AWSMOCK_QT_UI_S3_OBJECT_UPLOAD_REQUEST_H
#define AWSMOCK_QT_UI_S3_OBJECT_UPLOAD_REQUEST_H

#include <QJsonObject>
#include <QJsonArray>

#include <utils/JsonUtils.h>

struct S3ObjectUploadRequest {
    QString bucketArn;

    QString bucketName;

    QString key;

    QString contentType;

    QString content;

    long size;

    void FromJson(const QJsonObject &jsonObject) {
        bucketArn = jsonObject["bucketArn"].toString();
        bucketName = jsonObject["bucketName"].toString();
        key = jsonObject["key"].toString();
        contentType = jsonObject["contentType"].toString();
        content = jsonObject["content"].toString();
        size = jsonObject["size"].toInt();
    }
};
#endif //AWSMOCK_QT_UI_S3_OBJECT_UPLOAD_REQUEST_H
