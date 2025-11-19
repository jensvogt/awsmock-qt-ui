#ifndef AWSMOCK_QT_UI_SEND_MESSAGE_RESPONSE_H
#define AWSMOCK_QT_UI_SEND_MESSAGE_RESPONSE_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <dto/sqs/SQSMessageAttribute.h>

struct SQSSendMessageResponse {
    QString region;

    QString messageId;

    QString md5Body;

    QString md5MessageAttributes;

    QString md5MessageSystemAttributes;

    QString sequenceNumber;

    QMap<QString, MessageAttribute> messageAttributes;

    void FromJson(const QJsonDocument &jsonDoc) {
        region = jsonDoc["Region"].toString();
        messageId = jsonDoc["MessageId"].toString();
        md5Body = jsonDoc["MD5Body"].toString();
        md5MessageAttributes = jsonDoc["MD5MessageAttributes"].toString();
        md5MessageSystemAttributes = jsonDoc["MD5MessageSystemAttributes"].toString();
        sequenceNumber = jsonDoc["SequenceNumber"].toString();
    }

    [[nodiscard]] QByteArray ToJson() const {
        QJsonObject jObject;
        jObject["Region"] = region;
        jObject["MessageId"] = messageId;
        jObject["MD5Body"] = md5Body;
        jObject["MD5MessageAttributes"] = md5MessageAttributes;
        jObject["MD5MessageSystemAttributes"] = md5MessageSystemAttributes;
        jObject["SequenceNumber"] = sequenceNumber;
        const QJsonDocument jDoc(jObject);
        return jDoc.toJson();
    }
};

#endif // AWSMOCK_QT_UI_SEND_MESSAGE_RESPONSE_H
