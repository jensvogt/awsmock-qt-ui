
#include <modules/ssm/SSMService.h>

void SSMService::ListParameters(const QString &prefix) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jSorting;
    jSorting["sortDirection"] = -1;
    jSorting["column"] = "messages";

    QJsonArray jSortingArray;
    jSortingArray.append(jSorting);

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["prefix"] = prefix;
    jRequest["pageSize"] = -1;
    jRequest["pageIndex"] = -1;
    jRequest["sortColumns"] = jSortingArray;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "ssm"},
                          {"x-awsmock-action", "list-parameter-counters"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              //qDebug() << QString(response);
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  SSMParameterListResponse ssmResponse;
                                  ssmResponse.FromJson(jsonDoc);
                                  emit ListParameterSignal(ssmResponse);
                              } else {
                                  qCritical() << "Response is not an object!";
                              }
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().TimerSignal("ListParameters", timer.elapsed());
                      });
}

void SSMService::GetParameter(const QString &name) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["Name"] = name;
    jRequest["WithDescription"] = true;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "ssm"},
                          {"x-awsmock-action", "get-parameter-counter"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              qDebug() << QString(response);
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  SSMParameterGetResponse ssmResponse;
                                  ssmResponse.FromJson(jsonDoc);
                                  emit GetParameterSignal(ssmResponse);
                              } else {
                                  qCritical() << "Response is not an object!";
                              }
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().TimerSignal("ListParameters", timer.elapsed());
                      });
}
