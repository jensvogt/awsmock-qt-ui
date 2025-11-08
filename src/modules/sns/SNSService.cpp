
#include <modules/sns/SNSService.h>

SNSService::SNSService() {
    url = QUrl(Configuration::instance().GetBaseUrl());
}

void SNSService::AddTopic(const QString &region, const QString &topicName) {
    QJsonObject jRequest;
    jRequest["region"] = region;
    jRequest["topicName"] = topicName;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {{"x-awsmock-target", "sns"},
                       {"x-awsmock-action", "create-topic"},
                       {"content-type", "application/json"}},
                      [this](const bool success, const QByteArray& response, int status, const QString& error) {
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

    QJsonObject jRequest;
    jRequest["prefix"] = prefix;
    jRequest["pageSize"] = -1;
    jRequest["pageIndex"] = -1;
    jRequest["sortColumns"] = jSortingArray;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {{"x-awsmock-target", "sns"},
                       {"x-awsmock-action", "list-topic-counters"},
                       {"content-type", "application/json"}},
                      [this](const bool success, const QByteArray& response, int, const QString& error) {
                          if (success) {

                              // The API returns an array od objects
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  SNSListTopicResult snsResponse;
                                  snsResponse.FromJson(jsonDoc);
                                  emit ListTopicSignal(snsResponse);

                              } else {
                                  QMessageBox::critical(nullptr,"Error", "Response is not an object!");
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

    QJsonObject jRequest;
    jRequest["topicArn"] = topicArn;
    jRequest["prefix"] = prefix;
    jRequest["pageSize"] = -1;
    jRequest["pageIndex"] = -1;
    jRequest["sortColumns"] = jSortingArray;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {{"x-awsmock-target", "sns"},
                       {"x-awsmock-action", "list-message-counters"},
                       {"content-type", "application/json"}},
                      [this](const bool success, const QByteArray& response, int, const QString& error) {
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

void SNSService::PurgeTopic(const QString &topicArn)
{
    QJsonObject jRequest;
    jRequest["topicArn"] = topicArn;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {{"x-awsmock-target", "sns"},
                       {"x-awsmock-action", "list-message-counters"},
                       {"content-type", "application/json"}},
                      [this](bool success, QByteArray response, int status, QString error) {
                          if (success) {
                              emit ReloadMessagesSignal();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SNSService::PurgeAllTopics(){
    _restManager.post(url,
                      nullptr,
                      {{"x-awsmock-target", "sns"},
                       {"x-awsmock-action", "purge-all-topics"},
                       {"content-type", "application/json"}},
                      [this](bool success, QByteArray response, int status, QString error) {
                          if (success) {
                              emit ReloadMessagesSignal();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SNSService::GetTopicDetails(QString topicArn){
    QJsonObject jRequest;
    jRequest["topicArn"] = topicArn;
    QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {{"x-awsmock-target", "sns"},
                       {"x-awsmock-action", "get-topic-details"},
                       {"content-type", "application/json"}},
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

void SNSService::DeleteTopic(const QString &topicArn){
    QJsonObject jRequest;
    jRequest["topicArn"] = topicArn;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {{"x-awsmock-target", "sns"},
                       {"x-awsmock-action", "delete-topic"},
                       {"content-type", "application/json"}},
                      [this](const bool success, QByteArray, int, const QString &error) {
                          if (success) {
                              emit ReloadMessagesSignal();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SNSService::GetSnsMessageDetails(QString messageId){
    QJsonObject jRequest;
    jRequest["messageId"] = messageId;
    QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {{"x-awsmock-target", "sns"},
                       {"x-awsmock-action", "get-message-counters"},
                       {"content-type", "application/json"}},
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
