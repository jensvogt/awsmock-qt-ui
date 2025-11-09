#ifndef AWSMOCK_QT_UI_APPLICATION_UPLOAD_REQUEST_H
#define AWSMOCK_QT_UI_APPLICATION_UPLOAD_REQUEST_H

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

struct ApplicationUploadRequest {
    QString region;

    QString applicationName;

    QString version;

    QString applicationCode;

    QString archive;

    QString ToJson() const {

        QJsonObject jRequest;
        jRequest["region"] = region;
        jRequest["version"] = version;
        jRequest["archive"] = archive;
        jRequest["applicationName"] = applicationName;
        jRequest["applicationCode"] = applicationCode;
        const QJsonDocument requestDoc(jRequest);
        return requestDoc.toJson();
    }
};

#endif // AWSMOCK_QT_UI_APPLICATION_UPLOAD_REQUEST_H
