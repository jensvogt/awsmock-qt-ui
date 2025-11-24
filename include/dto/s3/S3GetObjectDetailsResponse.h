#ifndef AWSMOCK_QT_UI_S3_GET_OBJECT_DETAILS_RESPONSE_H
#define AWSMOCK_QT_UI_S3_GET_OBJECT_DETAILS_RESPONSE_H

#include <QJsonObject>
#include <QMimeDatabase>

struct S3GetObjectDetailsResponse {
    QString region;

    QString bucketName;

    QString key;

    QString contentType;

    QMimeType mimeType;

    QString internalName;

    QByteArray body;

    long size;

    QString owner;

    QDateTime created;

    QDateTime modified;

    QMap<QString, QString> metadata;

    void FromJson(const QJsonObject &jsonObject) {
        region = jsonObject["region"].toString();
        bucketName = jsonObject["bucketName"].toString();
        key = jsonObject["key"].toString();
        contentType = jsonObject["contentType"].toString();
        internalName = jsonObject["internalName"].toString();
        size = jsonObject["size"].toInteger();
        owner = jsonObject["user"].toString();
        body = QByteArray::fromBase64(jsonObject["body"].toString().toUtf8());
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODate);

        if (!jsonObject["metadata"].isNull()) {
            for (const auto &metadataKey: jsonObject["metadata"].toObject().keys()) {
                metadata[metadataKey] = jsonObject["metadata"].toObject()[metadataKey].toString();
            }
        }

        // Get mime type
        const QMimeDatabase db;
        mimeType = db.mimeTypeForData(body);
        qDebug() << "Mime type: " << mimeType.name();
    }
};

#endif // AWSMOCK_QT_UI_S3_GET_OBJECT_DETAILS_RESPONSE_H
