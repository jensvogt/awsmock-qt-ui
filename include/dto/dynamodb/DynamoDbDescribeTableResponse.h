#ifndef AWSMOCK_QT_UI_DYNAMODB_DESCRIBE_TABLE_RESPONSE_H
#define AWSMOCK_QT_UI_DYNAMODB_DESCRIBE_TABLE_RESPONSE_H

// Qt includes
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

// AwsMock includes
#include <dto/dynamodb/DynamoDbAttribute.h>
#include <dto/dynamodb/DynamoDbKeySchema.h>

struct DynamoDbDescribeTableResponse {

    QString region;

    QString tableName;

    QString tableClass;

    QString tableArn;

    QString status;

    long size;

    long itemCount;

    bool deletionProtection;

    QList<DynamoDbAttribute> attributes;

    QList<DynamoDbKeySchema> keySchema;

    DynamoDbProvisionedThroughput provisionedThroughput;

    void FromJson(const QJsonDocument &jsonDoc) {
        region = jsonDoc["Region"].toString();
        tableName = jsonDoc["TableName"].toString();
        tableClass = jsonDoc["TableClass"].toString();
        tableArn = jsonDoc["TableArn"].toString();
        size = jsonDoc["TableSizeBytes"].toInteger();
        itemCount = jsonDoc["ItemCount"].toInteger();
        status = jsonDoc["TableStatus"].toString();
        deletionProtection = jsonDoc["DeletionProtectionEnabled"].toBool();
        provisionedThroughput.FromJsonObject(jsonDoc["ProvisionedThroughput"].toObject());

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
        result["Region"] = region;
        result["TableName"] = tableName;
        result["TableClass"] = tableClass;
        result["TableSizeBytes"] = static_cast<qint64>(size);
        result["ItemCount"] = static_cast<qint64>(itemCount);
        result["ProvisionedThroughput"] = provisionedThroughput.ToJsonObject();

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

#endif // AWSMOCK_QT_UI_DYNAMODB_DESCRIBE_TABLE_RESPONSE_H
