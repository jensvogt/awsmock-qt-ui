#pragma once

// Qt include
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

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
