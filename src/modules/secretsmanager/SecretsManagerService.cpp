#include <modules/secretsmanager/SecretsManagerService.h>

void SecretsManagerService::ListSecrets() {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jSorting;
    jSorting["sortDirection"] = -1;
    jSorting["column"] = "name";

    QJsonArray jSortingArray;
    jSortingArray.append(jSorting);

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["prefix"] = "";
    jRequest["pageSize"] = -1;
    jRequest["pageIndex"] = -1;
    jRequest["sortColumns"] = jSortingArray;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "secretsmanager"},
                          {"x-awsmock-action", "ListSecretCounters"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {
                              // The API returns an JSON secretsManager counter list
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  SecretsListResponse secretsListResponse;
                                  secretsListResponse.FromJson(jsonDoc);
                                  emit ReloadSecretsSignal(secretsListResponse);
                              } else {
                                  QMessageBox::critical(nullptr, "Error", "Response is not an object!");
                              }
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                          emit EventBus::instance().TimerSignal("ListSecrets", timer.elapsed());
                      });
}

void SecretsManagerService::GetSecret(const QString &secretId) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["SecretId"] = secretId;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "secretsmanager"},
                          {"x-awsmock-action", "GetSecretDetails"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {
                              // The API returns an JSON secretsManager counter list
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  SecretCounter secretCounter;
                                  secretCounter.FromJson(jsonDoc.object());
                                  emit GetSecretsDetailsSignal(secretCounter);
                              } else {
                                  QMessageBox::critical(nullptr, "Error", "Response is not an object!");
                              }
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                          emit EventBus::instance().TimerSignal("GetSecret", timer.elapsed());
                      });
}

void SecretsManagerService::UpdateSecret(const SecretCounter &secretCounter) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["secretDetails"] = secretCounter.ToJsonObject();
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "secretsmanager"},
                          {"x-awsmock-action", "UpdateDetailsSecret"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              // The API returns an JSON secretsManager counter list
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  SecretCounter s;
                                  s.FromJson(jsonDoc.object());
                                  emit GetSecretsDetailsSignal(s);
                              } else {
                                  QMessageBox::critical(nullptr, "Error", "Response is not an object!");
                              }
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                          emit EventBus::instance().TimerSignal("UpdateSecret", timer.elapsed());
                      });
}
