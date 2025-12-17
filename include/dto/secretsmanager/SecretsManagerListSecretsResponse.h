#ifndef AWSMOCK_QT_UI_SECRETSMANAGER_LIST_SECRETS_RESPONSE_H
#define AWSMOCK_QT_UI_SECRETSMANAGER_LIST_SECRETS_RESPONSE_H

// Qt includes
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

// AwsMock includes
#include <dto/secretsmanager/SecretCounter.h>

struct SecretsListResponse {
    long total{};

    QList<SecretCounter> secretCounters;

    void FromJson(const QJsonDocument &jsonDoc) {
        for (QJsonArray jArray = jsonDoc["secretCounters"].toArray(); const auto &element: jArray) {
            SecretCounter secretCounter;
            secretCounter.FromJson(element.toObject());
            secretCounters.append(secretCounter);
        }
    }
};

#endif // AWSMOCK_QT_UI_SECRETSMANAGER_LIST_SECRETS_RESPONSE_H
