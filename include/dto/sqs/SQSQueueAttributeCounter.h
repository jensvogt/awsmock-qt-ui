#pragma once

// Qt includes
#include <QJsonObject>

struct SQSQueueAttributeCounter {

    QString key;

    QString value;

    void FromJson(const QJsonObject &jsonObject) {
        key = jsonObject["attributeKey"].toString();
        value = jsonObject["attributeValue"].toString();
    }
};
