#ifndef AWSMOCK_QT_UI_DYNAMODB_LIST_TABLE_RESPONSE_H
#define AWSMOCK_QT_UI_DYNAMODB_LIST_TABLE_RESPONSE_H

// Qt includes
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

// AwsMock includes
#include <dto/dynamodb/DynamoDbTableCounter.h>

struct DynamoDbListTableResponse {
    long total{};

    QList<DynamoDbTableCounter> tableCounters;

    void FromJson(const QJsonDocument &jsonDoc) {

        total = jsonDoc.object()["total"].toInteger();

        for (QJsonArray jArray = jsonDoc["tableCounters"].toArray(); const auto &element: jArray) {
            DynamoDbTableCounter tableCounter;
            tableCounter.FromJson(element.toObject());
            tableCounters.append(tableCounter);
        }
    }
};

#endif // AWSMOCK_QT_UI_DYNAMODB_LIST_TABLE_RESPONSE_H
