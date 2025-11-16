
#include <modules/s3/S3Service.h>

S3Service::S3Service() {
    url = QUrl(Configuration::instance().GetBaseUrl());
}

// void S3Service::AddTopic(const QString &region, const QString &topicName) {
//     QJsonObject jRequest;
//     jRequest["region"] = region;
//     jRequest["topicName"] = topicName;
//     const QJsonDocument requestDoc(jRequest);
//
//     _restManager.post(url,
//                       requestDoc.toJson(),
//                       {
//                           {"x-awsmock-target", "s3"},
//                           {"x-awsmock-action", "create-topic"},
//                           {"content-type", "application/json"}
//                       },
//                       [this](const bool success, const QByteArray &response, int status, const QString &error) {
//                           if (success) {
//                               emit ReloadMessagesSignal();
//                           } else {
//                               QMessageBox::critical(nullptr, "Error", error);
//                           }
//                       });
// }

void S3Service::ListBuckets(const QString &prefix) {
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
                      {
                          {"x-awsmock-target", "s3"},
                          {"x-awsmock-action", "ListBucketCounters"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              // The API returns an array od objects
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  S3ListBucketResult s3Response;
                                  s3Response.FromJson(jsonDoc);
                                  emit ListBucketSignal(s3Response);
                              } else {
                                  QMessageBox::critical(nullptr, "Error", "Response is not an object!");
                              }
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void S3Service::PurgeBucket(const QString &bucketName) {

    QJsonObject jRequest;
    jRequest["region"] = Configuration::instance().GetRegion();
    jRequest["bucketName"] = bucketName;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "s3"},
                          {"x-awsmock-action", "PurgeBucket"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, QByteArray, int, const QString &error) {
                          if (success) {
                              emit ReloadBucketListSignal();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void S3Service::AddBucket(const QString &bucketName) {

    QJsonObject jRequest;
    jRequest["Name"] = bucketName;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "s3"},
                          {"x-awsmock-action", "AddBucketCounter"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, QByteArray, int, const QString &error) {
                          if (success) {
                              emit ReloadBucketListSignal();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void S3Service::DeleteBucket(const QString &bucketName) {

    QJsonObject jRequest;
    jRequest["Bucket"] = bucketName;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "s3"},
                          {"x-awsmock-action", "DeleteBucketCounter"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, QByteArray, int, const QString &error) {
                          if (success) {
                              emit ReloadBucketListSignal();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void S3Service::GetBucketDetails(const QString &bucketName) {
    QJsonObject jRequest;
    jRequest["region"] = Configuration::instance().GetRegion();
    jRequest["bucketName"] = bucketName;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "s3"},
                          {"x-awsmock-action", "GetBucket"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {

                              // The API returns an JSON document
                              const QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                              S3GetBucketDetailsResponse bucketResponse;
                              bucketResponse.FromJson(jsonDoc);
                              emit GetBucketDetailsSignal(bucketResponse);
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

// void S3Service::ListMessages(const QString &topicArn, const QString &prefix) {
//     QJsonObject jSorting;
//     jSorting["sortDirection"] = -1;
//     jSorting["column"] = "created";
//
//     QJsonArray jSortingArray;
//     jSortingArray.append(jSorting);
//
//     QJsonObject jRequest;
//     jRequest["topicArn"] = topicArn;
//     jRequest["prefix"] = prefix;
//     jRequest["pageSize"] = -1;
//     jRequest["pageIndex"] = -1;
//     jRequest["sortColumns"] = jSortingArray;
//     const QJsonDocument requestDoc(jRequest);
//
//     _restManager.post(url,
//                       requestDoc.toJson(),
//                       {
//                           {"x-awsmock-target", "s3"},
//                           {"x-awsmock-action", "list-message-counters"},
//                           {"content-type", "application/json"}
//                       },
//                       [this](const bool success, const QByteArray &response, int, const QString &error) {
//                           if (success) {
//                               // The API returns an array of objects
//                               if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
//                                   S3ListMessagesResult s3Response;
//                                   s3Response.FromJson(jsonDoc);
//                                   emit ListMessagesSignal(s3Response);
//                               } else {
//                                   //m_quoteLabel->setText("Error: Failed to parse API response.");
//                               }
//                           } else {
//                               QMessageBox::critical(nullptr, "Error", error);
//                           }
//                       });
// }
//
// void S3Service::PurgeTopic(const QString &topicArn) {
//     QJsonObject jRequest;
//     jRequest["topicArn"] = topicArn;
//     const QJsonDocument requestDoc(jRequest);
//
//     _restManager.post(url,
//                       requestDoc.toJson(),
//                       {
//                           {"x-awsmock-target", "s3"},
//                           {"x-awsmock-action", "list-message-counters"},
//                           {"content-type", "application/json"}
//                       },
//                       [this](bool success, QByteArray response, int status, QString error) {
//                           if (success) {
//                               emit ReloadMessagesSignal();
//                           } else {
//                               QMessageBox::critical(nullptr, "Error", error);
//                           }
//                       });
// }
//
// void S3Service::PurgeAllTopics() {
//     _restManager.post(url,
//                       nullptr,
//                       {
//                           {"x-awsmock-target", "s3"},
//                           {"x-awsmock-action", "purge-all-topics"},
//                           {"content-type", "application/json"}
//                       },
//                       [this](bool success, QByteArray response, int status, QString error) {
//                           if (success) {
//                               emit ReloadMessagesSignal();
//                           } else {
//                               QMessageBox::critical(nullptr, "Error", error);
//                           }
//                       });
// }
//
// void S3Service::GetTopicDetails(const QString &topicArn) {
//     QJsonObject jRequest;
//     jRequest["topicArn"] = topicArn;
//     QJsonDocument requestDoc(jRequest);
//
//     _restManager.post(url,
//                       requestDoc.toJson(),
//                       {
//                           {"x-awsmock-target", "s3"},
//                           {"x-awsmock-action", "get-topic-details"},
//                           {"content-type", "application/json"}
//                       },
//                       [this](bool success, QByteArray response, int status, QString error) {
//                           if (success) {
//                               // The API returns an JSON document
//                               QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
//                               S3GetTopicDetailsResponse response;
//                               response.FromJson(jsonDoc);
//
//                               emit GetTopicDetailsSignal(response);
//                           } else {
//                               QMessageBox::critical(nullptr, "Error", error);
//                           }
//                       });
// }
//
// void S3Service::DeleteTopic(const QString &topicArn) {
//     QJsonObject jRequest;
//     jRequest["topicArn"] = topicArn;
//     const QJsonDocument requestDoc(jRequest);
//
//     _restManager.post(url,
//                       requestDoc.toJson(),
//                       {
//                           {"x-awsmock-target", "s3"},
//                           {"x-awsmock-action", "delete-topic"},
//                           {"content-type", "application/json"}
//                       },
//                       [this](const bool success, QByteArray, int, const QString &error) {
//                           if (success) {
//                               emit ReloadMessagesSignal();
//                           } else {
//                               QMessageBox::critical(nullptr, "Error", error);
//                           }
//                       });
// }
//
// void S3Service::GetS3MessageDetails(const QString &messageId) {
//     QJsonObject jRequest;
//     jRequest["messageId"] = messageId;
//     QJsonDocument requestDoc(jRequest);
//
//     _restManager.post(url,
//                       requestDoc.toJson(),
//                       {
//                           {"x-awsmock-target", "s3"},
//                           {"x-awsmock-action", "get-message-counters"},
//                           {"content-type", "application/json"}
//                       },
//                       [this](const bool success, const QByteArray &response, int, const QString &error) {
//                           if (success) {
//                               // The API returns an array containing one object
//                               const QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
//                               S3GetMessageDetailsResponse s3Response;
//                               s3Response.FromJson(jsonDoc["message"].toObject());
//                               emit GetMessageDetailsSignal(s3Response);
//                           } else {
//                               QMessageBox::critical(nullptr, "Error", error);
//                           }
//                       });
// }
//
// void S3Service::DeleteMessage(const QString &topicArn, const QString &messageId) {
//
//     QJsonObject jRequest;
//     jRequest["topicArn"] = topicArn;
//     jRequest["messageId"] = messageId;
//     const QJsonDocument requestDoc(jRequest);
//
//     _restManager.post(url,
//                       requestDoc.toJson(),
//                       {
//                           {"x-awsmock-target", "s3"},
//                           {"x-awsmock-action", "delete-message"},
//                           {"content-type", "application/json"}
//                       },
//                       [this](const bool success, QByteArray, int, const QString &error) {
//                           if (success) {
//                               emit ReloadMessagesSignal();
//                           } else {
//                               QMessageBox::critical(nullptr, "Error", error);
//                           }
//                       });
// }
