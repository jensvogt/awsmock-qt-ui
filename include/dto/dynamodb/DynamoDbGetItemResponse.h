#ifndef AWSMOCK_QT_UI_DYNAMODB_GET_ITEM_RESPONSE_H
#define AWSMOCK_QT_UI_DYNAMODB_GET_ITEM_RESPONSE_H

// Qt includes
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

struct DynamoDbGetItemResponse {

    QString region;
    QString tableName;
    QString user;
    QString partitionKey;
    QString sortKey;
    QString attributes;
    QDateTime created;
    QDateTime modified;

    void FromJson(const QJsonDocument &jsonDoc) {
        region = jsonDoc.object()["region"].toString();
        tableName = jsonDoc.object()["tableName"].toString();
        user = jsonDoc.object()["user"].toString();
        partitionKey = jsonDoc.object()["partitionKey"].toString();
        sortKey = jsonDoc.object()["sortKey"].toString();
        attributes = JsonUtils::WriteJsonToString(jsonDoc.object()["itemCounter"].toObject()["attributes"].toObject());
        created = QDateTime::fromString(jsonDoc.object()["itemCounter"].toObject()["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonDoc.object()["itemCounter"].toObject()["modified"].toString(), Qt::ISODate);
    }
};

#endif // AWSMOCK_QT_UI_DYNAMODB_GET_ITEM_RESPONSE_H
