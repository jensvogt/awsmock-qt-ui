#ifndef AWSMOCK_QT_UI_DYNAMODB_ATTRIBUTE_H
#define AWSMOCK_QT_UI_DYNAMODB_ATTRIBUTE_H

// Qt includes
#include <QJsonObject>

enum DynamoDbAttributeType {
    STRING_ATTRIBUTE_TYPE,
    STRING_SET_ATTRIBUTE_TYPE,
    NUMBER_ATTRIBUTE_TYPE,
    NUMBER_SET_ATTRIBUTE_TYPE,
    BINARY_ATTRIBUTE_TYPE,
    BINARY_SET_ATTRIBUTE_TYPE,
    MAP_ATTRIBUTE_TYPE,
    LIST_ATTRIBUTE_TYPE,
    BOOL_ATTRIBUTE_TYPE,
    NULL_VALUE_ATTRIBUTE_TYPE,
};

static std::map<DynamoDbAttributeType, QString> DynamoDbAttributeTypeNames{
    {STRING_ATTRIBUTE_TYPE, "S"},
    {STRING_SET_ATTRIBUTE_TYPE, "SS"},
    {NUMBER_ATTRIBUTE_TYPE, "N"},
    {NUMBER_SET_ATTRIBUTE_TYPE, "NS"},
    {BINARY_ATTRIBUTE_TYPE, "B"},
    {BINARY_SET_ATTRIBUTE_TYPE, "BS"},
    {MAP_ATTRIBUTE_TYPE, "M"},
    {LIST_ATTRIBUTE_TYPE, "L"},
    {BOOL_ATTRIBUTE_TYPE, "BOOL"},
    {NULL_VALUE_ATTRIBUTE_TYPE, "NULL"},
};

static std::map<DynamoDbAttributeType, QString> DynamoDbAttributeTypeDisplayNames{
    {STRING_ATTRIBUTE_TYPE, "String (S)"},
    {STRING_SET_ATTRIBUTE_TYPE, "String Set (SS)"},
    {NUMBER_ATTRIBUTE_TYPE, "Number (N)"},
    {NUMBER_SET_ATTRIBUTE_TYPE, "Number Set (NS)"},
    {BINARY_ATTRIBUTE_TYPE, "Binary (B)"},
    {BINARY_SET_ATTRIBUTE_TYPE, "Binary Set(BS)"},
    {MAP_ATTRIBUTE_TYPE, "Map (M)"},
    {LIST_ATTRIBUTE_TYPE, "List (L)"},
    {BOOL_ATTRIBUTE_TYPE, "Boolean (BOOL)"},
    {NULL_VALUE_ATTRIBUTE_TYPE, "Null (NULL)"},
};

[[maybe_unused]] static QString DynamoDbAttributeTypeToString(const DynamoDbAttributeType &attributeType) {
    return DynamoDbAttributeTypeNames[attributeType];
}

[[maybe_unused]] static DynamoDbAttributeType DynamoDbAttributeTypeFromString(const QString &attributeType) {
    for (auto &[fst, snd]: DynamoDbAttributeTypeNames) {
        if (snd == attributeType) {
            return fst;
        }
    }
    return STRING_ATTRIBUTE_TYPE;
}

[[maybe_unused]] static DynamoDbAttributeType DynamoDbAttributeTypeFromIndex(const int index) {
    int row = 0;
    for (const auto &fst: DynamoDbAttributeTypeNames | std::views::keys) {
        if (row == index) {
            return fst;
        }
        row++;
    }
    return STRING_ATTRIBUTE_TYPE;
}

[[maybe_unused]] static QStringList DynamoDbAttributeTypeStringList() {
    QStringList attributeTypes;
    for (auto &snd: DynamoDbAttributeTypeDisplayNames | std::views::values) {
        attributeTypes.append(snd);
    }
    return attributeTypes;
}

struct DynamoDbAttribute {

    QString attributeName;
    DynamoDbAttributeType attributeType;

    void FromJsonObject(const QJsonObject &jsonObject) {
        attributeName = jsonObject["AttributeName"].toString();
        attributeType = DynamoDbAttributeTypeFromString(jsonObject["AttributeType"].toString());
    }

    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject result;
        result["AttributeName"] = attributeName;
        result["AttributeType"] = DynamoDbAttributeTypeToString(attributeType);
        return result;
    }
};

#endif // AWSMOCK_QT_UI_DYNAMODB_ATTRIBUTE_H
