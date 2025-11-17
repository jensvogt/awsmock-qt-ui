
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

void S3Service::ListObjects(const QString &bucketName, const QString &prefix) {

    QJsonObject jSorting;
    jSorting["sortDirection"] = -1;
    jSorting["column"] = "messages";

    QJsonArray jSortingArray;
    jSortingArray.append(jSorting);

    QJsonObject jRequest;
    jRequest["bucket"] = bucketName;
    jRequest["prefix"] = prefix;
    jRequest["pageSize"] = -1;
    jRequest["pageIndex"] = -1;
    jRequest["sortColumns"] = jSortingArray;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "s3"},
                          {"x-awsmock-action", "ListObjectCounters"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              // The API returns an array od objects
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  S3ListObjectsResponse s3Response;
                                  s3Response.FromJson(jsonDoc);
                                  emit ListObjectsSignal(s3Response);
                              } else {
                                  QMessageBox::critical(nullptr, "Error", "Response is not an object!");
                              }
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void S3Service::DeleteObject(const QString &bucketName, const QString &key) {

    QJsonObject jRequest;
    jRequest["Bucket"] = bucketName;
    jRequest["Key"] = key;
    jRequest["VersionId"] = "";
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "s3"},
                          {"x-awsmock-action", "DeleteObjectCounter"},
                          {"content-type", "application/json"}
                      },
                      [this](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              emit ReloadObjectsSignal();
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}
