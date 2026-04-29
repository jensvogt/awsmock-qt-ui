#ifndef AWSMOCK_QT_UI_SQS_MESSAGE_ATTRIBUTE_DATA_TYPE_H
#define AWSMOCK_QT_UI_SQS_MESSAGE_ATTRIBUTE_DATA_TYPE_H

enum MessageAttributeDataType {
    STRING,
    NUMBER,
    BINARY
};

static QMap<MessageAttributeDataType, QString> MessageAttributeDataTypeNames{
    {STRING, "String"},
    {NUMBER, "Number"},
    {BINARY, "Binary"},
};

[[maybe_unused]]
static QString MessageAttributeDataTypeToString(const MessageAttributeDataType &messageAttributeDataType) {
    return MessageAttributeDataTypeNames[messageAttributeDataType];
}

[[maybe_unused]]
static MessageAttributeDataType MessageAttributeDataTypeFromString(const QString &messageAttributeDataType) {
    return MessageAttributeDataTypeNames.key(messageAttributeDataType, STRING);
}

#endif// AWSMOCK_QT_UI_SQS_MESSAGE_ATTRIBUTE_DATA_TYPE_H
