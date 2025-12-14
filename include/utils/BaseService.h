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
    BaseService() {
        // Create REST manager
        _restManager = new RestManager();

        // Create a new timer
        _timer = new QTimer();

        // Connect its timeout signal to a slot/lambda
        connect(_timer, &QTimer::timeout, this, &BaseService::HandleTimer);

        _timer->start(5000);
    }

    ~BaseService() override = default;

    static QJsonObject CreateBaseRequest() {
        QJsonObject jRequest;
        jRequest["region"] = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");
        jRequest["user"] = Configuration::instance().GetValue<QString>("aws.user", "none");
        return jRequest;
    }

private slots:
    void HandleTimer() const {
        _restManager->get(Configuration::instance().GetValue<QString>("server.base-url", "eu-central-1"),
                          {
                              {"x-awsmock-target", "module"},
                              {"x-awsmock-action", "ping"}
                          },
                          [](const bool success, const QByteArray &, int status, const QString &error) {
                              if (success) {
                                  Configuration::instance().SetConnectionState(true);
                              } else {
                                  Configuration::instance().SetConnectionState(false);
                              }
                          });
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

