#pragma once

// Qt includes
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

struct SQSQueueUpdateRequest {

    QString queueArn;

    QString deadLetterQueueArn;

    long retentionPeriod{};

    long maxMessageSize{};

    long visibilityTimeout{};

    long delay{};

    long maxRetries{};

    QString owner;

    void FromJson(const QJsonDocument &jsonDoc) {
        queueArn = jsonDoc["arn"].toString();
        deadLetterQueueArn = jsonDoc["deadLetterQueueArn"].toString();
        maxRetries = jsonDoc["maxRetries"].toInt();
        retentionPeriod = jsonDoc["retentionPeriod"].toInteger();
        visibilityTimeout = jsonDoc["visibilityTimeout"].toInteger();
        delay = jsonDoc["delay"].toInteger();
        owner = jsonDoc["owner"].toString();
    }

    [[nodiscard]] QByteArray ToJson() const {
        QJsonObject jObject;
        jObject["arn"] = queueArn;
        jObject["deadLetterQueueArn"] = deadLetterQueueArn;
        jObject["retentionPeriod"] = static_cast<qint64>(retentionPeriod);
        jObject["visibilityTimeout"] = static_cast<qint64>(visibilityTimeout);
        jObject["delay"] = static_cast<qint64>(delay);
        jObject["maxRetries"] = static_cast<qint64>(maxRetries);
        jObject["owner"] = owner;
        const QJsonDocument jDoc(jObject);
        return jDoc.toJson();
    }
};
