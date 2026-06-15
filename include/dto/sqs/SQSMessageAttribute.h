#pragma once

// Awsmock includes
#include <dto/sqs/SQSMessageAttributeDataType.h>

struct SQSMessageAttribute final {
    /**
     * Message attribute string value
     */
    QString stringValue = {};

    /**
     * Message attribute number value
     */
    QList<QString> stringListValues = {};

    /**
     * Message attribute binary value
     */
    unsigned char *binaryValue = nullptr;

    /**
     * Logical data contentType
     */
    MessageAttributeDataType dataType{};

    void FromJson(const QJsonDocument &jsonDoc) {
        stringValue = jsonDoc["stringValue"].toString();
        dataType = MessageAttributeDataTypeFromString(jsonDoc["DataType"].toString());
        //binaryValue = (jsonDoc["binaryValue"].toString()..c_str();
    }

    void FromJson(const QJsonObject &jsonObject) {
        stringValue = jsonObject["StringValue"].toString();
        dataType = MessageAttributeDataTypeFromString(jsonObject["DataType"].toString());
        //binaryValue = (jsonDoc["binaryValue"].toString()..c_str();
    }

    [[nodiscard]] QByteArray ToJson() const {
        QJsonObject jObject;
        jObject["StringValue"] = stringValue;
        jObject["DataType"] = MessageAttributeDataTypeToString(dataType);
        const QJsonDocument jDoc(jObject);
        return jDoc.toJson();
    }
};
