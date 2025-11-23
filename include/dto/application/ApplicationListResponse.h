#ifndef AWSMOCK_QT_UI_APPLICATION_LIST_RESPONSE_H
#define AWSMOCK_QT_UI_APPLICATION_LIST_RESPONSE_H

#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <dto/sns/SNSTopicCounter.h>

#include <dto/application/ApplicationCounter.h>

struct ApplicationListResponse {
    long total{};

    QList<ApplicationCounter> applicationCounters;

    void FromJson(const QJsonDocument &jsonDoc) {
        for (QJsonArray jArray = jsonDoc["applications"].toArray(); const auto &element: jArray) {
            ApplicationCounter applicationCounter;
            applicationCounter.FromJson(element.toObject());
            applicationCounters.append(applicationCounter);
        }
    }
};

#endif // AWSMOCK_QT_UI_APPLICATION_LIST_RESPONSE_H
