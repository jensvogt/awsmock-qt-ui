//
// Created by vogje01 on 11/11/25.
//

#pragma once

#include <QTimer>

#include <utils/Configuration.h>
#include <utils/RestManager.h>

class BaseService : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     */
    BaseService() = default;

    ~BaseService() override = default;

    static QJsonObject CreateBaseRequest() {
        QJsonObject jRequest;
        jRequest["region"] = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");
        jRequest["user"] = Configuration::instance().GetValue<QString>("aws.user", "none");
        jRequest["requestId"] = QUuid::createUuid().toString(QUuid::StringFormat::WithoutBraces);
        return jRequest;
    }

    static QUrl GetBaseUrl() {
        return {Configuration::instance().GetValue<QString>("server.base-url", "eu-central-1")};
    }

    static QUrl GetBaseUrl(const QString &path) {
        return {Configuration::instance().GetValue<QString>("server.base-url", "eu-central-1") + "/" + path};
    }

    QStringList getApis() {
        return _apis;
    }

    void setApis(const QStringList &apis) {
        _apis = apis;
    }

private:
    /**
     * @brief Network access manager
     */
    RestManager *_restManager;

    /**
     * @brief Timer
     */
    QTimer *_timer;

    /**
     * @brief Api names
     */
    QStringList _apis;
};
