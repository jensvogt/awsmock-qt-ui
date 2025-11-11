#ifndef DASHBOARD_SERVICE_H
#define DASHBOARD_SERVICE_H

#include <QMessageBox>
#include <QObject>
#include <utils/Configuration.h>
#include <utils/RestManager.h>
#include <utils/JsonUtils.h>

#include <utils/BaseService.h>
#include <dto/dashboard/DashboardCounterResult.h>
#include <modules/dashboard/ChartConfig.h>

class DashboardService final : public BaseService {
    Q_OBJECT

public:
    /**
     * @brief SNSService
     */
    DashboardService();

    /**
     * @brief Get a multi series counters
     *
     * @param config chart configuration
     */
    void GetMultiSeriesCounter(const ChartConfig &config);

signals:
    void ReloadMonitoringSignal(const DashboardCounter &result);

private:
    /**
     * @brief HTTP REST manager
     */
    RestManager _restManager;

    /**
     * @brief Base URL
     */
    QUrl url;
};


#endif // DASHBOARD_SERVICE_H
