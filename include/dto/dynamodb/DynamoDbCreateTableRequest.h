#ifndef AWSMOCK_QT_UI_DYNAMODB_CREATE_TABLE_REQUEST_H
#define AWSMOCK_QT_UI_DYNAMODB_CREATE_TABLE_REQUEST_H

// Qt includes
#include <QJsonArray>
#include <QJsonDocument>

// AwsMock includes
#include <dto/dynamodb/DynamoDbAttribute.h>
#include <dto/dynamodb/DynamoDbKeySchema.h>

enum DynamoDbTableClass {
    STANDARD,
    STANDARD_INFREQUENT_ACCESS
};

static std::map<DynamoDbTableClass, QString> DynamoDbTableClassNames{
    {STANDARD, "STANDARD"},
    {STANDARD_INFREQUENT_ACCESS, "STANDARD_INFREQUENT_ACCESS"},
};

[[maybe_unused]] static QString DynamoDbTableClassToString(const DynamoDbTableClass &tableClass) {
    return DynamoDbTableClassNames[tableClass];
}

[[maybe_unused]] static DynamoDbTableClass DynamoDbTableClassFromString(const QString &tableClass) {
    for (auto &[fst, snd]: DynamoDbTableClassNames) {
        if (snd == tableClass) {
            return fst;
        }
    }
    return STANDARD;
}

struct DynamoDbCreateTableRequest {

    QString tableName;

    DynamoDbTableClass tableClass = STANDARD;

    QList<DynamoDbAttribute> attributes;

    QList<DynamoDbKeySchema> keySchema;

    void FromJson(const QJsonDocument &jsonDoc) {
        tableName = jsonDoc["TableName"].toString();
        tableClass = DynamoDbTableClassFromString(jsonDoc["TableClass"].toString());

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
        result["TableClass"] = DynamoDbTableClassToString(tableClass);

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

#endif // AWSMOCK_QT_UI_DYNAMODB_CREATE_TABLE_REQUEST_H
