#pragma once

// Qt includes
#include <QJsonObject>

struct SQSQueueLambdaTriggerCounter {
    QString uuid;

    QString arn;

    bool enabled = false;

    void FromJson(const QJsonObject &jsonObject) {
        uuid = jsonObject["uuid"].toString();
        arn = jsonObject["arn"].toString();
        enabled = jsonObject["enabled"].toBool();
    }
};
