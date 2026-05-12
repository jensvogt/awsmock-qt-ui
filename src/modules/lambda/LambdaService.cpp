#include <modules/lambda/LambdaService.h>

void LambdaService::ListLambdas(const QString &prefix, const long pageSize, const long pageIndex, const QString &sortAttribute, const int sortDirection) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jSorting;
    jSorting["sortDirection"] = sortDirection;
    jSorting["column"] = sortAttribute;

    QJsonArray jSortingArray;
    jSortingArray.append(jSorting);

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["prefix"] = prefix;
    jRequest["pageSize"] = static_cast<qint64>(pageSize);
    jRequest["pageIndex"] = static_cast<qint64>(pageIndex);
    jRequest["sortColumns"] = jSortingArray;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "list-function-counters"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {
                              // The API returns an JSON lambda counter list
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  LambdaListResponse lambdaResponse;
                                  lambdaResponse.FromJson(jsonDoc);
                                  emit ReloadLambdasSignal(lambdaResponse);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance()
                                  .
                                  TimerSignal("ListLambdas", timer.elapsed());
                      });
}

void LambdaService::GetLambda(const QString &lambdaArn) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["functionArn"] = lambdaArn;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "get-function-counters"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  LambdaGetResponse lambdaResponse;
                                  lambdaResponse.FromJson(jsonDoc.object());
                                  emit GetLambdaDetailsSignal(lambdaResponse);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance()
                                  .
                                  TimerSignal("GetLambda", timer.elapsed());
                      });
}

void LambdaService::GetLambdaInstances(const QString &lambdaArn) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jSorting;
    jSorting["sortDirection"] = -1;
    jSorting["column"] = "messages";

    QJsonArray jSortingArray;
    jSortingArray.append(jSorting);

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["lambdaArn"] = lambdaArn;
    jRequest["prefix"] = "";
    jRequest["pageSize"] = -1;
    jRequest["pageIndex"] = -1;
    jRequest["sortColumns"] = jSortingArray;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "list-instance-counters"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  LambdaListInstancesResponse lambdaResponse;
                                  lambdaResponse.FromJson(jsonDoc);
                                  emit ListLambdaInstancesSignal(lambdaResponse);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("GetLambdaInstances", timer.elapsed());
                      });
}

void LambdaService::GetLambdaEnvironment(const QString &lambdaArn) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jSorting;
    jSorting["sortDirection"] = -1;
    jSorting["column"] = "messages";

    QJsonArray jSortingArray;
    jSortingArray.append(jSorting);

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["lambdaArn"] = lambdaArn;
    jRequest["prefix"] = "";
    jRequest["pageSize"] = -1;
    jRequest["pageIndex"] = -1;
    jRequest["sortColumns"] = jSortingArray;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "list-environment-counters"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  LambdaListEnvironmentResponse lambdaResponse;
                                  lambdaResponse.FromJson(jsonDoc);
                                  emit ListLambdaEnvironmentSignal(lambdaResponse);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("GetLambdaEnvironment", timer.elapsed());
                      });
}

void LambdaService::AddLambdaEnvironment(const QString &lambdaArn, const QString &key, const QString &value) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["FunctionArn"] = lambdaArn;
    jRequest["Key"] = key;
    jRequest["Value"] = value;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "add-function-environment"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              emit LoadLambdaEnvironment();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("AddLambdaEnvironment", timer.elapsed());
                      });
}

void LambdaService::RemoveLambdaEnvironment(const QString &lambdaArn, const QString &key) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["FunctionArn"] = lambdaArn;
    jRequest["Key"] = key;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "delete-function-environment"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadLambdaEnvironment();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("RemoveLambdaEnvironment", timer.elapsed());
                      });
}

void LambdaService::ListLambdaLogs(const QString &lambdaArn) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["lambdaArn"] = lambdaArn;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "list-lambda-result-counters"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  LambdaListResultsResponse lambdaResponse;
                                  lambdaResponse.FromJson(jsonDoc);
                                  emit ListLambdaResultsSignal(lambdaResponse);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("ListLambdaLogs", timer.elapsed());
                      });
}

