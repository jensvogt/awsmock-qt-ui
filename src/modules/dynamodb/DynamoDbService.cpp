
#include <modules/dynamodb/DynamoDbService.h>

void DynamoDbService::ListTables(const QString &prefix) {
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
                          {"x-awsmock-target", "dynamodb"},
                          {"x-awsmock-action", "list-table-counters"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              // The API returns an array od objects
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  DynamoDbListTableResponse dynamodbResponse;
                                  dynamodbResponse.FromJson(jsonDoc);
                                  emit ListTablesSignal(dynamodbResponse);
                              } else {
                                  qCritical() << "Response is not an object!";
                              }
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().TimerSignal("ListTables", timer.elapsed());
                      });
}

void DynamoDbService::DeleteTable(const QString &tableName) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["TableName"] = tableName;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "dynamodb"},
                          {"x-awsmock-action", "delete-table"},
                          {"content-type", "application/json"}
                      },
                      [timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (!success) {
                              qCritical() << error;
                          }
                          emit EventBus::instance().TimerSignal("DeleteTable", timer.elapsed());
                      });
}
