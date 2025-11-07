#ifndef AWSMOCK_QT_UI_APPLICATION_H
#define AWSMOCK_QT_UI_APPLICATION_H

#include <QDateTime>
#include <QJsonObject>

#include "utils/JsonUtils.h"

struct Application {

    QString name;

    QString runtime;

    QString runType;

    QString version;

    QString archive;

    long privatePort = -1;

    long publicPort = -1;

    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject jsonObject;
        jsonObject["name"] = name;
        jsonObject["runtime"] = runtime;
        jsonObject["runType"] = runType;
        jsonObject["version"] = version;
        jsonObject["archive"] = archive;
        jsonObject["privatePort"] = static_cast<qint64>(privatePort);
        jsonObject["publicPort"] = static_cast<qint64>(publicPort);
        JsonUtils::WriteJsonString(jsonObject);
        return jsonObject;
    }
};

#endif // AWSMOCK_QT_UI_APPLICATION_H
