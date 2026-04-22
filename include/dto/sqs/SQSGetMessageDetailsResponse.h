#ifndef GET_SQS_MESSAGE_DETAILS_RESPONSE_H
#define GET_SQS_MESSAGE_DETAILS_RESPONSE_H


#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <utils/JsonUtils.h>

struct MessageAttribute {

    QString name;

    QString stringValue;

    QString stringListValue;

    QString dataType;

    void FromJson(const QString &attributeName, QJsonObject jsonObject) {
        name = attributeName;
        dataType = jsonObject["DataType"].toString();
        stringValue = jsonObject["StringValue"].toString();
        stringListValue = jsonObject["StringListValue"].toString();
    }

    [[nodiscard]]
    QJsonObject ToJsonObject() const {
        QJsonObject attribute;
        attribute.insert("Name", name);
        attribute.insert("DataType", dataType);
        attribute.insert("StringValue", stringValue);
        attribute.insert("StringListValue", stringListValue);
        return attribute;
    }
};

struct SQSAttribute {
    QString key;

    QString value;
};

struct SQSGetMessageDetailsResponse {
    QString region;

    QString queueName;

    QString messageId;

    QString id;

    QString receiptHandle;

    QString body;

    QString md5OfBody;

    QString md5OfMessageAttributes;

    QString md5OfSystemAttributes;

    QString contentType;

    int retries{};

    QDateTime created;

    QDateTime modified;

    QList<MessageAttribute> messageAttributes;

    QList<SQSAttribute> attributes;

    void FromJson(QJsonObject jsonObject) {
        region = jsonObject["region"].toString();
        queueName = jsonObject["queueName"].toString();
        id = jsonObject["id"].toString();
        messageId = jsonObject["messageId"].toString();
        receiptHandle = jsonObject["receiptHandle"].toString();
        body = jsonObject["body"].toString();
        md5OfBody = jsonObject["md5OfBody"].toString();
        md5OfMessageAttributes = jsonObject["md5OfMessageAttributes"].toString();
        md5OfSystemAttributes = jsonObject["md5OfSystemAttributes"].toString();
        contentType = jsonObject["contentType"].toString();
        retries = jsonObject["retries"].toInt();
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODate);

        // Message attributes
        for (const QString &key: jsonObject["messageAttributes"].toObject().keys()) {

            MessageAttribute messageAttribute;
            messageAttribute.FromJson(key, jsonObject["messageAttributes"].toObject().value(key).toObject());
            messageAttributes.append(messageAttribute);
        }

        // System attributes
        for (const QString &key: jsonObject["attributes"].toObject().keys()) {

            SQSAttribute attribute;
            attribute.key = key;
            attribute.value = jsonObject["attributes"].toObject().value(key).toString();
            attributes.append(attribute);
        }
    }
};

#endif // GET_SQS_MESSAGE_DETAILS_RESPONSE_H
