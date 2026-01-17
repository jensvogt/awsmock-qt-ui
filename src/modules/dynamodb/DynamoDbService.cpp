
#include <modules/dynamodb/DynamoDbService.h>

void DynamoDbService::CreateTable(const DynamoDbCreateTableRequest &request) {
    QElapsedTimer timer;
    timer.start();

    const QJsonDocument requestDoc(request.ToJsonObject());

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "dynamodb"},
                          {"x-awsmock-action", "create-table"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              // The API returns an array od objects
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  DynamoDbCreateTableResponse dynamodbResponse;
                                  dynamodbResponse.FromJson(jsonDoc);
                                  emit CreateTableSignal(dynamodbResponse);
                              } else {
                                  qCritical() << "Response is not an object!";
                              }
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().TimerSignal("ListTables", timer.elapsed());
                      });
}

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

void DynamoDbService::DescribeTable(const QString &tableName) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["TableName"] = tableName;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "dynamodb"},
                          {"x-awsmock-action", "describe-table"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  DynamoDbDescribeTableResponse dynamodbResponse;
                                  dynamodbResponse.FromJson(QJsonDocument(jsonDoc.object()["Table"].toObject()));
                                  emit DescribeTableSignal(dynamodbResponse);
                              }
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().TimerSignal("DescribeTable", timer.elapsed());
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
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadTableListSignal();
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().TimerSignal("DeleteTable", timer.elapsed());
                      });
}

void DynamoDbService::ListItems(const QString &tableName) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["TableName"] = tableName;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "dynamodb"},
                          {"x-awsmock-action", "scan"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  DynamoDbListItemResponse dynamodbResponse;
                                  dynamodbResponse.FromJson(jsonDoc);
                                  emit ListItemsSignal(dynamodbResponse);
                              } else {
                                  qCritical() << "Response is not an object!";
                              }
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().TimerSignal("ListTables", timer.elapsed());
                      });
}

void DynamoDbService::PurgeTable(const QString &tableName) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["TableName"] = tableName;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "dynamodb"},
                          {"x-awsmock-action", "delete-all-items"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  DynamoDbListItemResponse dynamodbResponse;
                                  dynamodbResponse.FromJson(jsonDoc);
                                  emit ListItemsSignal(dynamodbResponse);
                              } else {
                                  qCritical() << "Response is not an object!";
                              }
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().TimerSignal("ListTables", timer.elapsed());
                      });
}
