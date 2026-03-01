#ifndef DASHBOARD_SERVICE_H
#define DASHBOARD_SERVICE_H

#include <QObject>
#include <QThread>
#include <QJsonObject>
#include <QJsonDocument>
#include <QElapsedTimer>

#include <utils/Configuration.h>
#include <utils/ScopedTimer.h>
#include <utils/RestManager.h>
#include <utils/BaseService.h>
#include <utils/Logging.h>
#include <modules/dashboard/ChartConfig.h>
#include <dto/dashboard/DashboardCounterResult.h>

class DashboardService : public BaseService {
    Q_OBJECT

public:
    DashboardService();

    ~DashboardService() override;

    void GetMultiSeriesCounter(const ChartConfig &config);

signals:
    void ReloadMonitoringSignal(const DashboardCounter &counter);

private:
    RestManager *_restManager = nullptr;
    QThread _networkThread;
};

#endif // DASHBOARD_SERVICE_H
