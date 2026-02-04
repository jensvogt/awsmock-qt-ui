
#include <modules/module/ModuleService.h>

ModuleService::ModuleService() {
    url = QUrl(Configuration::instance().GetValue<QString>("server.base-url", "http://localhost:4566"));
}

void ModuleService::ExportInfrastructure(const QString &exportFilename) {
    QElapsedTimer timer;
    timer.start();

    QJsonArray array;
    array.append({"sqs"});
    array.append({"sns"});
    array.append({"s3"});
    array.append({"application"});
    array.append({"lambda"});
    array.append({"cognito"});
    array.append({"kms"});
    array.append({"dynamodb"});
    array.append({"secretsmanager"});
    array.append({"transfer"});
    array.append({"ssm"});
    array.append({"apigateway"});

    QJsonObject jRequest;
    jRequest["includeObjects"] = false;
    jRequest["prettyPrint"] = true;
    jRequest["cleanFirst"] = false;
    jRequest["modules"] = array;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "module"},
                          {"x-awsmock-action", "export"},
                          {"content-type", "application/json"}
                      },
                      [this,exportFilename, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              // The API returns an infrastructure object
                              emit ExportResponseSignal(exportFilename, response.toStdString().data());
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                          emit EventBus::instance().TimerSignal("ExportInfrastructure", timer.elapsed());
                      });
}

void ModuleService::ImportInfrastructure(const QString &content) {
    QElapsedTimer timer;
    timer.start();

    _restManager.post(url,
                      content.toUtf8(),
                      {
                          {"x-awsmock-target", "module"},
                          {"x-awsmock-action", "import"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              // The API returns an infrastructure object
                              emit ImportResponseSignal();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                          emit EventBus::instance().TimerSignal("ImportInfrastructure", timer.elapsed());
                      });
}

void ModuleService::CleanInfrastructure() {
    QElapsedTimer timer;
    timer.start();

    QJsonArray array;
    array.append({"sqs"});
    array.append({"sns"});
    array.append({"s3"});

    QJsonObject jRequest;
    jRequest["onlyObjects"] = true;
    jRequest["prettyPrint"] = false;
    jRequest["modules"] = array;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "module"},
                          {"x-awsmock-action", "clean-objects"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              // The API returns an infrastructure object
                              emit CleanResponseSignal();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                          emit EventBus::instance().TimerSignal("CleanInfrastructure", timer.elapsed());
                      });
}

void ModuleService::GetServerConfig() {
    QElapsedTimer timer;
    timer.start();

    _restManager.get(url,
                     {
                         {"x-awsmock-target", "module"},
                         {"x-awsmock-action", "get-config"},
                         {"content-type", "application/json"}
                     },
                     [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                         if (success) {
                             const QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                             GatewayConfig serverConfig;
                             serverConfig.FromJson(jsonDoc.object());
                             emit GetServerConfigSignal(serverConfig);
                         } else {
                             QMessageBox::critical(nullptr, "Error", error);
                         }
                         emit EventBus::instance().TimerSignal("GetServerConfig", timer.elapsed());
                     });
}

void ModuleService::GetInfrastructure() {
    QElapsedTimer timer;
    timer.start();

    _restManager.get(url,
                     {
                         {"x-awsmock-target", "module"},
                         {"x-awsmock-action", "get-infrastructure"},
                         {"content-type", "application/json"}
                     },
                     [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                         if (success) {
                             // The API returns an infrastructure object as string
                             emit GetInfrastructureSignal(QString(response));
                         } else {
                             QMessageBox::critical(nullptr, "Error", error);
                         }
                         emit EventBus::instance().TimerSignal("GetInfrastructure", timer.elapsed());
                     });
}
