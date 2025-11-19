#ifndef AWSMOC_QT_UI_QUEUE_UPDATE_REQUEST_H
#define AWSMOC_QT_UI_QUEUE_UPDATE_REQUEST_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

struct SQSQueueUpdateRequest {

public:

    QString queueArn;

    long retentionPeriod;

//    long maxMessageSize;

    long visibilityTimeout;

    long delay;

    void FromJson(const QJsonDocument& jsonDoc) {
        queueArn = jsonDoc["queueArn"].toString();
        retentionPeriod = jsonDoc["retentionPeriod"].toInteger();
        visibilityTimeout = jsonDoc["visibilityTimeout"].toInteger();
        delay = jsonDoc["delay"].toInteger();
    }

    [[nodiscard]] QByteArray ToJson() const {
        QJsonObject jObject;
        jObject["queueArn"] = queueArn;
        jObject["retentionPeriod"] = static_cast<qint64>(retentionPeriod);
        jObject["visibilityTimeout"] = static_cast<qint64>(visibilityTimeout);
        jObject["delay"] = static_cast<qint64>(delay);
        const QJsonDocument jDoc(jObject);
        return jDoc.toJson();
    }
};

#endif // AWSMOC_QT_UI_QUEUE_UPDATE_REQUEST_H
