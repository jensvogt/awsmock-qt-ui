#ifndef AWSMOCK_QT_UI_SNS_SEND_MESSAGE_RESPONSE_H
#define AWSMOCK_QT_UI_SNS_SEND_MESSAGE_RESPONSE_H

// Qt includes
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QXmlStreamReader>

// AwsMock includes
#include <dto/sns/SNSMessageAttribute.h>

struct SNSSendMessageResponse {
    QString requestId;

    QString messageId;

    QString md5Body;

    QString md5MessageAttributes;

    QString md5MessageSystemAttributes;

    QString sequenceNumber;

    QMap<QString, SNSMessageAttribute> messageAttributes;

    void FromJson(const QJsonDocument &jsonDoc) {
        messageId = jsonDoc["MessageId"].toString();
        md5Body = jsonDoc["MD5Body"].toString();
        md5MessageAttributes = jsonDoc["MD5MessageAttributes"].toString();
        md5MessageSystemAttributes = jsonDoc["MD5MessageSystemAttributes"].toString();
        sequenceNumber = jsonDoc["SequenceNumber"].toString();
    }

    [[nodiscard]] QByteArray ToJson() const {
        QJsonObject jObject;
        jObject["MessageId"] = messageId;
        jObject["MD5Body"] = md5Body;
        jObject["MD5MessageAttributes"] = md5MessageAttributes;
        jObject["MD5MessageSystemAttributes"] = md5MessageSystemAttributes;
        jObject["SequenceNumber"] = sequenceNumber;
        const QJsonDocument jDoc(jObject);
        return jDoc.toJson();
    }

    void FromXml(const QString &xmlString) {
        QXmlStreamReader xml(xmlString);
        while (!xml.atEnd()) {
            xml.readNext();

            if (xml.isStartElement() && xml.name() == "MessageId") {
                messageId = xml.readElementText();
            }
            if (xml.isStartElement() && xml.name() == "RequestId") {
                requestId = xml.readElementText();
            }
        }
    }
};

#endif // AWSMOCK_QT_UI_SNS_SEND_MESSAGE_RESPONSE_H
