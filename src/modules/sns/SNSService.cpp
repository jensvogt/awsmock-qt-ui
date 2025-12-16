
#include <QUrlQuery>
#include <modules/sns/SNSService.h>

void SNSService::AddTopic(const QString &region, const QString &topicName) {
    QJsonObject jRequest = CreateBaseRequest();
    jRequest["topicName"] = topicName;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "create-topic"},
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

void SNSService::ListTopics(const QString &prefix) {
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
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "list-topic-counters"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              // The API returns an array od objects
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  SNSListTopicResult snsResponse;
                                  snsResponse.FromJson(jsonDoc);
                                  emit ListTopicSignal(snsResponse);
                              } else {
                                  QMessageBox::critical(nullptr, "Error", "Response is not an object!");
                              }
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SNSService::ListMessages(const QString &topicArn, const QString &prefix) {
    QJsonObject jSorting;
    jSorting["sortDirection"] = -1;
    jSorting["column"] = "created";

    QJsonArray jSortingArray;
    jSortingArray.append(jSorting);

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["topicArn"] = topicArn;
    jRequest["prefix"] = prefix;
    jRequest["pageSize"] = -1;
    jRequest["pageIndex"] = -1;
    jRequest["sortColumns"] = jSortingArray;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "list-message-counters"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              // The API returns an array of objects
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  SNSListMessagesResult snsResponse;
                                  snsResponse.FromJson(jsonDoc);
                                  emit ListMessagesSignal(snsResponse);
                              } else {
                                  //m_quoteLabel->setText("Error: Failed to parse API response.");
                              }
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SNSService::PurgeTopic(const QString &topicArn) {
    QJsonObject jRequest = CreateBaseRequest();
    jRequest["topicArn"] = topicArn;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "purge-topic"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadMessagesSignal();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SNSService::PurgeAllTopics() {
    _restManager.post(GetBaseUrl(),
                      nullptr,
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "purge-all-topics"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadMessagesSignal();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SNSService::PurgeMessages(const QString &topicArn) {
    QJsonObject jRequest = CreateBaseRequest();
    jRequest["topicArn"] = topicArn;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "purge-topic"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadMessagesSignal();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SNSService::GetTopicDetails(const QString &topicArn) {
    QJsonObject jRequest = CreateBaseRequest();
    jRequest["topicArn"] = topicArn;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "get-topic-details"},
                          {"content-type", "application/json"}
                      },
                      [this](bool success, QByteArray response, int status, QString error) {
                          if (success) {
                              // The API returns an JSON document
                              QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                              SNSGetTopicDetailsResponse response;
                              response.FromJson(jsonDoc);

                              emit GetTopicDetailsSignal(response);
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SNSService::DeleteTopic(const QString &topicArn) {
    QJsonObject jRequest = CreateBaseRequest();
    jRequest["topicArn"] = topicArn;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "delete-topic"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, QByteArray, int, const QString &error) {
                          if (success) {
                              emit ReloadMessagesSignal();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SNSService::SendMessage(const SNSSendMessageRequest &request) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jAttributes;
    for (const auto &[key,value]: request.messageAttributes) {
        QJsonObject jAttribute;
        jAttribute["DataType"] = value.dataType;
        jAttribute["StringValue"] = value.stringValue;
        jAttributes[key] = jAttribute;
    }

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["TopicArn"] = request.topicArn;
    jRequest["Message"] = request.body;
    jRequest["MessageAttributes"] = jAttributes;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "publish-counter"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                              SNSSendMessageResponse snsResponse;
                              snsResponse.FromJson(jsonDoc);
                              emit SendMessagesSignal(snsResponse);
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                          emit EventBus::instance().TimerSignal("GetMultiSeriesCounter", timer.elapsed());
                      });
}

void SNSService::GetSnsMessageDetails(const QString &messageId) {
    QJsonObject jRequest = CreateBaseRequest();
    jRequest["messageId"] = messageId;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "get-message-counters"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              // The API returns an array containing one object
                              const QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                              SNSGetMessageDetailsResponse snsResponse;
                              snsResponse.FromJson(jsonDoc["message"].toObject());
                              emit GetMessageDetailsSignal(snsResponse);
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SNSService::DeleteMessage(const QString &topicArn, const QString &messageId) {
    QJsonObject jRequest = CreateBaseRequest();
    jRequest["topicArn"] = topicArn;
    jRequest["messageId"] = messageId;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "delete-message"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, QByteArray, int, const QString &error) {
                          if (success) {
                              emit ReloadMessagesSignal();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}
