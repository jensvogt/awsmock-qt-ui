#pragma once

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

[[maybe_unused]] static QString MessageAttributeDataTypeToString(const MessageAttributeDataType &messageAttributeDataType) {
    return MessageAttributeDataTypeNames.value(messageAttributeDataType, "String");
}

[[maybe_unused]] static MessageAttributeDataType MessageAttributeDataTypeFromString(const QString &messageAttributeDataType) {
    return MessageAttributeDataTypeNames.key(messageAttributeDataType, STRING);
}
