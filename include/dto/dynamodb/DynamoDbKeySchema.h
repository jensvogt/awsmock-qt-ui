#ifndef AWSMOCK_QT_UI_DYNAMODB_KEY_SCHEMA_H
#define AWSMOCK_QT_UI_DYNAMODB_KEY_SCHEMA_H

// Qt includes
#include <QString>

// AwsMock includes
#include <dto/dynamodb/DynamoDbAttribute.h>

struct DynamoDbKeySchema {

    QString attributeName;
    QString keyType;

    void FromJsonObject(const QJsonObject &jsonObject) {
        attributeName = jsonObject["AttributeName"].toString();
        keyType = jsonObject["KeyType"].toString();
    }

    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject result;
        result["AttributeName"] = attributeName;
        result["KeyType"] = keyType;
        return result;
    }
};

#endif // AWSMOCK_QT_UI_DYNAMODB_KEY_SCHEMA_H
