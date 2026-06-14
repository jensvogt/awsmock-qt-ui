#pragma once

// Qt includes
#include <QJsonArray>
#include <QJsonDocument>

struct SQSUpdateTagsRequest {

    QString queueUrl;

    QMap<QString, QString> tags;

    void FromJson(const QJsonDocument &jsonDoc) {

        for (QJsonObject jObject = jsonDoc["Tags"].toObject(); const auto &key: jObject.keys()) {
            tags[key] = jObject[key].toString();
        }
    }
};
