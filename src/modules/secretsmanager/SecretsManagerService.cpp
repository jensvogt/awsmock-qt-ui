#include <modules/secretsmanager/SecretsManagerService.h>

void SecretsManagerService::CreateSecret(const SecretCounter &secretCounter) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["Name"] = secretCounter.name;
    jRequest["SecretString"] = secretCounter.secretString;
    jRequest["Description"] = secretCounter.description;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "secretsmanager"},
                          {"x-awsmock-action", "CreateSecret"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              // The API returns an JSON secretsManager counter list
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  SecretCounter s;
                                  s.FromJson(jsonDoc.object());
                                  emit LoadAllSecrets();
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("UpdateSecret", timer.elapsed());
                      });
}

void SecretsManagerService::ListSecrets(const QString &prefix, const long pageSize, const long pageIndex, const QString &sortAttribute, const int sortDirection) {
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
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
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
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("GetSecret", timer.elapsed());
                      });
}

void SecretsManagerService::GetVersions(const QString &secretId) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jSorting;
    jSorting["sortDirection"] = -1;
    jSorting["column"] = "name";

    QJsonArray jSortingArray;
    jSortingArray.append(jSorting);

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["secretId"] = secretId;
    jRequest["prefix"] = "";
    jRequest["pageSize"] = -1;
    jRequest["pageIndex"] = -1;
    jRequest["sortColumns"] = jSortingArray;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "secretsmanager"},
                          {"x-awsmock-action", "ListSecretVersions"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  SecretGetVersionResponse secretVersionResponse;
                                  secretVersionResponse.FromJson(jsonDoc.object());
                                  emit GetSecretsVersionsSignal(secretVersionResponse);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("GetVersions", timer.elapsed());
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
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  SecretCounter s;
                                  s.FromJson(jsonDoc.object());
                                  emit GetSecretsDetailsSignal(s);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("UpdateSecret", timer.elapsed());
                      });
}

void SecretsManagerService::DeleteSecret(const QString &secretId) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["SecretId"] = secretId;
    jRequest["ForceDeleteWithoutRecovery"] = true;
    jRequest["RecoveryWindowInDays"] = 7;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "secretsmanager"},
                          {"x-awsmock-action", "DeleteSecret"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              emit LoadAllSecrets();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("DeleteSecret", timer.elapsed());
                      });
}
