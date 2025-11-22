#ifndef AWSMOCK_QT_UI_SQS_MESSAGE_ATTRIBUTE_H
#define AWSMOCK_QT_UI_SQS_MESSAGE_ATTRIBUTE_H

// C++ standard includes
#include <string>

#include <dto/sqs/SQSMessageAttributeDataType.h>

struct MessageAttribute final {
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
     * Logical data type
     */
    MessageAttributeDataType dataType{};

    void FromJson(const QJsonDocument &jsonDoc) {
        stringValue = jsonDoc["stringValue"].toString();
        dataType = MessageAttributeDataTypeFromString(jsonDoc["DataType"].toString().toStdString());
        //binaryValue = (jsonDoc["binaryValue"].toString()..c_str();
    }

    void FromJson(const QJsonObject &jsonObject) {
        stringValue = jsonObject["StringValue"].toString();
        dataType = MessageAttributeDataTypeFromString(jsonObject["DataType"].toString().toStdString());
        //binaryValue = (jsonDoc["binaryValue"].toString()..c_str();
    }

    [[nodiscard]] QByteArray ToJson() const {
        QJsonObject jObject;
        jObject["StringValue"] = stringValue;
        jObject["DataType"] = QString::fromStdString(MessageAttributeDataTypeToString(dataType));
        const QJsonDocument jDoc(jObject);
        return jDoc.toJson();
    }
};

#endif// AWSMOCK_QT_UI_SQS_MESSAGE_ATTRIBUTE_H
