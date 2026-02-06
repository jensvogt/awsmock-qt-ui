#include <modules/kms/KMSService.h>

void KMSService::ListKmsKeys(const QString &prefix) {
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
                          {"x-awsmock-target", "kms"},
                          {"x-awsmock-action", "list-key-counters"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  KMSListKeysResponse kmsResponse;
                                  kmsResponse.FromJson(jsonDoc);
                                  emit ListKeysSignal(kmsResponse);
                              } else {
                                  qCritical() << "Response is not an array!";
                              }
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().DockerStatsTimerSignal("ListKmsKeys", timer.elapsed());
                      });
}

void KMSService::DeleteKey(const QString &keyId) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["keyId"] = keyId;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "kms"},
                          {"x-awsmock-action", "delete-key"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, QByteArray, int, const QString &error) {
                          if (success) {
                              emit ReloadKeySignal();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                          emit EventBus::instance().TimerSignal("DeleteKey", timer.elapsed());
                      });
}
