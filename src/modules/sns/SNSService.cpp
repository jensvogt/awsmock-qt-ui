#include <modules/sns/SNSService.h>

#include "dto/sns/SNSSubscriptionResponse.h"

void SNSService::AddTopic(const QString &region, const QString &topicName) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["region"] = region;
    jRequest["topicName"] = topicName;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "create-topic"},
                          {"content-type", "application/json"}
                      },
                      [this,timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadMessagesSignal();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance()
                                  .
                                  TimerSignal("AddTopic", timer.elapsed());
                      });
}

void SNSService::ListTopics(const QString &prefix, const long pageSize, const long pageIndex, const QString &sortAttribute, const int sortDirection) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jSorting;
    jSorting["sortDirection"] = sortDirection;
    jSorting["column"] = sortAttribute;

    QJsonArray jSortingArray;
    jSortingArray.append(jSorting);

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["prefix"] = prefix;
    jRequest["pageSize"] = static_cast<qlonglong>(pageSize);
    jRequest["pageIndex"] = static_cast<qlonglong>(pageIndex);
    jRequest["sortColumns"] = jSortingArray;

    _restManager.post(GetBaseUrl(),
                      QJsonDocument(jRequest).toJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "list-topic-counters"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  SNSListTopicResult snsResponse;
                                  snsResponse.FromJson(jsonDoc);
                                  emit ListTopicSignal(snsResponse);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance()
                                  .
                                  TimerSignal("ListTopics", timer.elapsed());
                      });
}

void SNSService::ListTopicAttributes(const QString &topicArn) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jSorting;
    jSorting["sortDirection"] = -1;
    jSorting["column"] = "key";

    QJsonArray jSortingArray;
    jSortingArray.append(jSorting);

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["topicArn"] = topicArn;
    jRequest["prefix"] = "";
    jRequest["pageSize"] = -1;
    jRequest["pageIndex"] = -1;
    jRequest["sortColumns"] = jSortingArray;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "list-attribute-counters"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  ListTopicAttributesResponse snsResponse;
                                  snsResponse.FromJson(jsonDoc);
                                  emit ListTopicAttributesSignal(snsResponse);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance()
                                  .
                                  TimerSignal("ListTopicAttributes", timer.elapsed());
                      });
}

void SNSService::ListTopicTags(const QString &topicArn) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jSorting;
    jSorting["sortDirection"] = -1;
    jSorting["column"] = "name";

    QJsonArray jSortingArray;
    jSortingArray.append(jSorting);

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["topicArn"] = topicArn;
    jRequest["prefix"] = "";
    jRequest["pageSize"] = -1;
    jRequest["pageIndex"] = -1;
    jRequest["sortColumns"] = jSortingArray;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "list-tag-counters"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  ListTopicTagsResponse snsResponse;
                                  snsResponse.FromJson(jsonDoc);
                                  emit ListTopicTagsSignal(snsResponse);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance()
                                  .
                                  TimerSignal("ListTopicTags", timer.elapsed());
                      });
}

void SNSService::ListTopicSubscriptions(const QString &topicArn) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jSorting;
    jSorting["sortDirection"] = -1;
    jSorting["column"] = "id";

    QJsonArray jSortingArray;
    jSortingArray.append(jSorting);

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["topicArn"] = topicArn;
    jRequest["prefix"] = "";
    jRequest["pageSize"] = -1;
    jRequest["pageIndex"] = -1;
    jRequest["sortColumns"] = jSortingArray;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "list-subscription-counters"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  ListTopicSubscriptionsResponse snsResponse;
                                  snsResponse.FromJson(jsonDoc);
                                  emit ListTopicSubscriptionsSignal(snsResponse);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance()
                                  .
                                  TimerSignal("ListTopicSubscriptions", timer.elapsed());
                      });
}

void SNSService::ListMessages(const QString &topicArn, const QString &prefix, const long pageSize, const long pageIndex, const QString &sortAttribute, const int sortDirection) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jSorting;
    jSorting["sortDirection"] = sortDirection;
    jSorting["column"] = sortAttribute;

    QJsonArray jSortingArray;
    jSortingArray.append(jSorting);

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["topicArn"] = topicArn;
    jRequest["prefix"] = prefix;
    jRequest["pageSize"] = static_cast<qint64>(pageSize);
    jRequest["pageIndex"] = static_cast<qint64>(pageIndex);
    jRequest["sortColumns"] = jSortingArray;

    _restManager.post(GetBaseUrl(),
                      QJsonDocument(jRequest).toJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "list-message-counters"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              // The API returns an array of objects
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  SNSListMessagesResult snsResponse;
                                  snsResponse.FromJson(jsonDoc);
                                  emit ListMessagesSignal(snsResponse);
                              } else {
                                  logError << error;
                              }
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance()
                                  .
                                  TimerSignal("ListMessages", timer.elapsed());
                      });
}

void SNSService::PurgeTopic(const QString &topicArn) {
    QElapsedTimer timer;
    timer.start();

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
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadMessagesSignal();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance()
                                  .
                                  TimerSignal("PurgeTopic", timer.elapsed());
                      });
}

void SNSService::PurgeAllTopics() {
    QElapsedTimer timer;
    timer.start();
    _restManager.post(GetBaseUrl(),
                      nullptr,
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "purge-all-topics"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadMessagesSignal();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance()
                                  .
                                  TimerSignal("PurgeAllTopics", timer.elapsed());
                      });
}

void SNSService::PurgeMessages(const QString &topicArn) {
    QElapsedTimer timer;
    timer.start();

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
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadMessagesSignal();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance()
                                  .
                                  TimerSignal("PurgeMessages", timer.elapsed());
                      });
}

