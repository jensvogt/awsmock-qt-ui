#ifndef AWSMOCK_QT_UI_SECRETSMANAGER_GET_SECRET_VERSION_RESPONSE_H
#define AWSMOCK_QT_UI_SECRETSMANAGER_GET_SECRET_VERSION_RESPONSE_H

#include <QJsonObject>
#include <QJsonDocument>

#include <dto/secretsmanager/SecretVersionCounter.h>

struct SecretGetVersionResponse {

    QVector<SecretVersionCounter> secretVersionCounters;

    void FromJson(const QJsonObject &jsonObject) {
        for (int i = 0; i < jsonObject.value("secretVersionCounters").toArray().count(); ++i) {
            SecretVersionCounter secretVersionCounter;
            secretVersionCounter.FromJson(jsonObject["secretVersionCounters"].toArray()[i].toObject());
            secretVersionCounters.append(secretVersionCounter);
        }
    }
};

#endif // AWSMOCK_QT_UI_SECRETSMANAGER_GET_SECRET_VERSION_RESPONSE_H
