#include <modules/docker/DockerService.h>

DockerService::DockerService() {
    url = QUrl(Configuration::instance().GetValue<QString>("server.base-url", "http://localhost:4566"));
}

void DockerService::ListDockerContainer(const QString &prefix) {
    QElapsedTimer timer;
    timer.start();

    const QJsonObject jRequest;
    jRequest["prefix"] = prefix;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "container"},
                          {"x-awsmock-action", "list-containers"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {
                              // The API returns an array contains an array of docker statistics
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  DockerContainersResponse dockerResponse;
                                  dockerResponse.FromJson(jsonDoc);
                                  emit ReloadDockerContainerSignal(dockerResponse);
                              } else {
                                  QMessageBox::critical(nullptr, "Error", "Response is not an array!");
                              }
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                          emit EventBus::instance().TimerSignal("GetDockerStats", timer.elapsed());
                      });
}

void DockerService::ListDockerStats(const QList<QString> &containerIds) {
    QElapsedTimer timer;
    timer.start();

    QJsonArray array;
    for (const auto &containerId: containerIds) {
        array.append(containerId);
    }

    QJsonObject jRequest;
    jRequest["containerIds"] = array;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "container"},
                          {"x-awsmock-action", "list-container-stats"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {
                              // The API returns an array contains an array of docker statistics
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  DockerStatsResponse dockerResponse;
                                  dockerResponse.FromJson(jsonDoc);
                                  emit ReloadDockerStatsSignal(dockerResponse);
                              } else {
                                  QMessageBox::critical(nullptr, "Error", "Response is not an object!");
                              }
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                          emit EventBus::instance().TimerSignal("GetDockerStats", timer.elapsed());
                      });
}
