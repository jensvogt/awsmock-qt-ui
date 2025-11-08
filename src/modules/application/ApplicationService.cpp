#include <modules/application/ApplicationService.h>

ApplicationService::ApplicationService() {
    url = QUrl(Configuration::instance().GetBaseUrl());
}

void ApplicationService::ListApplications(const QString &prefix) {
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
                          {"x-awsmock-target", "application"},
                          {"x-awsmock-action", "list-applications"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {
                              // The API returns an array containing one object: [{"q":"quote text", "a":"author"}]
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  ApplicationListResponse applicationResponse;
                                  applicationResponse.FromJson(jsonDoc);
                                  emit ReloadApplicationsSignal(applicationResponse);
                              } else {
                                  QMessageBox::critical(nullptr, "Error", "Response is not an object!");
                              }
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void ApplicationService::UploadApplication(const ApplicationUploadRequest &request) {
    _restManager.post(url,
                      request.ToJson().toUtf8(),
                      {
                          {"x-awsmock-target", "application"},
                          {"x-awsmock-action", "upload-application"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  QMessageBox::information(nullptr, "Information", "Application uploaded!");
                                  emit LoadAllApplications();
                              } else {
                                  QMessageBox::critical(nullptr, "Error", "Response is not an object!");
                              }
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void ApplicationService::CreateApplication(const ApplicationCreateRequest &request) {
    _restManager.post(url,
                      request.ToJson().toUtf8(),
                      {
                          {"x-awsmock-target", "application"},
                          {"x-awsmock-action", "create-application"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  QMessageBox::information(nullptr, "Information", "Application uploaded!");
                                  emit LoadAllApplications();
                              } else {
                                  QMessageBox::critical(nullptr, "Error", "Response is not an object!");
                              }
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void ApplicationService::GetApplication(const QString &name) {

    QJsonObject jRequest;
    jRequest["name"] = name;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "application"},
                          {"x-awsmock-action", "get-application"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  ApplicationGetResponse applicationResponse;
                                  applicationResponse.FromJson(jsonDoc.object());
                                  emit GetApplicationDetailsSignal(applicationResponse);
                              } else {
                                  QMessageBox::critical(nullptr, "Error", "Response is not an object!");
                              }
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void ApplicationService::StartApplication(const QString &name) {
    QJsonObject jApplication;
    jApplication["region"] = Configuration::instance().GetRegion();
    jApplication["name"] = name;

    QJsonObject jRequest;
    jRequest["application"] = jApplication;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "application"},
                          {"x-awsmock-action", "start-application"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadAllApplications();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void ApplicationService::StopApplication(const QString &name) {
    QJsonObject jApplication;
    jApplication["region"] = Configuration::instance().GetRegion();
    jApplication["name"] = name;

    QJsonObject jRequest;
    jRequest["application"] = jApplication;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "application"},
                          {"x-awsmock-action", "stop-application"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadAllApplications();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void ApplicationService::RestartApplication(const QString &name) {
    QJsonObject jApplication;
    jApplication["region"] = Configuration::instance().GetRegion();
    jApplication["name"] = name;

    QJsonObject jRequest;
    jRequest["application"] = jApplication;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "application"},
                          {"x-awsmock-action", "restart-application"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadAllApplications();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void ApplicationService::RestartAllApplications() {
    _restManager.post(url,
                      nullptr,
                      {
                          {"x-awsmock-target", "application"},
                          {"x-awsmock-action", "restart-all-applications"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadAllApplications();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void ApplicationService::ReloadApplication(const QString &name) {

    QJsonObject jApplication;
    jApplication["region"] = Configuration::instance().GetRegion();
    jApplication["name"] = name;

    QJsonObject jRequest;
    jRequest["application"] = jApplication;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "application"},
                          {"x-awsmock-action", "restart-application"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadAllApplications();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void ApplicationService::DeleteApplication(const QString &name) {
    QJsonObject jRequest;
    jRequest["name"] = name;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "application"},
                          {"x-awsmock-action", "delete-application"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit LoadAllApplications();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}
