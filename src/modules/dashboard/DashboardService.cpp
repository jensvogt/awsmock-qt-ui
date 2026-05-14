#include <modules/dashboard/DashboardService.h>

DashboardService::DashboardService() {

    // Create RestManager in its own thread
    _restManager = new RestManager();
    _restManager->moveToThread(&_networkThread);
    _networkThread.start();
}

DashboardService::~DashboardService() {
    _networkThread.quit();
    _networkThread.wait();
    delete _restManager;
}


void DashboardService::GetMultiSeriesCounter(const Awsmock::Components::MonitoringConfig &config) {
    QElapsedTimer timer;
    timer.start();

    const QJsonObject jRequest{
        {"region", config.region},
        {"name", config.name},
        {"labelName", config.series},
        {"step", config.step},
        {"limit", config.limit},
        {"start", config.start.toMSecsSinceEpoch()},
        {"end", config.end.toMSecsSinceEpoch()}
    };

    const QJsonDocument requestDoc(jRequest);

    // MUST capture explicitly in C++20
    QMetaObject::invokeMethod(
        this,
        [this, config, json = requestDoc.toJson(), timer]() {
            _restManager = new RestManager();
            _restManager->post(
                GetBaseUrl(),
                json,
                {
                    {"x-awsmock-target", "monitoring"},
                    {"x-awsmock-action", "get-multi-counters"},
                    {"content-type", "application/json"}
                },
                [this, config, timer](const bool success, const QByteArray &response, const int status, const QString &) {
                    if (success) {
                        if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                            DashboardCounter counter;
                            counter.FromJson(jsonDoc.object());
                            counter.chartConfig = config;
                            emit ReloadMonitoringSignal(counter);
                        } else {
                            logWarning << "Response is not an object!";
                        }
                    }
                    emit EventBus::instance().TimerSignal("GetMultiSeriesCounter", timer.elapsed());
                }
            );
        }, Qt::QueuedConnection);
}
