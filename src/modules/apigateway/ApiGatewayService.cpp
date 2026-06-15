#include <modules/apigateway/ApiGatewayService.h>

void ApiGatewayService::CreateRestApi(const RestApiCreateRequest &request) {
    QElapsedTimer timer;
    timer.start();

    _restManager.post(GetBaseUrl(),
                      request.ToJson().toUtf8(),
                      {
                          {"x-awsmock-target", "apigateway"},
                          {"x-awsmock-action", "create-rest-api"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadRestApisSignal();
                          } else {
                              logError << "http status: " << error;
                          }
                          emit EventBus::instance().TimerSignal("CreateRestApi", timer.elapsed());
                      });
}

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
                                  //std::cerr << JsonUtils::WriteJsonToString(jsonDoc.object()).toStdString() << std::endl;
                                  RestApiListResponse restApiListResponse;
                                  restApiListResponse.FromJson(jsonDoc);
                                  emit ListRestApisSignal(restApiListResponse);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << "http status: " << error;
                          }
                          emit EventBus::instance().TimerSignal("ListRestApis", timer.elapsed());
                      });
}

void ApiGatewayService::GetRestApi(const QString &name) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest;
    jRequest["name"] = name;

    _restManager.post(GetBaseUrl(),
                      QJsonDocument(jRequest).toJson(),
                      {
                          {"x-awsmock-target", "apigateway"},
                          {"x-awsmock-action", "get-rest-api-counter"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  std::cerr << JsonUtils::WriteJsonToString(jsonDoc.object()).toStdString() << std::endl;
                                  RestApiGetResponse restApiGetResponse;
                                  restApiGetResponse.FromJson(jsonDoc);
                                  emit GetRestApiSignal(restApiGetResponse);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << "http status: " << error;
                          }
                          emit EventBus::instance().TimerSignal("GetRestApi", timer.elapsed());
                      });
}

void ApiGatewayService::UpdateRestApi(const RestApiUpdateRequest &request) {
    QElapsedTimer timer;
    timer.start();

    _restManager.post(GetBaseUrl(),
                      request.ToJson().toUtf8(),
                      {
                          {"x-awsmock-target", "apigateway"},
                          {"x-awsmock-action", "update-rest-api-counter"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadRestApisSignal();
                          } else {
                              logError << "http status: " << error;
                          }
                          emit EventBus::instance().TimerSignal("UpdateRestApi", timer.elapsed());
                      });
}

void ApiGatewayService::DeleteResource(const QString &restApiId, const QString &resourceId) {
    QElapsedTimer timer;
    timer.start();

    _restManager.del(GetBaseUrl("restapis/" + restApiId + "/resources/" + resourceId),
                     {
                         {"x-awsmock-target", "apigateway"},
                         {"x-awsmock-action", "delete-resource"},
                         {"content-type", "application/json"}
                     },
                     [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                         if (success) {
                             emit ReloadRestApisSignal();
                         } else {
                             logError << "http status: " << error;
                         }
                         emit EventBus::instance().TimerSignal("DeleteResource", timer.elapsed());
                     });
}

void ApiGatewayService::DeleteRestApi(const QString &name) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest;
    jRequest["name"] = name;

    _restManager.post(GetBaseUrl(),
                      QJsonDocument(jRequest).toJson(),
                      {
                          {"x-awsmock-target", "apigateway"},
                          {"x-awsmock-action", "delete-rest-api-counter"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadRestApisSignal();
                          } else {
                              logError << "http status: " << error;
                          }
                          emit EventBus::instance().TimerSignal("UpdateRestApi", timer.elapsed());
                      });
}
