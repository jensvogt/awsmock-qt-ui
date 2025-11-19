#include <modules/dashboard/DashboardService.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <utils/Configuration.h>

DashboardService::DashboardService() {
    _url = QUrl(Configuration::instance().GetBaseUrl());

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


void DashboardService::GetMultiSeriesCounter(const ChartConfig &config) {
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
        [this, config, json = requestDoc.toJson()]() {
            _restManager = new RestManager();
            _restManager->post(
                _url,
                json,
                {
                    {"x-awsmock-target", "monitoring"},
                    {"x-awsmock-action", "get-multi-counters"},
                    {"content-type", "application/json"}
                },
                [this, config](const bool success, const QByteArray &response, const int status, const QString &error) {
                    if (!success) {
                        qDebug() << "Status:" << status
                                << "Config:" << config.name << "/" << config.series
                                << "Error:" << error;
                        return;
                    }

                    Configuration::instance().SetConnectionState(true);

                    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                    if (!jsonDoc.isObject())
                        return;

                    DashboardCounter counter;
                    counter.FromJson(jsonDoc.object());
                    counter.chartConfig = config;

                    emit ReloadMonitoringSignal(counter);
                }
            );
        },
        Qt::QueuedConnection
    );
}
