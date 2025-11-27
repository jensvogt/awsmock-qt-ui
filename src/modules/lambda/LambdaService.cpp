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

void LambdaService::GetLambdaInstances(const QString &lambdaArn) {
    QJsonObject jSorting;
    jSorting["sortDirection"] = -1;
    jSorting["column"] = "messages";

    QJsonArray jSortingArray;
    jSortingArray.append(jSorting);

    QJsonObject jRequest;
    jRequest["lambdaArn"] = lambdaArn;
    jRequest["prefix"] = "";
    jRequest["pageSize"] = -1;
    jRequest["pageIndex"] = -1;
    jRequest["sortColumns"] = jSortingArray;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "list-instance-counters"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  LambdaListInstancesResponse lambdaResponse;
                                  lambdaResponse.FromJson(jsonDoc);
                                  emit ListLambdaInstancesSignal(lambdaResponse);
                              } else {
                                  QMessageBox::critical(nullptr, "Error", "Response is not an object!");
                              }
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void LambdaService::GetLambdaEnvironment(const QString &lambdaArn) {
    QJsonObject jSorting;
    jSorting["sortDirection"] = -1;
    jSorting["column"] = "messages";

    QJsonArray jSortingArray;
    jSortingArray.append(jSorting);

    QJsonObject jRequest;
    jRequest["lambdaArn"] = lambdaArn;
    jRequest["prefix"] = "";
    jRequest["pageSize"] = -1;
    jRequest["pageIndex"] = -1;
    jRequest["sortColumns"] = jSortingArray;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "list-environment-counters"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  LambdaListEnvironmentResponse lambdaResponse;
                                  lambdaResponse.FromJson(jsonDoc);
                                  emit ListLambdaEnvironmentSignal(lambdaResponse);
                              } else {
                                  QMessageBox::critical(nullptr, "Error", "Response is not an object!");
                              }
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void LambdaService::AddLambdaEnvironment(const QString &lambdaArn, const QString &key, const QString &value) {

    QJsonObject jRequest;
    jRequest["FunctionArn"] = lambdaArn;
    jRequest["Key"] = key;
    jRequest["Value"] = value;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "add-function-environment"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              emit LoadLambdaEnvironment();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void LambdaService::RemoveLambdaEnvironment(const QString &lambdaArn, const QString &key) {

    QJsonObject jRequest;
    jRequest["FunctionArn"] = lambdaArn;
    jRequest["Key"] = key;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "lambda"},
                          {"x-awsmock-action", "delete-function-environment"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadLambdaEnvironment();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}
