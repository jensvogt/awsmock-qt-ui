#ifndef AWSMOCK_QT_UI_DYNAMODB_LIST_ITEM_RESPONSE_H
#define AWSMOCK_QT_UI_DYNAMODB_LIST_ITEM_RESPONSE_H

// Qt includes
#include <QList>
#include <QMap>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

// AwsMock includes
#include <dto/dynamodb/DynamoDbTableCounter.h>

struct Item {
    QString type;
    QString value;
};

struct DynamoDbListItemResponse {

    QString tableName;

    long count{};

    long scannedCount{};

    QList<QString> items;

    void FromJson(const QJsonDocument &jsonDoc) {
        tableName = jsonDoc["TableName"].toString();
        count = jsonDoc["Count"].toInt();
        scannedCount = jsonDoc["ScannedCount"].toInt();

        for (QJsonArray jArray = jsonDoc["Items"].toArray(); const auto &element: jArray) {
            items.append(JsonUtils::WriteJsonToString(element.toObject()));
        }
    }
};

#endif // AWSMOCK_QT_UI_DYNAMODB_LIST_ITEM_RESPONSE_H
