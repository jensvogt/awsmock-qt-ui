#ifndef AWSMOCK_QT_UI_DYNAMODB_EXPORT_ITEMS_RESPONSE_H
#define AWSMOCK_QT_UI_DYNAMODB_EXPORT_ITEMS_RESPONSE_H

// Qt includes
#include <QList>
#include <QMap>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

// AwsMock includes
#include <dto/dynamodb/DynamoDbTableCounter.h>

struct DynamoDbExportItemsResponse {

    QList<QString> items;

    void FromJson(const QJsonDocument &jsonDoc) {
        for (QJsonArray jArray = jsonDoc["Items"].toArray(); const auto &element: jArray) {
            items.append(JsonUtils::WriteJsonToString(element.toObject()));
        }
    }
};

#endif // AWSMOCK_QT_UI_DYNAMODB_EXPORT_ITEMS_RESPONSE_H