void LambdaService::GetLambdaResult(const QString &oid) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["oid"] = oid;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "get-lambda-result-counter"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  LambdaGetResultsResponse lambdaResponse;
                                  lambdaResponse.FromJson(jsonDoc);
                                  emit GetLambdaResultSignal(lambdaResponse);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("GetLambdaResult", timer.elapsed());
                      });
}

void LambdaService::GetLambdaResults(const QString &oid) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["oid"] = oid;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "get-lambda-result-counter"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  LambdaListEnvironmentResponse lambdaResponse;
                                  lambdaResponse.FromJson(jsonDoc);
                                  emit ListLambdaEnvironmentSignal(lambdaResponse);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("GetLambdaResults", timer.elapsed());
                      });
}

void LambdaService::UploadLambdaCode(const LambdaUploadRequest &request) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["version"] = request.version;
    jRequest["functionArn"] = request.lambdaArn;
    jRequest["functionCode"] = request.lambdaCode;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "upload-function-code"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadLambdas();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("UploadLambdaCode", timer.elapsed());
                      });
}

void LambdaService::UpdateLambda(const QString &lambdaArn, const bool enabled) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["functionArn"] = lambdaArn;
    jRequest["enabled"] = enabled;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "update-lambda"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadLambdas();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("UpdateLambda", timer.elapsed());
                      });
}

void LambdaService::UpdateLambdaEnvironment(const QString &lambdaArn, const QString &key, const QString &value) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["FunctionArn"] = lambdaArn;
    jRequest["Key"] = key;
    jRequest["Value"] = value;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "update-function-environment"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadLambdaEnvironment();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("UpdateLambdaEnvironment", timer.elapsed());
                      });
}

void LambdaService::RebuildLambda(const QString &name, const QString &version) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["name"] = name;
    jRequest["version"] = version;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "rebuild-lambda"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadLambdas();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().DockerStatsTimerSignal("DeleteLambda", timer.elapsed());
                      });
}

void LambdaService::ListLambdaArns() {
    QElapsedTimer timer;
    timer.start();

    _restManager.post(GetBaseUrl(),
                      {},
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "list-arns"},
                          {"content-type", "application/json"}
                      },
                      [this,timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  LambdaListArnsResponse lambdaResponse;
                                  lambdaResponse.FromJson(jsonDoc);
                                  emit ListLambdaArnsSignal(lambdaResponse);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().DockerStatsTimerSignal("DeleteLambda", timer.elapsed());
                      });
}

void LambdaService::StartInstance(const QString &lambdaArn) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["functionArn"] = lambdaArn;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "start-lambda"},
                          {"content-type", "application/json"}
                      },
                      [this, timer, lambdaArn](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadLambdaDetails();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("DeleteLambdaResults", timer.elapsed());
                      });
}

void LambdaService::StopInstance(const QString &lambdaArn, const QString &instanceId) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["functionArn"] = lambdaArn;
    jRequest["instanceId"] = instanceId;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "stop-instance"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadLambdaInstances();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("StopInstance", timer.elapsed());
                      });
}

void LambdaService::DeleteLambda(const QString &name) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["FunctionName"] = name;
    jRequest["Qualifier"] = "";
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "delete-function"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadLambdaEnvironment();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance()
                                  .
                                  DockerStatsTimerSignal("DeleteLambda", timer.elapsed());
                      });
}

void LambdaService::DeleteLambdaResults(const QString &lambdaArn) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["lambdaArn"] = lambdaArn;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "delete-lambda-result-counters"},
                          {"content-type", "application/json"}
                      },
                      [this, timer, lambdaArn](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ListLambdaLogs(lambdaArn);
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance()
                                  .
                                  TimerSignal("DeleteLambdaResults", timer.elapsed());
                      });
}
