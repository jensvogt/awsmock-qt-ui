#include <modules/apigateway/ApiGatewayService.h>
#include <dto/apigateway/UsagePlanListResponse.h>

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

void ApiGatewayService::GetApiKey(const QString &keyId) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest;
    jRequest["keyId"] = keyId;

    _restManager.post(GetBaseUrl(),
                      QJsonDocument(jRequest).toJson(),
                      {
                          {"x-awsmock-target", "apigateway"},
                          {"x-awsmock-action", "get-api-key-counter"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  RestApiKey apiKey;
                                  apiKey.fromJson(jsonDoc.object()["apiKey"].toObject());
                                  emit GetApiKeySignal(apiKey);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << "http status: " << error;
                          }
                          emit EventBus::instance().TimerSignal("GetApiKey", timer.elapsed());
                      });
}

void ApiGatewayService::CreateApiKey(const RestApiKeyCreateRequest &request) {
    QElapsedTimer timer;
    timer.start();

    _restManager.post(GetBaseUrl(),
                      request.ToJson().toUtf8(),
                      {
                          {"x-awsmock-target", "apigateway"},
                          {"x-awsmock-action", "create-api-key"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadApiKeysSignal();
                          } else {
                              logError << "http status: " << error;
                          }
                          emit EventBus::instance().TimerSignal("CreateApiKey", timer.elapsed());
                      });
}

void ApiGatewayService::UpdateApiKey(const RestApiKeyUpdateRequest &request) {
    QElapsedTimer timer;
    timer.start();

    _restManager.post(GetBaseUrl(),
                      request.ToJson().toUtf8(),
                      {
                          {"x-awsmock-target", "apigateway"},
                          {"x-awsmock-action", "update-api-key"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadApiKeysSignal();
                          } else {
                              logError << "http status: " << error;
                          }
                          emit EventBus::instance().TimerSignal("UpdateApiKey", timer.elapsed());
                      });
}

void ApiGatewayService::ListApiKeys(const QString &prefix, const long pageSize, const long pageIndex, const QString &sortAttribute, const int sortDirection) {
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
                          {"x-awsmock-action", "list-api-key-counters"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  RestApiKeyListResponse listResponse;
                                  listResponse.FromJson(jsonDoc);
                                  emit ListApiKeysSignal(listResponse);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << "http status: " << error;
                          }
                          emit EventBus::instance().TimerSignal("ListApiKeys", timer.elapsed());
                      });
}

void ApiGatewayService::DeleteApiKey(const QString &keyId) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest;
    jRequest["apiKey"] = keyId;

    _restManager.post(GetBaseUrl(),
                      QJsonDocument(jRequest).toJson(),
                      {
                          {"x-awsmock-target", "apigateway"},
                          {"x-awsmock-action", "delete-api-key"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadApiKeysSignal();
                          } else {
                              logError << "http status: " << error;
                          }
                          emit EventBus::instance().TimerSignal("DeleteApiKey", timer.elapsed());
                      });
}

void ApiGatewayService::EnableApiKey(const QString &keyId) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest;
    jRequest["keyId"] = keyId;

    _restManager.post(GetBaseUrl(),
                      QJsonDocument(jRequest).toJson(),
                      {
                          {"x-awsmock-target", "apigateway"},
                          {"x-awsmock-action", "enable-api-key"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadApiKeysSignal();
                          } else {
                              logError << "http status: " << error;
                          }
                          emit EventBus::instance().TimerSignal("EnableApiKey", timer.elapsed());
                      });
}

void ApiGatewayService::DisableApiKey(const QString &keyId) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest;
    jRequest["keyId"] = keyId;

    _restManager.post(GetBaseUrl(),
                      QJsonDocument(jRequest).toJson(),
                      {
                          {"x-awsmock-target", "apigateway"},
                          {"x-awsmock-action", "disable-api-key"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadApiKeysSignal();
                          } else {
                              logError << "http status: " << error;
                          }
                          emit EventBus::instance().TimerSignal("DisableApiKey", timer.elapsed());
                      });
}

void ApiGatewayService::ListUsagePlans(const QString &prefix, const long pageSize, const long pageIndex, const QString &sortAttribute, const int sortDirection) {
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
                          {"x-awsmock-action", "list-usage-plan-counters"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  UsagePlanListResponse listResponse;
                                  listResponse.FromJson(jsonDoc);
                                  emit ListUsagePlansSignal(listResponse);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << "http status: " << error;
                          }
                          emit EventBus::instance().TimerSignal("ListUsagePlans", timer.elapsed());
                      });
}

void ApiGatewayService::GetUsagePlan(const QString &planId) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest;
    jRequest["usagePlanId"] = planId;

    _restManager.post(GetBaseUrl(),
                      QJsonDocument(jRequest).toJson(),
                      {
                          {"x-awsmock-target", "apigateway"},
                          {"x-awsmock-action", "get-usage-plan-counter"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  UsagePlan usagePlan;
                                  usagePlan.fromJson(jsonDoc.object()["usagePlan"].toObject());
                                  emit GetUsagePlanSignal(usagePlan);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << "http status: " << error;
                          }
                          emit EventBus::instance().TimerSignal("GetUsagePlan", timer.elapsed());
                      });
}

void ApiGatewayService::CreateUsagePlan(const UsagePlanCreateRequest &request) {
    QElapsedTimer timer;
    timer.start();

    _restManager.post(GetBaseUrl(),
                      request.ToJson().toUtf8(),
                      {
                          {"x-awsmock-target", "apigateway"},
                          {"x-awsmock-action", "create-usage-plan-counter"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadUsagePlansSignal();
                          } else {
                              logError << "http status: " << error;
                          }
                          emit EventBus::instance().TimerSignal("CreateUsagePlan", timer.elapsed());
                      });
}

void ApiGatewayService::UpdateUsagePlan(const UsagePlanUpdateRequest &request) {
    QElapsedTimer timer;
    timer.start();

    _restManager.post(GetBaseUrl(),
                      request.ToJson().toUtf8(),
                      {
                          {"x-awsmock-target", "apigateway"},
                          {"x-awsmock-action", "update-usage-plan-counter"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadUsagePlansSignal();
                          } else {
                              logError << "http status: " << error;
                          }
                          emit EventBus::instance().TimerSignal("UpdateUsagePlan", timer.elapsed());
                      });
}

void ApiGatewayService::DeleteUsagePlan(const QString &planId) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest;
    jRequest["usagePlanId"] = planId;

    _restManager.post(GetBaseUrl(),
                      QJsonDocument(jRequest).toJson(),
                      {
                          {"x-awsmock-target", "apigateway"},
                          {"x-awsmock-action", "delete-usage-plan-counter"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadUsagePlansSignal();
                          } else {
                              logError << "http status: " << error;
                          }
                          emit EventBus::instance().TimerSignal("DeleteUsagePlan", timer.elapsed());
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
