//
// Created by vogje01 on 12/17/25.
//

#ifndef AWSMOCK_QT_UI_KMS_KEY_COUNTER_H
#define AWSMOCK_QT_UI_KMS_KEY_COUNTER_H

// Qt includes
#include <QString>
#include <QDateTime>

// Awsmock includes
#include <dto/kms/KeySpec.h>
#include <dto/kms/KeyUsage.h>
#include <dto/kms/KeyState.h>

struct KeyCounter {

    /**
     * AWS region
     */
    QString region;

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
     * Origin
     */
    QString origin;

    /**
     * Creation date
     */
    QDateTime created;

    /**
     * Modification date
     */
    QDateTime modified;

    void FromJson(const QJsonObject &jsonObject) {
        region = jsonObject["region"].toString();
        keyId = jsonObject["keyId"].toString();
        arn = jsonObject["keyArn"].toString();
        description = jsonObject["description"].toString();
        origin = jsonObject["origin"].toString();
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODate);
    }

    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject jsonObject;
        jsonObject["region"] = region;
        jsonObject["keyId"] = keyId;
        jsonObject["keyArn"] = arn;
        jsonObject["description"] = description;
        jsonObject["origin"] = origin;
        jsonObject["created"] = created.toString(Qt::ISODate);
        jsonObject["modified"] = modified.toString(Qt::ISODate);
        return jsonObject;
    }
};
#endif //AWSMOCK_QT_UI_KMS_KEY_COUNTER_H
