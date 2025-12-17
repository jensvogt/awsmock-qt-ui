#ifndef AWSMOCK_QT_UI_SNS_SEND_MESSAGE_REQUEST_H
#define AWSMOCK_QT_UI_SNS_SEND_MESSAGE_REQUEST_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <dto/sns/SNSMessageAttribute.h>

struct SNSSendMessageRequest {
    QString region;

    QString topicArn;

    QString body;

    QString contentType;

    long delaySeconds{};

    std::map<QString, SNSMessageAttribute> messageAttributes;

    void FromJson(const QJsonDocument &jsonDoc) {
        region = jsonDoc["Region"].toString();
        topicArn = jsonDoc["TopicArn"].toString();
        body = jsonDoc["MessageBody"].toString();
        contentType = jsonDoc["ContentType"].toString();
        delaySeconds = jsonDoc["DelaySeconds"].toInt();
    }

    [[nodiscard]] QByteArray ToJson() const {
        QJsonObject jObject;
        jObject["Region"] = region;
        jObject["TopicArn"] = topicArn;
        jObject["MessageBody"] = body;
        jObject["ContentType"] = contentType;
        jObject["DelaySeconds"] = static_cast<qint64>(delaySeconds);

        QJsonObject jAttributeObject;
        for (const auto &[fst, snd]: messageAttributes) {
            QJsonObject attributeObject;
            attributeObject["DataType"] = "String";
            attributeObject["StringValue"] = snd.stringValue;
            jAttributeObject[fst] = attributeObject;
        }
        jObject["MessageAttributes"] = jAttributeObject;

        const QJsonDocument jDoc(jObject);
        return jDoc.toJson();
    }
};

#endif // AWSMOCK_QT_UI_SNS_SEND_MESSAGE_REQUEST_H
