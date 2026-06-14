#pragma once

// Qt includes
#include <QJsonArray>
#include <QJsonDocument>

struct SQSListQueueTagsResponse {
    long total{};

    QMap<QString, QString> tags;

    void FromJson(const QJsonDocument &jsonDoc) {

        for (QJsonObject jObject = jsonDoc["tags"].toObject(); const auto &key: jObject.keys()) {
            tags[key] = jObject[key].toString();
        }
    }
};
