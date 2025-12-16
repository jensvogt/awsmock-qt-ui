#ifndef AWSMOCK_QT_UI_LAMBDA_UPLOAD_REQUEST_H
#define AWSMOCK_QT_UI_LAMBDA_UPLOAD_REQUEST_H

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

struct LambdaUploadRequest {
    QString region;

    QString lambdaName;

    QString lambdaArn;

    QString version;

    QString lambdaCode;

    QString archive;

    [[nodiscard]] QString ToJson() const {
        QJsonObject jRequest;
        jRequest["region"] = region;
        jRequest["version"] = version;
        jRequest["archive"] = archive;
        jRequest["lambdaName"] = lambdaName;
        jRequest["lambdaArn"] = lambdaArn;
        jRequest["lambdaCode"] = lambdaCode;
        const QJsonDocument requestDoc(jRequest);
        return requestDoc.toJson();
    }
};

#endif // AWSMOCK_QT_UI_LAMBDA_UPLOAD_REQUEST_H
