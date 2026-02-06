//
// Created by vogje01 on 11/11/25.
//

#ifndef AWSMOCK_QT_UI_BASE_SERVICE_H
#define AWSMOCK_QT_UI_BASE_SERVICE_H

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
        jRequest["requestId"] = QUuid::createUuid().toString();
        return jRequest;
    }

    static QUrl GetBaseUrl() {
        return {Configuration::instance().GetValue<QString>("server.base-url", "eu-central-1")};
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
};

#endif //AWSMOCK_QT_UI_BASE_SERVICE_H

