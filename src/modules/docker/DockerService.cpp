#include <modules/docker/DockerService.h>

DockerService::DockerService() {
    url = QUrl(Configuration::instance().GetValue<QString>("server.base-url", "http://localhost:4566"));
}

void DockerService::ListDockerContainer(const QString &prefix) {
    QElapsedTimer timer;
    timer.start();

    const QJsonObject jRequest = CreateBaseRequest();
    jRequest["prefix"] = prefix;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "container"},
                          {"x-awsmock-action", "list-containers"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              // The API returns an array contains an array of docker statistics
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  DockerContainersResponse dockerResponse;
                                  dockerResponse.FromJson(jsonDoc);
                                  emit ReloadDockerContainerSignal(dockerResponse);
                              } else {
                                  qCritical() << "Response is not an array!";
                              }
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().DockerStatsTimerSignal("ListDockerContainer", timer.elapsed());
                      });
}

void DockerService::ListDockerStats() {
    QElapsedTimer timer;
    timer.start();
    const QJsonDocument requestDoc(CreateBaseRequest());

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "container"},
                          {"x-awsmock-action", "list-container-stats"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              // The API returns an array contains an array of docker statistics
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  DockerStatsResponse dockerResponse;
                                  dockerResponse.FromJson(jsonDoc);
                                  emit ReloadDockerStatsSignal(dockerResponse);
                              } else {
                                  qCritical() << "Response is not an object!";
                              }
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().DockerStatsTimerSignal("ListDockerStats", timer.elapsed());
                      });
}

void DockerService::StartContainer(const QString &containerId) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["containerId"] = containerId;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "container"},
                          {"x-awsmock-action", "start-container"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadContainerList();
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().DockerStatsTimerSignal("StartContainer", timer.elapsed());
                      });
}

void DockerService::StopContainer(const QString &containerId) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["containerId"] = containerId;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "container"},
                          {"x-awsmock-action", "stop-container"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadContainerList();
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().DockerStatsTimerSignal("StopContainer", timer.elapsed());
                      });
}

void DockerService::RestartContainer(const QString &containerId) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["containerId"] = containerId;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "container"},
                          {"x-awsmock-action", "restart-container"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadContainerList();
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().DockerStatsTimerSignal("RestartContainer", timer.elapsed());
                      });
}

void DockerService::KillContainer(const QString &containerId) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["containerId"] = containerId;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "container"},
                          {"x-awsmock-action", "kill-container"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadContainerList();
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().DockerStatsTimerSignal("KillContainer", timer.elapsed());
                      });
}

void DockerService::DeleteContainer(const QString &containerId) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["containerId"] = containerId;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "container"},
                          {"x-awsmock-action", "delete-container"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadContainerList();
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().DockerStatsTimerSignal("KillContainer", timer.elapsed());
                      });
}
