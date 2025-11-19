#ifndef AWSMOCK_QT_UI_SQS_QUEUE_ATTRIBUTE_COUNTER_H
#define AWSMOCK_QT_UI_SQS_QUEUE_ATTRIBUTE_COUNTER_H

#include <QDateTime>
#include <QJsonObject>

struct SQSQueueAttributeCounter {

    QString key;

    QString value;

    void FromJson(const QJsonObject &jsonObject) {
        key = jsonObject["attributeKey"].toString();
        value = jsonObject["attributeValue"].toString();
    }
};


#endif // AWSMOCK_QT_UI_SQS_QUEUE_ATTRIBUTE_COUNTER_H
