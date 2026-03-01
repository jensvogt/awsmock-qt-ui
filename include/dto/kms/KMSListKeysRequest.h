#ifndef AWSMOCK_QT_UI_KMS_LIST_KEYS_REQUEST_H
#define AWSMOCK_QT_UI_KMS_LIST_KEYS_REQUEST_H

// Qt includes
#include <QList>
#include <QMap>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

// AwsMock includes
#include <dto/kms/KeyCounter.h>

struct KMSListKeysRequest {

    /**
     * Key ID
     */
    QString keyId;

    /**
     * Arn
     */
    QString arn;

    /**
     * Key specification
     */
    KeySpec keySpec = KeySpec::SYMMETRIC_DEFAULT;

    /**
     * Key usage
     */
    KeyUsage keyUsage = KeyUsage::ENCRYPT_DECRYPT;

    /**
     * Key state
     */
    KeyState keyState = KeyState::UNAVAILABLE;

    /**
     * Multi region
     */
    bool multiRegion = false;

    /**
     * Description
     */
    QString description;

    /**
     * Creation date
     */
    QDateTime created;

    /**
     * Modification date
     */
    QDateTime modified;

    void FromJson(const QJsonDocument &jsonDoc) {
        keyId = jsonDoc["keyId"].toString();
        arn = jsonDoc["keyArn"].toString();
        keySpec = KeySpecFromString(jsonDoc["keySpec"].toString());
        // scannedCount = jsonDoc["ScannedCount"].toInt();
        //
        // for (QJsonArray jArray = jsonDoc["Items"].toArray(); const auto &element: jArray) {
        //     items.append(JsonUtils::WriteJsonToString(element.toObject()));
        // }
    }
};

#endif // AWSMOCK_QT_UI_KMS_LIST_KEYS_REQUEST_H
