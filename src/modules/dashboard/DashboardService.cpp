
#include <modules/dashboard/DashboardService.h>

DashboardService::DashboardService() {
    url = QUrl(Configuration::instance().GetBaseUrl());
}

void DashboardService::GetMultiSeriesCounter(const ChartConfig &config) {
    QJsonObject jRequest;
    jRequest["region"] = config.region;
    jRequest["name"] = config.name;
    jRequest["labelName"] = config.series;
    jRequest["labelValue"] = "";
    jRequest["step"] = config.step;
    jRequest["limit"] = config.limit;
    jRequest["start"] = config.start.toMSecsSinceEpoch();
    jRequest["end"] = config.end.toMSecsSinceEpoch();
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "monitoring"},
                          {"x-awsmock-action", "get-multi-counters"},
                          {"content-type", "application/json"}
                      },
                      [this, config](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {

                              // The API returns an array of values
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  DashboardCounter counter;
                                  counter.FromJson(jsonDoc.object());
                                  counter.chartConfig = config;
                                  emit ReloadMonitoringSignal(counter);
                              }
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}
