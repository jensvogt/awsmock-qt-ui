#include <modules/ssm/SSMService.h>

#include "utils/Logging.h"

void SSMService::CreateParameter(const SSMParameterCounter &parameter) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["region"] = parameter.region;
    jRequest["name"] = parameter.name;
    jRequest["value"] = parameter.parameterValue;
    jRequest["description"] = parameter.description;
    jRequest["type"] = ParameterTypeToString(parameter.type);
    jRequest["kmsKeyArn"] = parameter.kmsKeyArn;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "ssm"},
                          {"x-awsmock-action", "create-parameter-counter"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadParameterListSignal();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("ListParameters", timer.elapsed());
                      });
}

void SSMService::ListParameters(const QString &prefix, const int sortColumn, const int sortDirection) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jSorting;
    jSorting["sortDirection"] = sortDirection;
    jSorting["column"] = sortColumn == 0 ? "name" : "";

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
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  SSMParameterListResponse ssmResponse;
                                  ssmResponse.FromJson(jsonDoc);
                                  emit ListParameterSignal(ssmResponse);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
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
                              //JsonUtils::WriteJsonString(QJsonDocument::fromJson(response).object());
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  SSMParameterGetResponse ssmResponse;
                                  ssmResponse.FromJson(jsonDoc);
                                  emit GetParameterSignal(ssmResponse);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("ListParameters", timer.elapsed());
                      });
}

void SSMService::UpdateParameter(const SSMParameterCounter &parameter) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jSorting;
    jSorting["sortDirection"] = -1;
    jSorting["column"] = "messages";

    QJsonArray jSortingArray;
    jSortingArray.append(jSorting);

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["region"] = parameter.region;
    jRequest["name"] = parameter.name;
    jRequest["value"] = parameter.parameterValue;
    jRequest["description"] = parameter.description;
    jRequest["type"] = ParameterTypeToString(parameter.type);
    jRequest["kmsKeyArn"] = parameter.kmsKeyArn;
    jRequest["prefix"] = "";
    jRequest["pageSize"] = -1;
    jRequest["pageIndex"] = -1;
    jRequest["sortColumns"] = jSortingArray;

    if (!parameter.tags.isEmpty()) {
        QJsonObject jTagsObject;
        for (auto it = parameter.tags.cbegin(); it != parameter.tags.cend(); ++it) {
            jTagsObject.insert(it.key(), it.value());
        }
        jRequest["tags"] = jTagsObject;
    }
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "ssm"},
                          {"x-awsmock-action", "update-parameter-counter"},
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
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("ListParameters", timer.elapsed());
                      });
}

void SSMService::DeleteParameter(const QString &parameterName) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["Name"] = parameterName;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "ssm"},
                          {"x-awsmock-action", "delete-parameter"},
                          {"content-type", "application/json"}
                      },
                      [this,timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              emit ReloadParameterListSignal();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("DeleteParameter", timer.elapsed());
                      });
}
