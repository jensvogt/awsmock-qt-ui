
#include <modules/sqs/SQSService.h>

SQSService::SQSService() {
    url = QUrl(Configuration::instance().GetValue<QString>("server.base-url", "eu-central-1"));
}

void SQSService::ListQueues(const QString &prefix) {
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
                      [this](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              // The API returns an array od objects
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  SQSQueueListResponse sqsResponse;
                                  sqsResponse.FromJson(jsonDoc);
                                  emit ListQueuesSignal(sqsResponse);
                              } else {
                                  QMessageBox::critical(nullptr, "Error", "Response is not an object!");
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
                              emit ReloadQueuesSignal();
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
                              emit ReloadQueuesSignal();
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
                              emit ReloadQueuesSignal();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SQSService::UpdateQueue(const SQSQueueUpdateRequest &updateQueueRequest) {
    _restManager.post(url,
                      updateQueueRequest.ToJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "update-queue"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {
                              emit ReloadQueuesSignal();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SQSService::ListQueueAttributes(const QString &queueArn, const QString &prefix) {
    QJsonObject jSorting;
    jSorting["sortDirection"] = -1;
    jSorting["column"] = "name";

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
                          {"x-awsmock-action", "list-queue-attribute-counters"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              // The API returns an array od objects
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  SQSQueueAttributeListResponse sqsResponse;
                                  sqsResponse.FromJson(jsonDoc);
                                  emit ListQueueAttributesSignal(sqsResponse);
                              } else {
                                  QMessageBox::critical(nullptr, "Error", "Response is not an object!");
                              }
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SQSService::ListQueueLambdaTriggers(const QString &queueArn, const QString &prefix) {
    QJsonObject jSorting;
    jSorting["sortDirection"] = -1;
    jSorting["column"] = "name";

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
                          {"x-awsmock-action", "list-lambda-trigger-counters"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              // The API returns an array od objects
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  SQSListQueueLambdaTriggersResponse sqsResponse;
                                  sqsResponse.FromJson(jsonDoc);
                                  emit ListQueueLambdaTriggersSignal(sqsResponse);
                              } else {
                                  QMessageBox::critical(nullptr, "Error", "Response is not an object!");
                              }
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
                              emit ReloadQueuesSignal();
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
                          {"x-awsmock-target", "sqs"},
                          {"x-awsmock-action", "redrive-messages"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadQueuesSignal();
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
                          {"x-awsmock-target", "sqs"},
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
                      [this](const bool success, const QByteArray &response, int status, const QString &error) {
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

void SQSService::ListMessages(const QString &queueArn, const QString &prefix) {
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
                      [this](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              // The API returns an array of objects
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  SQSListMessagesResponse sqsResponse;
                                  sqsResponse.FromJson(jsonDoc);
                                  emit ListMessagesSignal(sqsResponse);
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
                          {"x-awsmock-action", "purge-queue"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {
                              emit ReloadMessagesSignal();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SQSService::SendMessage(const SQSSendMessageRequest &request) {

    _restManager.post(url,
                      request.ToJson(),
                      {
                          {"x-awsmock-target", "sqs"},
                          {"x-awsmock-action", "send-message"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              // The API returns an array od objects
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  SQSSendMessageResponse sqsResponse;
                                  sqsResponse.FromJson(jsonDoc);
                                  emit SendMessagesSignal(sqsResponse);
                              } else {
                                  QMessageBox::critical(nullptr, "Error", "Response is not an object!");
                              }
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SQSService::DeleteMessage(const QString &queueUrl, const QString &receiptHandle) {

    QJsonObject jRequest;
    jRequest["QueueUrl"] = queueUrl;
    jRequest["ReceiptHandle"] = receiptHandle;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "sqs"},
                          {"x-awsmock-action", "delete-message"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int status, const QString &error) {
                          if (success) {
                              emit ReloadMessagesSignal();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

