#ifndef AWSMOCK_QT_UI_DASHBOARD_SERVICE_H
#define AWSMOCK_QT_UI_DASHBOARD_SERVICE_H

// Qt includes
#include <QElapsedTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QThread>

// Awsmock includes
#include <components/MonitoringConfig.h>
#include <dto/dashboard/DashboardCounterResult.h>
#include <modules/dashboard/ChartConfig.h>
#include <utils/BaseService.h>
#include <utils/Configuration.h>
#include <utils/Logging.h>
#include <utils/RestManager.h>
#include <utils/ScopedTimer.h>

class DashboardService : public BaseService {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     */
    DashboardService();

    /**
     * @brief Destructor
     */
    ~DashboardService() override;

    /**
     * @brief Lists the monitoring counter for the given config
     *
     * @param config monitoring config
     */
    void GetMultiSeriesCounter(const Awsmock::Components::MonitoringConfig &config);

signals:
    /**
     * @brief Signals a monitoring list arrived
     *
     * @param counter counter list
     */
    void ReloadMonitoringSignal(const DashboardCounter &counter);

private:
    /**
     * @brief REST manager
     */
    RestManager *_restManager = nullptr;

    /**
     * @brief Network thread
     */
    QThread _networkThread;
};

#endif // AWSMOCK_QT_UI_DASHBOARD_SERVICE_H
