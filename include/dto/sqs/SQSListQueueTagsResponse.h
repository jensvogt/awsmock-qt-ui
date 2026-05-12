#ifndef AWSMOCK_QT_UI_SQS_LIST_QUEUE_TAGS_RESPONSE_H
#define AWSMOCK_QT_UI_SQS_LIST_QUEUE_TAGS_RESPONSE_H

// Qt includes
#include <QJsonArray>
#include <QJsonDocument>

// Awsmock includes
#include <dto/sqs/SQSQueueLambdaTriggerCounter.h>
#include <dto/sqs/SQSMessageAttribute.h>

struct SQSListQueueTagsResponse {
    long total{};

    QMap<QString, QString> tags;

    void FromJson(const QJsonDocument &jsonDoc) {

        for (QJsonObject jObject = jsonDoc["tags"].toObject(); const auto &key: jObject.keys()) {
            tags[key] = jObject[key].toString();
        }
    }
};

#endif // AWSMOCK_QT_UI_SQS_LIST_QUEUE_TAGS_RESPONSE_H
