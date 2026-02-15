#ifndef AWSMOCK_QT_UI_COGNITO_DELETE_USERPOOL_REQUEST_H
#define AWSMOCK_QT_UI_COGNITO_DELETE_USERPOOL_REQUEST_H

// Qt includes
#include <QJsonObject>
#include <QJsonDocument>

// AwsMock includes

struct CognitoDeleteUserpoolRequest {

    QString userPoolId;

    void FromJson(const QJsonDocument &jsonDoc) {
        userPoolId = jsonDoc["UserPoolId"].toString();
    }

    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject result;
        result["UserPoolId"] = userPoolId;
        return result;
    }

    [[nodiscard]] QString ToJson() const {
        return QJsonDocument(ToJsonObject()).toJson();
    }
};

#endif // AWSMOCK_QT_UI_COGNITO_DELETE_USERPOOL_REQUEST_H
