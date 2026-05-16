#ifndef AWSMOCK_QT_UI_SQS_IS_DLQ_RESPONSE_H
#define AWSMOCK_QT_UI_SQS_IS_DLQ_RESPONSE_H

#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <dto/sqs/SQSQueueCounter.h>

struct SQSIsDlqResponse {

    bool isDlq{};

    QList<QString> mainQueues;

    void FromJson(const QJsonDocument &jsonDoc) {

        isDlq = jsonDoc.object()["isDlq"].toBool();
        for (QJsonArray jArray = jsonDoc["mainQueues"].toArray(); const auto &element: jArray) {
            mainQueues.append(element.toString());
        }
    }
};

#endif // AWSMOCK_QT_UI_SQS_IS_DLQ_RESPONSE_H
