#ifndef AWSMOC_QT_UI_QUEUE_UPDATE_REQUEST_H
#define AWSMOC_QT_UI_QUEUE_UPDATE_REQUEST_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <dto/sqs/SQSMessageAttribute.h>

struct SQSSendMessageRequest {
    QString queueUrl;

    QString body;

    QString contentType;

    long delaySeconds;

    std::map<std::string, MessageAttribute> messageAttributes;

    void FromJson(const QJsonDocument &jsonDoc) {
        queueUrl = jsonDoc["queueUrl"].toString();
        body = jsonDoc["body"].toString();
        contentType = jsonDoc["contentType"].toString();
        delaySeconds = jsonDoc["delaySeconds"].toInt();
    }

    [[nodiscard]] QByteArray ToJson() const {
        QJsonObject jObject;
        jObject["queueUrl"] = queueUrl;
        jObject["body"] = body;
        jObject["contentType"] = contentType;
        jObject["delaySeconds"] = static_cast<qint64>(delaySeconds);
        const QJsonDocument jDoc(jObject);
        return jDoc.toJson();
    }
};

#endif // AWSMOC_QT_UI_QUEUE_UPDATE_REQUEST_H
