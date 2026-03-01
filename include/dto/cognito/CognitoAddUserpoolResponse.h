#ifndef AWSMOCK_QT_UI_COGNITO_CREATE_USERPOOL_RESPONSE_H
#define AWSMOCK_QT_UI_COGNITO_CREATE_USERPOOL_RESPONSE_H

// Qt includes
#include <QJsonObject>
#include <QJsonDocument>

// AwsMock includes
#include <dto/cognito/CognitoUserpool.h>

struct CognitoAddUserpoolResponse {

    CognitoUserpool userpool;

    void FromJson(const QJsonDocument &jsonDoc) {
        userpool.FromJson(jsonDoc["UserPool"].toObject());
    }

    [[nodiscard]] QJsonObject ToJsonObject() const {
        return userpool.ToJsonObject();
    }

    [[nodiscard]] QString ToJson() const {
        return QJsonDocument(ToJsonObject()).toJson();
    }
};

#endif // AWSMOCK_QT_UI_COGNITO_CREATE_USERPOOL_RESPONSE_H
