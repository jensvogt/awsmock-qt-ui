//
// Created by vogje01 on 11/11/25.
//

#ifndef AWSMOCK_QT_UI_BASE_SERVICE_H
#define AWSMOCK_QT_UI_BASE_SERVICE_H

#include <QTimer>

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

    ~BaseService() override {
    }

private slots:
    void HandleTimer() const {

        _restManager->get(Configuration::instance().GetBaseUrl(),
                          {
                              {"x-awsmock-target", "module"},
                              {"x-awsmock-action", "ping"}
                          },
                          [](const bool success, const QByteArray &response, int status, const QString &error) {
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

