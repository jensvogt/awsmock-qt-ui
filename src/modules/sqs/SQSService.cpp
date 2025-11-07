#include<modules/sqs/SQSService.h>

SQSService::SQSService() {
    url = QUrl(Configuration::instance().GetBaseUrl());
}

void SQSService::ListQueues(const QString &prefix, QTableWidget *tableWidget) {
    this->tableWidget = tableWidget;

    QJsonObject jSorting;
    jSorting["sortDirection"] = -1;
    jSorting["column"] = "attributes.approximateNumberOfMessages";

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
                          {"x-awsmock-target", "sqs"},
                          {"x-awsmock-action", "list-queue-counters"},
                          {"content-type", "application/json"}
                      },
                      [tableWidget](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {
                              // The API returns an array containing one object: [{"q":"quote text", "a":"author"}]

                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  tableWidget->setRowCount(0);
                                  tableWidget->setSortingEnabled(false); // stop sorting
                                  tableWidget->sortItems(-1);
                                  const QJsonArray counterArray = jsonDoc["queueCounters"].toArray();
                                  for (auto r = 0; r < counterArray.count(); r++) {
                                      tableWidget->insertRow(r);
                                      tableWidget->setItem(
                                          r, 0, new QTableWidgetItem(counterArray.at(r)["queueName"].toString()));
                                      auto *item1 = new QTableWidgetItem;
                                      item1->setData(Qt::EditRole, counterArray.at(r)["available"].toInt());
                                      tableWidget->setItem(r, 1, item1);
                                      auto *item2 = new QTableWidgetItem;
                                      item2->setData(Qt::EditRole, counterArray.at(r)["invisible"].toInt());
                                      tableWidget->setItem(r, 2, item2);
                                      auto *item3 = new QTableWidgetItem;
                                      item3->setData(Qt::EditRole, counterArray.at(r)["delayed"].toInt());
                                      tableWidget->setItem(r, 3, item3);
                                      auto *item4 = new QTableWidgetItem;
                                      item4->setData(Qt::EditRole, counterArray.at(r)["size"].toDouble());
                                      tableWidget->setItem(r, 4, item4);
                                      auto *item5 = new QTableWidgetItem;
                                      item5->setData(Qt::EditRole, counterArray.at(r)["created"].toString());
                                      tableWidget->setItem(r, 5, item5);
                                      auto *item6 = new QTableWidgetItem;
                                      item6->setData(Qt::EditRole, counterArray.at(r)["modified"].toString());
                                      tableWidget->setItem(r, 6, item6);
                                      auto *item7 = new QTableWidgetItem;
                                      item7->setData(Qt::EditRole, counterArray.at(r)["queueUrl"].toString());
                                      tableWidget->setItem(r, 7, item7);
                                      auto *item8 = new QTableWidgetItem;
                                      item8->setData(Qt::EditRole, counterArray.at(r)["queueArn"].toString());
                                      tableWidget->setItem(r, 8, item8);
                                      auto *checkItem = new QTableWidgetItem();
                                      checkItem->setCheckState(
                                          counterArray.at(r)["isDlq"].toBool() ? Qt::Checked : Qt::Unchecked);
                                      checkItem->setFlags(
                                          checkItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                                      tableWidget->setItem(r, 9, checkItem);
                                  }
                                  tableWidget->setRowCount(counterArray.count());
                                  tableWidget->setSortingEnabled(true);
                              } else {
                                  //m_quoteLabel->setText("Error: Failed to parse API response.");
                              }
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SQSService::PurgeQueue(const QString &queueUrl) {
    QJsonObject jRequest;
    jRequest["QueueUrl"] = queueUrl;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "purge-queue"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {
                              emit LoadContent();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SQSService::PurgeAllQueues() {
    _restManager.post(url,
                      nullptr,
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "purge-all-queues"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {
                              emit LoadContent();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SQSService::AddQueue(const QString &queueName) {
    QJsonObject jRequest;
    jRequest["QueueName"] = queueName;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "create-queue"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {
                              emit LoadContent();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SQSService::UpdateQueue(const UpdateQueueRequest &updateQueueRequest) {
    _restManager.post(url,
                      updateQueueRequest.ToJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "update-queue"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {
                              emit LoadContent();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SQSService::DeleteQueue(const QString &queueUrl) {
    QJsonObject jRequest;
    jRequest["QueueUrl"] = queueUrl;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "delete-queue"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {
                              emit LoadContent();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SQSService::RedriveQueue(const QString &queueArn) {
    QJsonObject jRequest;
    jRequest["QueueArn"] = queueArn;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "redrive-messages"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {
                              emit LoadContent();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SQSService::GetQueueDetails(const QString &queueArn) {
    QJsonObject jRequest;
    jRequest["QueueArn"] = queueArn;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "get-queue-details"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {
                              // The API returns an array containing one object
                              const QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                              SQSGetQueueDetailsResponse sqsResponse;
                              sqsResponse.FromJson(jsonDoc);

                              emit GetQueueDetailsSignal(sqsResponse);
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SQSService::GetSqsMessageDetails(const QString &messageId) {
    QJsonObject jRequest;
    jRequest["messageId"] = messageId;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "sqs"},
                          {"x-awsmock-action", "get-message-counters"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray& response, int status, const QString& error) {
                          if (success) {
                              // The API returns an JSON object
                              const QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                              SQSGetMessageDetailsResponse sqsResponse;
                              sqsResponse.FromJson(jsonDoc["message"].toObject());
                              emit GetSqsMessageDetailsSignal(sqsResponse);
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SQSService::ListMessages(const QString &queueArn, const QString &prefix, QTableWidget *tableWidget) {

    // Copy table widget
    this->tableWidget = tableWidget;

    QJsonObject jSorting;
    jSorting["sortDirection"] = -1;
    jSorting["column"] = "created";

    QJsonArray jSortingArray;
    jSortingArray.append(jSorting);

    QJsonObject jRequest;
    jRequest["queueArn"] = queueArn;
    jRequest["prefix"] = prefix;
    jRequest["pageSize"] = -1;
    jRequest["pageIndex"] = -1;
    jRequest["sortColumns"] = jSortingArray;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "sqs"},
                          {"x-awsmock-action", "list-message-counters"},
                          {"content-type", "application/json"}
                      },
                      [tableWidget](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {

                              // The API returns an array containing one object
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  tableWidget->clearContents();
                                  tableWidget->setRowCount(0);
                                  tableWidget->setSortingEnabled(false); // stop sorting
                                  tableWidget->sortItems(-1);
                                  const QJsonArray counterArray = jsonDoc["messageCounters"].toArray();
                                  for (auto r = 0; r < counterArray.count(); r++) {
                                      tableWidget->insertRow(r);
                                      tableWidget->setItem(
                                          r, 0, new QTableWidgetItem(counterArray.at(r)["messageId"].toString()));
                                      tableWidget->setItem(
                                          r, 1, new QTableWidgetItem(counterArray.at(r)["contentType"].toString()));

                                      auto *item1 = new QTableWidgetItem;
                                      item1->setData(Qt::EditRole, counterArray.at(r)["size"].toInt());
                                      tableWidget->setItem(r, 2, item1);

                                      auto *item2 = new QTableWidgetItem;
                                      item2->setData(Qt::EditRole, counterArray.at(r)["retires"].toInt());
                                      tableWidget->setItem(r, 3, item2);

                                      auto *item3 = new QTableWidgetItem;
                                      item3->setData(Qt::EditRole, counterArray.at(r)["created"].toString());
                                      tableWidget->setItem(r, 4, item3);

                                      auto *item4 = new QTableWidgetItem;
                                      item4->setData(Qt::EditRole, counterArray.at(r)["modified"].toString());
                                      tableWidget->setItem(r, 5, item4);
                                  }
                                  tableWidget->setRowCount(static_cast<int>(counterArray.count()));
                                  tableWidget->setSortingEnabled(true);
                              } else {
                                  QMessageBox::critical(nullptr, "Error", "Failed to parse API response.");
                              }
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SQSService::PurgeAllMessages(const QString &QueueUrl) {

    QJsonObject jRequest;
    jRequest["QueueUrl"] = QueueUrl;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "sqs"},
                          {"x-awsmock-action", "purge-Queue"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray& response, int status, const QString& error) {
                          if (success) {
                              emit LoadContent();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}
