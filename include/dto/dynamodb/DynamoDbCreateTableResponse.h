#ifndef AWSMOCK_QT_UI_DYNAMODB_CREATE_TABLE_RESPONSE_H
#define AWSMOCK_QT_UI_DYNAMODB_CREATE_TABLE_RESPONSE_H

// Qt includes
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

struct DynamoDbCreateTableResponse {

    QString tableName;

    QString tableClass;

    QList<DynamoDbAttribute> attributes;

    QList<DynamoDbKeySchema> keySchema;

    void FromJson(const QJsonDocument &jsonDoc) {
        tableName = jsonDoc["TableName"].toString();
        tableClass = jsonDoc["TableClass"].toString();

        if (!jsonDoc["AttributeDefinitions"].isNull() && jsonDoc["AttributeDefinitions"].isArray() && !jsonDoc["AttributeDefinitions"].toArray().empty()) {
            for (const auto &element: jsonDoc["AttributeDefinitions"].toArray()) {
                DynamoDbAttribute attribute;
                attribute.FromJsonObject(element.toObject());
                attributes.append(attribute);
            }
        }
        if (!jsonDoc["KeySchema"].isNull() && jsonDoc["KeySchema"].isArray() && !jsonDoc["KeySchema"].toArray().empty()) {
            for (const auto &element: jsonDoc["KeySchema"].toArray()) {
                DynamoDbKeySchema keySchemaElement;
                keySchemaElement.FromJsonObject(element.toObject());
                keySchema.append(keySchemaElement);
            }
        }
    }

    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject result;
        result["TableName"] = tableName;
        result["TableClass"] = tableClass;

        if (!attributes.empty()) {
            QJsonArray jsonAttributes;
            for (auto it = attributes.constBegin(); it != attributes.constEnd(); ++it) {
                jsonAttributes.append(it->ToJsonObject());
            }
            result["Attributes"] = jsonAttributes;
        }
        if (!keySchema.empty()) {
            QJsonArray jsonKeySchema;
            for (auto it = keySchema.constBegin(); it != keySchema.constEnd(); ++it) {
                jsonKeySchema.append(it->ToJsonObject());
            }
            result["KeySchema"] = jsonKeySchema;
        }
        return result;
    }
};

#endif // AWSMOCK_QT_UI_DYNAMODB_CREATE_TABLE_RESPONSE_H
