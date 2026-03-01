#ifndef AWSMOCK_QT_UI_COGNITO_CREATE_USERPOOL_REQUEST_H
#define AWSMOCK_QT_UI_COGNITO_CREATE_USERPOOL_REQUEST_H

// Qt includes
#include <QJsonObject>
#include <QJsonDocument>

// AwsMock includes

struct CognitoAddUserpoolRequest {

    QString region;
    QString userpool;
    QString domain;

    void FromJson(const QJsonDocument &jsonDoc) {
        region = jsonDoc["Region"].toString();
        userpool = jsonDoc["PoolName"].toString();
        domain = jsonDoc["Domain"].toString();
    }

    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject result;
        result["Region"] = region;
        result["PoolName"] = userpool;
        result["Domain"] = userpool;
        return result;
    }

    [[nodiscard]] QString ToJson() const {
        return QJsonDocument(ToJsonObject()).toJson();
    }
};

#endif // AWSMOCK_QT_UI_COGNITO_CREATE_USERPOOL_REQUEST_H
