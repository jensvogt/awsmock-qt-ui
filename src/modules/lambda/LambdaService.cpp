#include <modules/lambda/LambdaService.h>

LambdaService::LambdaService() {
    url = QUrl(Configuration::instance().GetValue<QString>("server.base-url", "eu-central-1"));
}

void LambdaService::ListLambdas(const QString &prefix) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jSorting;
    jSorting["sortDirection"] = -1;
    jSorting["column"] = "name";

    QJsonArray jSortingArray;
    jSortingArray.append(jSorting);

    QJsonObject jRequest;
    jRequest["prefix"] = prefix;
    jRequest["pageSize"] = -1;
    jRequest["pageIndex"] = -1;
    jRequest["sortColumns"] = jSortingArray;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
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
                                  JsonUtils::WriteJsonString(jsonDoc.object());
                                  LambdaListResponse lambdaResponse;
                                  lambdaResponse.FromJson(jsonDoc);
                                  emit ReloadLambdasSignal(lambdaResponse);
                              } else {
                                  QMessageBox::critical(nullptr, "Error", "Response is not an object!");
                              }
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                          emit EventBus::instance().TimerSignal("GetMultiSeriesCounter", timer.elapsed());
                      });
}

void LambdaService::GetLambda(const QString &lambdaArn) {
    QJsonObject jRequest;
    jRequest["functionArn"] = lambdaArn;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "get-function-counters"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  LambdaGetResponse lambdaResponse;
                                  lambdaResponse.FromJson(jsonDoc.object());
                                  emit GetLambdaDetailsSignal(lambdaResponse);
                              } else {
                                  QMessageBox::critical(nullptr, "Error", "Response is not an object!");
                              }
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

/*
void LambdaService::UploadLambda(const LambdaUploadRequest &request) {
    _restManager.post(url,
                      request.ToJson().toUtf8(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "upload-lambda"},
                          {"content-type", "lambda/json"}
                      },
                      [this](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {
                              emit LoadAllLambdas();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void LambdaService::CreateLambda(const LambdaCreateRequest &request) {
    _restManager.post(url,
                      request.ToJson().toUtf8(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "create-lambda"},
                          {"content-type", "lambda/json"}
                      },
                      [this](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  QMessageBox::information(nullptr, "Information", "Lambda uploaded!");
                                  emit LoadAllLambdas();
                              } else {
                                  QMessageBox::critical(nullptr, "Error", "Response is not an object!");
                              }
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void LambdaService::GetLambda(const QString &name) {

    QJsonObject jRequest;
    jRequest["name"] = name;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "get-lambda"},
                          {"content-type", "lambda/json"}
                      },
                      [this](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  LambdaGetResponse lambdaResponse;
                                  lambdaResponse.FromJson(jsonDoc.object());
                                  emit GetLambdaDetailsSignal(lambdaResponse);
                              } else {
                                  QMessageBox::critical(nullptr, "Error", "Response is not an object!");
                              }
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void LambdaService::UpdateLambda(const Lambda &lambda) {

    QJsonObject jRequest;
    jRequest["lambda"] = lambda.ToJsonObject();
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "update-lambda"},
                          {"content-type", "lambda/json"}
                      },
                      [this](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              emit LoadAllLambdas();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void LambdaService::EnableLambda(const QString &name) {

    QJsonObject jLambda;
    jLambda["region"] = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");
    jLambda["name"] = name;

    QJsonObject jRequest;
    jRequest["region"] = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");
    jRequest["lambda"] = jLambda;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "enable-lambda"},
                          {"content-type", "lambda/json"}
                      },
                      [this](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadAllLambdas();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void LambdaService::DisableLambda(const QString &name) {

    QJsonObject jLambda;
    jLambda["region"] = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");
    jLambda["name"] = name;

    QJsonObject jRequest;
    jRequest["region"] = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");
    jRequest["lambda"] = jLambda;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "disable-lambda"},
                          {"content-type", "lambda/json"}
                      },
                      [this](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadAllLambdas();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void LambdaService::StartLambda(const QString &name) {
    QJsonObject jLambda;
    jLambda["region"] = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");
    jLambda["name"] = name;

    QJsonObject jRequest;
    jRequest["lambda"] = jLambda;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "start-lambda"},
                          {"content-type", "lambda/json"}
                      },
                      [this](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadAllLambdas();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void LambdaService::StopLambda(const QString &name) {
    QJsonObject jLambda;
    jLambda["region"] = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");
    jLambda["name"] = name;

    QJsonObject jRequest;
    jRequest["lambda"] = jLambda;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "stop-lambda"},
                          {"content-type", "lambda/json"}
                      },
                      [this](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadAllLambdas();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void LambdaService::RestartLambda(const QString &name) {
    QJsonObject jLambda;
    jLambda["region"] = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");
    jLambda["name"] = name;

    QJsonObject jRequest;
    jRequest["lambda"] = jLambda;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "restart-lambda"},
                          {"content-type", "lambda/json"}
                      },
                      [this](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadAllLambdas();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void LambdaService::RestartAllLambdas() {
    _restManager.post(url,
                      nullptr,
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "restart-all-lambdas"},
                          {"content-type", "lambda/json"}
                      },
                      [this](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadAllLambdas();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void LambdaService::RebuildLambda(const QString &name) {
    QJsonObject jLambda;
    jLambda["region"] = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");
    jLambda["name"] = name;

    QJsonObject jRequest;
    jRequest["lambda"] = jLambda;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "rebuild-lambda"},
                          {"content-type", "lambda/json"}
                      },
                      [this](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadAllLambdas();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void LambdaService::UploadLambdaCode(const QString &lambdaName, const QString &version, const QString &lambdaCode) {

    QJsonObject jRequest;
    jRequest["version"] = lambdaName;
    jRequest["lambdaName"] = lambdaName;
    jRequest["lambdaCode"] = lambdaCode;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "upload-lambda"},
                          {"content-type", "lambda/json"}
                      },
                      [this](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadAllLambdas();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void LambdaService::ListLambdaNames() {

    QJsonObject jRequest;
    jRequest["prefix"] = "";
    jRequest["pageSize"] = -1;
    jRequest["pageIndex"] = -1;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "list-lambda-names"},
                          {"content-type", "lambda/json"}
                      },
                      [this](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isArray()) {
                                  QList<QString> lambdaList;
                                  for (const auto &name: jsonDoc.array().toVariantList()) {
                                      lambdaList.append(name.toString());
                                  }
                                  emit ListLambdaNamedSignal(lambdaList);
                              } else {
                                  QMessageBox::critical(nullptr, "Error", "Response is not an object!");
                              }
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void LambdaService::DeleteLambda(const QString &name) {
    QJsonObject jRequest;
    jRequest["name"] = name;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "delete-lambda"},
                          {"content-type", "lambda/json"}
                      },
                      [this](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadAllLambdas();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}
*/
