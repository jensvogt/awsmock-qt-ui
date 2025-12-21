#ifndef AWSMOCK_QT_UI_DYNAMODB_ATTRIBUTE_H
#define AWSMOCK_QT_UI_DYNAMODB_ATTRIBUTE_H

// Qt includes
#include <QJsonObject>

struct DynamoDbAttribute {

    QString attributeName;
    QString attributeType;

    void FromJsonObject(const QJsonObject &jsonObject) {
        attributeName = jsonObject["AttributeName"].toString();
        attributeType = jsonObject["AttributeType"].toString();
    }

    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject result;
        result["AttributeName"] = attributeName;
        result["AttributeType"] = attributeType;
        return result;
    }
};

#endif // AWSMOCK_QT_UI_DYNAMODB_ATTRIBUTE_H
