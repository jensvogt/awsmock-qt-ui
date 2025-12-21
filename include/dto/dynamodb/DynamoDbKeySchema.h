#ifndef AWSMOCK_QT_UI_DYNAMODB_KEY_SCHEMA_H
#define AWSMOCK_QT_UI_DYNAMODB_KEY_SCHEMA_H

// Qt includes
#include <QString>

// AwsMock includes
#include <dto/dynamodb/DynamoDbAttribute.h>

enum DynamoDbKeyType {
    HASH,
    RANGE
};

static std::map<DynamoDbKeyType, QString> DynamoDbKeyTypeNames{
    {HASH, "HASH"},
    {RANGE, "RANGE"},
};

[[maybe_unused]] static QString DynamoDbKeyTypeToString(const DynamoDbKeyType &keyType) {
    return DynamoDbKeyTypeNames[keyType];
}

[[maybe_unused]] static DynamoDbKeyType DynamoDbKeyTypeFromString(const QString &keyType) {
    for (auto &[fst, snd]: DynamoDbKeyTypeNames) {
        if (snd == keyType) {
            return fst;
        }
    }
    return HASH;
}

[[maybe_unused]] static DynamoDbKeyType DynamoDbKeySchemaTypeFromIndex(const int index) {
    int row = 0;
    for (const auto &fst: DynamoDbKeyTypeNames | std::views::keys) {
        if (row == index) {
            return fst;
        }
        row++;
    }
    return HASH;
}

[[maybe_unused]] static QStringList DynamoDbKeyTypeStringList() {
    QStringList keyTypes;
    for (auto &snd: DynamoDbKeyTypeNames | std::views::values) {
        keyTypes.append(snd);
    }
    return keyTypes;
}

struct DynamoDbKeySchema {

    QString attributeName;
    DynamoDbKeyType keyType;

    void FromJsonObject(const QJsonObject &jsonObject) {
        attributeName = jsonObject["AttributeName"].toString();
        keyType = DynamoDbKeyTypeFromString(jsonObject["KeyType"].toString());
    }

    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject result;
        result["AttributeName"] = attributeName;
        result["KeyType"] = DynamoDbKeyTypeToString(keyType);
        return result;
    }
};

#endif // AWSMOCK_QT_UI_DYNAMODB_KEY_SCHEMA_H
