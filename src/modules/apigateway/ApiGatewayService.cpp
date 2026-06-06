#include <modules/apigateway/ApiGatewayService.h>

void ApiGatewayService::ListRestApis(const QString &prefix, const long pageSize, const long pageIndex, const QString &sortAttribute, const int sortDirection) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jSorting;
    jSorting["sortDirection"] = sortDirection;
    jSorting["column"] = sortAttribute;

    QJsonArray jSortingArray;
    jSortingArray.append(jSorting);

    QJsonObject jRequest;
    jRequest["prefix"] = prefix;
    jRequest["pageSize"] = static_cast<qint64>(pageSize);
    jRequest["pageIndex"] = static_cast<qint64>(pageIndex);
    jRequest["sortColumns"] = jSortingArray;

    _restManager.post(GetBaseUrl(),
                      QJsonDocument(jRequest).toJson(),
                      {
                          {"x-awsmock-target", "apigateway"},
                          {"x-awsmock-action", "list-rest-api-counters"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  std::cerr << JsonUtils::WriteJsonToString(jsonDoc.object()).toStdString() << std::endl;
                                  RestApiListResponse restApiListResponse;
                                  restApiListResponse.FromJson(jsonDoc);
                                  emit ListRestApisSignal(restApiListResponse);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << "http status: " << error;
                          }
                          emit EventBus::instance().TimerSignal("GetMultiSeriesCounter", timer.elapsed());
                      });
}
