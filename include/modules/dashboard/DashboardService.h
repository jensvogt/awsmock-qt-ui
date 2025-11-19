#ifndef DASHBOARD_SERVICE_H
#define DASHBOARD_SERVICE_H

#include <QObject>
#include <QThread>

#include <utils/RestManager.h>
#include <modules/dashboard/ChartConfig.h>
#include <dto/dashboard/DashboardCounterResult.h>

class DashboardService : public QObject {
    Q_OBJECT

public:
    DashboardService();

    ~DashboardService() override;

    void GetMultiSeriesCounter(const ChartConfig &config);

signals:
    void ReloadMonitoringSignal(const DashboardCounter &counter);

private:
    QUrl _url;
    RestManager *_restManager = nullptr;
    QThread _networkThread;
};

#endif // DASHBOARD_SERVICE_H
