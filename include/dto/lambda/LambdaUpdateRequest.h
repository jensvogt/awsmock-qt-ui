#ifndef AWSMOCK_QT_UI_LAMBDA_UPDATE_REQUEST_H
#define AWSMOCK_QT_UI_LAMBDA_UPDATE_REQUEST_H

#include <QJsonObject>
#include <QJsonDocument>

struct LambdaUpdateRequest {
    QString lambdaArn;

    bool enabled;

    [[nodiscard]] QString ToJson() const {
        QJsonObject jRequest;
        jRequest["lambdaArn"] = lambdaArn;
        jRequest["enabled"] = enabled;
        const QJsonDocument requestDoc(jRequest);
        return requestDoc.toJson();
    }
};

#endif // AWSMOCK_QT_UI_LAMBDA_UPDATE_REQUEST_H
