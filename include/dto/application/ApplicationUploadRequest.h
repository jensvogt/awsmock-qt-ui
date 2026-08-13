#pragma once

// Qt includes
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

struct ApplicationUploadRequest {
    QString region;

    QString applicationName;

    QString version;

    QString applicationCode;

    QString archive;

    long contentLength;

    [[nodiscard]]
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
