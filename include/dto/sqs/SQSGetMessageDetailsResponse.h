#ifndef GET_SQS_MESSAGE_DETAILS_RESPONSE_H
#define GET_SQS_MESSAGE_DETAILS_RESPONSE_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <utils/JsonUtils.h>

struct SQSMessageAttribute {

    QString name;

    QString stringValue;

    QString stringListValue;

    QString dataType;

    void FromJson(const QString &name, QJsonObject jsonObject) {
        this->name = name;
        dataType = jsonObject["DataType"].toString();
        stringValue = jsonObject["StringValue"].toString();
        stringListValue = jsonObject["StringListValue"].toString();
    }
};

struct SQSAttribute {

    QString key;

    QString value;
};

struct SQSGetMessageDetailsResponse {

public:
    QString region;

    QString messageId;

    QString id;

    QString receiptHandle;

    QString body;

    QString md5Body;

    QString md5Attributes;

    QDateTime created;

    QDateTime modified;

    QList<SQSMessageAttribute> messageAttributes;

    QList<SQSAttribute> attributes;

    void FromJson(QJsonObject jsonObject) {
        region = jsonObject["Region"].toString();
        id = jsonObject["Id"].toString();
        messageId = jsonObject["MessageId"].toString();
        receiptHandle = jsonObject["ReceiptHandle"].toString();
        body = jsonObject["Body"].toString();
        md5Body = jsonObject["MD5OfBody"].toString();
        md5Attributes = jsonObject["MD5OfMessageAttributes"].toString();
        created = QDateTime::fromString(jsonObject["Created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["Modified"].toString(), Qt::ISODate);

        // Message attributes
        for(const QString &key : jsonObject["MessageAttributes"].toObject().keys()) {

            SQSMessageAttribute messageAttribute;
            messageAttribute.FromJson(key, jsonObject["MessageAttributes"].toObject().value(key).toObject());
            messageAttributes.append(messageAttribute);
        }

        // System attributes
        for(const QString &key : jsonObject["Attributes"].toObject().keys()) {

            SQSAttribute attribute;
            attribute.key = key;
            attribute.value =  jsonObject["Attributes"].toObject().value(key).toString();
            attributes.append(attribute);
        }
    }
};

#endif // GET_SQS_MESSAGE_DETAILS_RESPONSE_H
