#ifndef SNS_GET_MESSAGE_DETAILS_RESPONSE_H
#define SNS_GET_MESSAGE_DETAILS_RESPONSE_H


#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <utils/JsonUtils.h>

#include "SNSMessageAttribute.h"

struct SNSAttribute {
    QString key;

    QString value;
};

struct SNSGetMessageDetailsResponse {
public:
    QString region;

    QString id;

    QString messageId;

    QString topicArn;

    QString message;

    QString contentType;

    QDateTime created;

    QDateTime modified;

    QList<SNSMessageAttribute> messageAttributes;

    void FromJson(QJsonObject jsonObject) {
        region = jsonObject["Region"].toString();
        id = jsonObject["Id"].toString();
        messageId = jsonObject["MessageId"].toString();
        topicArn = jsonObject["TopicArn"].toString();
        message = jsonObject["Message"].toString();
        contentType = jsonObject["ContentType"].toString();
        created = QDateTime::fromString(jsonObject["Created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["Modified"].toString(), Qt::ISODate);

        // Message attributes
        for (const QString &key: jsonObject["MessageAttributes"].toObject().keys()) {
            SNSMessageAttribute messageAttribute;
            messageAttribute.FromJson(jsonObject["MessageAttributes"].toObject().value(key).toObject());
            messageAttributes.append(messageAttribute);
        }
    }
};

#endif // SNS_GET_MESSAGE_DETAILS_RESPONSE_H
