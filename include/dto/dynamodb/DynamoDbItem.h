//
// Created by vogje01 on 12/22/25.
//

#ifndef AWSMOCK_QT_UI_DYNAMODB_ITEM_H
#define AWSMOCK_QT_UI_DYNAMODB_ITEM_H

// Qt includes
#include <QString>

// Awsmock includes
#include <utils/DateTimeUtils.h>
#include <dto/dynamodb/DynamoDbAttribute.h>

struct DynamoDbItem {

    /**
     * Table name
     */
    QString tableName;

    /**
     * Partition key
     */
    QString partitionKey;

    /**
     * Sort key
     */
    QString sortKey;

    /**
     * Item size in bytes
     */
    long size = 0;

    /**
     * Attributes
     */
    QString attributes;

    /**
     * Create timestamp
     */
    QDateTime created;

    /**
     * Create timestamp
     */
    QDateTime modified;

    void FromJson(const QJsonObject &jsonObject) {
        tableName = jsonObject["tableName"].toString();
        partitionKey = jsonObject["partitionKey"].toString();
        sortKey = jsonObject["sortKey"].toString();
        size = jsonObject["size"].toInteger();
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODate);
        attributes = JsonUtils::WriteJsonToString(jsonObject["attributes"].toObject());
    }
};
#endif //AWSMOCK_QT_UI_DYNAMODB_ITEM_H
