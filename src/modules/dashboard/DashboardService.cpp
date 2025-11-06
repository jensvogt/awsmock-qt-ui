
#include <modules/dashboard/DashboardService.h>


DashboardService::DashboardService() {
    url = QUrl(Configuration::instance().GetBaseUrl());
}

void DashboardService::GetMultiSeriesCounter(const QString &region, const QString &name, const QString &labelName
    , const QString &labelValue, const int step, const int limit, const QDateTime &start, const QDateTime &end) {

    QJsonObject jRequest;
    jRequest["region"] = region;
    jRequest["name"] = name;
    jRequest["labelName"] = labelName;
    jRequest["labelValue"] = labelValue;
    jRequest["step"] = step;
    jRequest["limit"] = limit;
    jRequest["start"] = start.toSecsSinceEpoch();
    jRequest["end"] = end.toSecsSinceEpoch();
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {{"x-awsmock-target", "monitoring"},
                       {"x-awsmock-action", "get-multi-counters"},
                       {"content-type", "application/json"}},
                      [this](const bool success, const QByteArray& response, int status, const QString& error) {
                          if (success) {

                              // The API returns an array containing one object: [{"q":"quote text", "a":"author"}]
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  JsonUtils::WriteJsonString(jsonDoc.object());
                              }

                              emit ReloadMessagesSignal();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}
