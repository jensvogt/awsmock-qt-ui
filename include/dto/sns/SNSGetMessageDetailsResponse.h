#pragma once

// Qt includes
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

// Awsmock includes
#include <dto/sns/SNSMessageAttribute.h>
#include <utils/JsonUtils.h>

struct SNSAttribute {
    QString key;

    QString value;
};

struct SNSGetMessageDetailsResponse {

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