void SNSService::GetTopicDetails(const QString &topicArn) {
    QElapsedTimer timer;
    timer.start();

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
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              // The API returns an JSON document
                              const QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                              SNSGetTopicDetailsResponse snsResponse;
                              snsResponse.FromJson(jsonDoc);
                              emit GetTopicDetailsSignal(snsResponse);
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance()
                                  .
                                  TimerSignal("GetTopicDetails", timer.elapsed());
                      });
}

void SNSService::ResetMessageCounters() {
    QElapsedTimer timer;
    timer.start();

    _restManager.post(GetBaseUrl(),
                      nullptr,
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "reload-all-counters"},
                          {"content-type", "application/json"}
                      },
                      [timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (!success) {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("ResetMessageCounters", timer.elapsed());
                      });
}

void SNSService::ListTopicDefaultAttributes(const QString &topicArn, const QString &prefix) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jSorting;
    jSorting["sortDirection"] = -1;
    jSorting["column"] = "name";

    QJsonArray jSortingArray;
    jSortingArray.append(jSorting);

    QJsonObject jRequest;
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
                          {"x-awsmock-action", "list-default-message-attribute-counters"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              // The API returns an array od objects
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  SNSListQueueDefaultAttributesResponse snsResponse;
                                  snsResponse.FromJson(jsonDoc);
                                  emit ListTopicDefaultAttributesSignal(snsResponse);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("ListQueueDefaultAttributes", timer.elapsed());
                      });
}

void SNSService::AddTopicDefaultAttributes(const QString &topicArn, const QString &key, const MessageAttribute &attribute) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest;
    jRequest["topicArn"] = topicArn;
    jRequest["name"] = key;
    jRequest["messageAttribute"] = attribute.ToJsonObject();
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      QJsonDocument(jRequest).toJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "add-default-message-attribute-counter"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              emit ReloadTopicDefaultAttributesSignal();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("AddTopicDefaultAttributes", timer.elapsed());
                      });
}

void SNSService::UpdateTopicDefaultAttributes(const QString &topicArn, const QString &key, const QString &value, const QString &dataType) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest;
    jRequest["topicArn"] = topicArn;
    jRequest["name"] = key;
    jRequest["value"] = value;
    jRequest["dataType"] = dataType;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      QJsonDocument(jRequest).toJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "update-default-message-attribute-counter"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadTopicDefaultAttributesSignal();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("UpdateTopicDefaultAttributes", timer.elapsed());
                      });
}

void SNSService::DeleteTopicDefaultAttributes(const QString &topicArn, const QString &key) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest;
    jRequest["topicArn"] = topicArn;
    jRequest["name"] = key;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      QJsonDocument(jRequest).toJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "delete-default-message-attribute-counter"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadTopicDefaultAttributesSignal();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("DeleteTopicDefaultAttributes", timer.elapsed());
                      });
}

void SNSService::DeleteTopic(const QString &topicArn) {
    QElapsedTimer timer;
    timer.start();

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
                      [this, timer](const bool success, QByteArray, int, const QString &error) {
                          if (success) {
                              emit ReloadMessagesSignal();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance()
                                  .
                                  TimerSignal("DeleteTopic", timer.elapsed());
                      });
}

void SNSService::SendMessage(const SNSSendMessageRequest &request) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jAttributes;
    for (const auto &[key,value]: request.messageAttributes) {
        QJsonObject jAttribute;
        jAttribute["DataType"] = MessageAttributeDataTypeToString(value.dataType);
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
                              const QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                              SNSSendMessageResponse snsResponse;
                              snsResponse.FromJson(jsonDoc);
                              emit SendMessagesSignal(snsResponse);
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("SendMessage", timer.elapsed());
                      });
}

void SNSService::AddSubscription(const QString &topicArn, const SNSTopicSubscription &topicSubscription) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["topicArn"] = topicArn;
    jRequest["protocol"] = topicSubscription.protocol;
    jRequest["endpoint"] = topicSubscription.endpoint;
    jRequest["owner"] = topicSubscription.owner;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "add-subscription-counter"},
                          {"content-type", "application/json"}
                      },
                      [this,timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit AddTopicSubscriptionSignal();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("AddSubscription", timer.elapsed());
                      });
}

void SNSService::GetSubscription(const QString &topicArn, const QString &subscriptionArn) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["topicArn"] = topicArn;
    jRequest["subscriptionArn"] = subscriptionArn;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "get-subscription-counter"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              // The API returns an array containing one object
                              const QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                              SNSSubscriptionResponse snsResponse;
                              snsResponse.FromJson(jsonDoc);
                              emit GetSubscriptionSignal(snsResponse);
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("GetSubscription", timer.elapsed());
                      });
}

void SNSService::DeleteTopicSubscription(const QString &topicArn, const QString &subscriptionArn) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["topicArn"] = topicArn;
    jRequest["subscriptionArn"] = subscriptionArn;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "sns"},
                          {"x-awsmock-action", "delete-subscription-counter"},
                          {"content-type", "application/json"}
                      },
                      [timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (!success) {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("AddSubscription", timer.elapsed());
                      });
}

void SNSService::GetSnsMessageDetails(const QString &messageId) {
    QElapsedTimer timer;
    timer.start();

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
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              // The API returns an array containing one object
                              const QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                              SNSGetMessageDetailsResponse snsResponse;
                              snsResponse.FromJson(jsonDoc["message"].toObject());
                              emit GetMessageDetailsSignal(snsResponse);
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("SendMessage", timer.elapsed());
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
                              logError << error;
                          }
                      });
}
