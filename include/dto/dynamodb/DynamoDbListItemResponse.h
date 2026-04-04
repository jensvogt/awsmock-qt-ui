#ifndef AWSMOCK_QT_UI_DYNAMODB_LIST_ITEM_RESPONSE_H
#define AWSMOCK_QT_UI_DYNAMODB_LIST_ITEM_RESPONSE_H

// Qt includes
#include <QList>
#include <QMap>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

// AwsMock includes
#include <utils/Logging.h>
#include <dto/dynamodb/DynamoDbTableCounter.h>
#include <dto/dynamodb/DynamoDbItem.h>

struct DynamoDbListItemResponse {

    QString tableName;

    long total{};

    long scannedCount{};

    QList<DynamoDbItem> items;

    void FromJson(const QJsonDocument &jsonDoc) {
        total = jsonDoc["total"].toInt();

        for (QJsonArray jArray = jsonDoc["itemCounters"].toArray(); const auto &element: jArray) {
            DynamoDbItem item;
            item.FromJson(element.toObject());
            items.append(item);
        }
    }
};

#endif // AWSMOCK_QT_UI_DYNAMODB_LIST_ITEM_RESPONSE_H
