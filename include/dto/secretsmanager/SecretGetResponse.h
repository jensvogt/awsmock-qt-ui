#ifndef AWSMOCK_QT_UI_SECRETSMANAGER_GET_SECRET_RESPONSE_H
#define AWSMOCK_QT_UI_SECRETSMANAGER_GET_SECRET_RESPONSE_H

#include <QJsonObject>
#include <QJsonDocument>

#include <dto/secretsmanager/SecretCounter.h>

struct SecretGetResponse {

    SecretCounter secretCounter;

    void FromJson(const QJsonObject &jsonObject) {
        secretCounter.FromJson(jsonObject["secretCounter"].toObject());
    }
};

#endif // AWSMOCK_QT_UI_SECRETSMANAGER_GET_SECRET_RESPONSE_H
