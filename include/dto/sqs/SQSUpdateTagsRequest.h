#ifndef AWSMOCK_QT_UI_SQS_UPDATE_TAGS_REQUEST_H
#define AWSMOCK_QT_UI_SQS_UPDATE_TAGS_REQUEST_H

// Qt includes
#include <QJsonArray>
#include <QJsonDocument>

// Awsmock includes
#include <dto/sqs/SQSQueueLambdaTriggerCounter.h>
#include <dto/sqs/SQSMessageAttribute.h>

struct SQSUpdateTagsRequest {

    QString queueUrl;

    QMap<QString, QString> tags;

    void FromJson(const QJsonDocument &jsonDoc) {

        for (QJsonObject jObject = jsonDoc["Tags"].toObject(); const auto &key: jObject.keys()) {
            tags[key] = jObject[key].toString();
        }
    }
};

#endif // AWSMOCK_QT_UI_SQS_UPDATE_TAGS_REQUEST_H
