#include <modules/module/ModuleService.h>

#include "modules/module/ModuleExportDialog.h"

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

    _restManager.post(GetBaseUrl(),
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
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("ExportInfrastructure", timer.elapsed());
                      });
}

void ModuleService::ExportInfrastructure(const QString &exportFilename, const QStringList &modules) {
    QElapsedTimer timer;
    timer.start();

    QJsonArray array;
    for (const auto &module: modules) {
        array.append(module);
    }

    QJsonObject jRequest;
    jRequest["includeObjects"] = false;
    jRequest["prettyPrint"] = true;
    jRequest["modules"] = array;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
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
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("ExportInfrastructure", timer.elapsed());
                      });
}

void ModuleService::ImportInfrastructure(const QString &content) {
    QElapsedTimer timer;
    timer.start();

    _restManager.post(GetBaseUrl(),
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
                              logError << error;
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

    _restManager.post(GetBaseUrl(),
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
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("CleanInfrastructure", timer.elapsed());
                      });
}

void ModuleService::GetServerConfig() {
    QElapsedTimer timer;
    timer.start();

    _restManager.get(GetBaseUrl(),
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
                             logError << error;
                         }
                         emit EventBus::instance().TimerSignal("GetServerConfig", timer.elapsed());
                     });
}

void ModuleService::GetInfrastructure(const QStringList &modules, const ExportType &exportType) {
    QElapsedTimer timer;
    timer.start();

    QJsonArray array;
    for (const auto &module: modules) {
        array.append(module);
    }

    QJsonObject jRequest;
    jRequest["includeObjects"] = false;
    jRequest["prettyPrint"] = true;
    jRequest["exportType"] = ExportTypeToString(exportType);
    jRequest["modules"] = array;

    _restManager.get(GetBaseUrl(),
                     QJsonDocument(jRequest).toJson(),
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
                             logError << error;
                         }
                         emit EventBus::instance().TimerSignal("GetInfrastructure", timer.elapsed());
                     });
}

void ModuleService::PingServer() {
    _restManager.get(GetBaseUrl(),
                     {
                         {"x-awsmock-target", "module"},
                         {"x-awsmock-action", "ping"},
                         {"content-type", "application/json"}
                     },
                     [](const bool success, const QByteArray &, int, const QString &) {
                         emit EventBus::instance().PingSignal(success);
                     });
}

void ModuleService::SetLogLevel(const QString &logLevel) {
    QElapsedTimer timer;
    timer.start();

    _restManager.get(GetBaseUrl(),
                     {
                         {"x-awsmock-target", "module"},
                         {"x-awsmock-action", "set-loglevel"},
                         {"content-type", "application/json"}
                     },
                     [this,timer](const bool success, const QByteArray &, int, const QString &error) {
                         if (!success) {
                             logError << error;
                         }
                         emit EventBus::instance().TimerSignal("SetLogLevel", timer.elapsed());
                     });
}

void ModuleService::GetLogLevel() {
    QElapsedTimer timer;
    timer.start();

    _restManager.get(GetBaseUrl(),
                     {
                         {"x-awsmock-target", "module"},
                         {"x-awsmock-action", "get-loglevel"},
                         {"content-type", "application/json"}
                     },
                     [this,timer](const bool success, const QByteArray &response, int, const QString &error) {
                         if (success) {
                             // The API returns the loglevel as simple plain string
                             emit GetLoglevelSignal(QString(response));
                         } else {
                             logError << error;
                         }
                         emit EventBus::instance().TimerSignal("GetLogLevel", timer.elapsed());
                     });
}

void ModuleService::ListModuleNames() {
    QElapsedTimer timer;
    timer.start();

    _restManager.get(GetBaseUrl(),
                     {
                         {"x-awsmock-target", "module"},
                         {"x-awsmock-action", "list-module-names"},
                         {"content-type", "application/json"}
                     },
                     [this,timer](const bool success, const QByteArray &response, int, const QString &error) {
                         if (success) {
                             const QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                             ListModuleNamesResponse modulesNames;
                             modulesNames.FromJson(jsonDoc.object());
                             emit ListModuleNamesSignal(modulesNames);
                         } else {
                             logError << error;
                         }
                         emit EventBus::instance().TimerSignal("ListModuleNames", timer.elapsed());
                     });
}
