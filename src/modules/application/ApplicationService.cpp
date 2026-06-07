#include <modules/application/ApplicationService.h>

void ApplicationService::ListApplications(const QString &prefix, const long pageSize, const long pageIndex, const QString &sortAttribute, const int sortDirection) {
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
                          {"x-awsmock-target", "application"},
                          {"x-awsmock-action", "list-applications"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {
                              // The API returns an array containing one object: [{"q":"quote text", "a":"author"}]
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  ApplicationListResponse applicationResponse;
                                  applicationResponse.FromJson(jsonDoc);
                                  emit ReloadApplicationsSignal(applicationResponse);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << "http status: " << error;
                          }
                          emit EventBus::instance().TimerSignal("ListApplications", timer.elapsed());
                      });
}

void ApplicationService::UploadApplication(const ApplicationUploadRequest &request) {
    QElapsedTimer timer;
    timer.start();

    const QByteArray body = request.ToJson().toUtf8();
    QString bodySize = QString::number(body.size());
    _restManager.post(GetBaseUrl(),
                      body,
                      {
                          {"x-awsmock-target", "application"},
                          {"x-awsmock-action", "upload-application"},
                          {"content-type", "application/json"},
                          {"content-length", bodySize}
                      },
                      [this, timer, bodySize](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              logInfo << "Application uploaded, size: " << bodySize;
                              emit LoadAllApplications();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("UploadApplication", timer.elapsed());
                      });
}

void ApplicationService::CreateApplication(const ApplicationCreateRequest &request) {
    QElapsedTimer timer;
    timer.start();

    _restManager.post(GetBaseUrl(),
                      request.ToJson().toUtf8(),
                      {
                          {"x-awsmock-target", "application"},
                          {"x-awsmock-action", "create-application"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  QMessageBox::information(nullptr, "Information", "Application uploaded!");
                                  emit LoadAllApplications();
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("CreateApplication", timer.elapsed());
                      });
}

void ApplicationService::GetApplication(const QString &name) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest;
    jRequest["name"] = name;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "application"},
                          {"x-awsmock-action", "get-application"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  ApplicationGetResponse applicationResponse;
                                  applicationResponse.FromJson(jsonDoc.object());
                                  emit GetApplicationDetailsSignal(applicationResponse);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("GetApplication", timer.elapsed());
                      });
}

void ApplicationService::UpdateApplication(const Application &application) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest;
    jRequest["application"] = application.ToJsonObject();

    _restManager.post(GetBaseUrl(),
                      QJsonDocument(jRequest).toJson(),
                      {
                          {"x-awsmock-target", "application"},
                          {"x-awsmock-action", "update-application"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadAllApplications();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("UpdateApplication", timer.elapsed());
                      });
}

void ApplicationService::EnableApplication(const QString &name) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jApplication;
    jApplication["region"] = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");
    jApplication["name"] = name;

    QJsonObject jRequest;
    jRequest["region"] = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");
    jRequest["application"] = jApplication;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "application"},
                          {"x-awsmock-action", "enable-application"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadAllApplications();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("EnableApplication", timer.elapsed());
                      });
}

void ApplicationService::DisableApplication(const QString &name) {
    QElapsedTimer timer;
    timer.start();
    QJsonObject jApplication;
    jApplication["region"] = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");
    jApplication["name"] = name;

    QJsonObject jRequest;
    jRequest["region"] = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");
    jRequest["application"] = jApplication;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "application"},
                          {"x-awsmock-action", "disable-application"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadAllApplications();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("DisableApplication", timer.elapsed());
                      });
}

void ApplicationService::StartApplication(const QString &name) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jApplication;
    jApplication["region"] = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");
    jApplication["name"] = name;

    QJsonObject jRequest;
    jRequest["application"] = jApplication;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "application"},
                          {"x-awsmock-action", "start-application"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadAllApplications();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("StartApplication", timer.elapsed());
                      });
}

void ApplicationService::StopApplication(const QString &name) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jApplication;
    jApplication["region"] = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");
    jApplication["name"] = name;

    QJsonObject jRequest;
    jRequest["application"] = jApplication;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "application"},
                          {"x-awsmock-action", "stop-application"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadAllApplications();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("StopApplication", timer.elapsed());
                      });
}

void ApplicationService::RestartApplication(const QString &name) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jApplication;
    jApplication["region"] = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");
    jApplication["name"] = name;

    QJsonObject jRequest;
    jRequest["application"] = jApplication;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "application"},
                          {"x-awsmock-action", "restart-application"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadAllApplications();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("RestartApplication", timer.elapsed());
                      });
}

void ApplicationService::RestartAllApplications() {
    QElapsedTimer timer;
    timer.start();

    _restManager.post(GetBaseUrl(),
                      nullptr,
                      {
                          {"x-awsmock-target", "application"},
                          {"x-awsmock-action", "restart-all-applications"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadAllApplications();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("RestartAllApplications", timer.elapsed());
                      });
}

void ApplicationService::RebuildApplication(const QString &name) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jApplication;
    jApplication["region"] = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");
    jApplication["name"] = name;

    QJsonObject jRequest;
    jRequest["application"] = jApplication;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "application"},
                          {"x-awsmock-action", "rebuild-application"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadAllApplications();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("RebuildApplication", timer.elapsed());
                      });
}

void ApplicationService::ListApplicationNames() {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest;
    jRequest["prefix"] = "";
    jRequest["pageSize"] = -1;
    jRequest["pageIndex"] = -1;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "application"},
                          {"x-awsmock-action", "list-application-names"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isArray()) {
                                  QList<QString> applicationList;
                                  for (const auto &name: jsonDoc.array().toVariantList()) {
                                      applicationList.append(name.toString());
                                  }
                                  emit ListApplicationNamedSignal(applicationList);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("ListApplicationNames", timer.elapsed());
                      });
}

void ApplicationService::DeleteApplication(const QString &name) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest;
    jRequest["name"] = name;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "application"},
                          {"x-awsmock-action", "delete-application"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadAllApplications();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("DeleteApplication", timer.elapsed());
                      });
}
