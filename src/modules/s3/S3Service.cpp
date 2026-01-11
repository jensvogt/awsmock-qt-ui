
#include <modules/s3/S3Service.h>

S3Service::S3Service() {
    _url = QUrl(Configuration::instance().GetValue<QString>("server.base-url", "http://localhost:4566"));
}

void S3Service::ListBuckets(const QString &prefix) {
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

    _restManager.post(_url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "s3"},
                          {"x-awsmock-action", "ListBucketCounters"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              // The API returns an array od objects
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  S3ListBucketResult s3Response;
                                  s3Response.FromJson(jsonDoc);
                                  emit ListBucketSignal(s3Response);
                              } else {
                                  qCritical() << "Response is not an object!";
                              }
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().TimerSignal("AddTopic", timer.elapsed());
                      });
}

void S3Service::PurgeBucket(const QString &bucketName) {
    QElapsedTimer timer;
    timer.start();
    QJsonObject jRequest = CreateBaseRequest();
    jRequest["bucketName"] = bucketName;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(_url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "s3"},
                          {"x-awsmock-action", "PurgeBucket"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, QByteArray, int, const QString &error) {
                          if (success) {
                              emit ReloadBucketListSignal();
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().TimerSignal("PurgeBucket", timer.elapsed());
                      });
}

void S3Service::AddBucket(const QString &bucketName) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["Name"] = bucketName;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(_url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "s3"},
                          {"x-awsmock-action", "AddBucketCounter"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, QByteArray, int, const QString &error) {
                          if (success) {
                              emit ReloadBucketListSignal();
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().TimerSignal("AddBucket", timer.elapsed());
                      });
}

void S3Service::UpdateBucket(const QString &bucketName, QMap<QString, QString> &metadata, const QString &versionStatus) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jMetadata;
    for (const auto &k: metadata.keys()) {
        jMetadata[k] = metadata[k];
    }

    QJsonObject jBucket = CreateBaseRequest();
    jBucket["Region"] = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");
    jBucket["BucketName"] = bucketName;
    jBucket["DefaultMetadata"] = jMetadata;
    jBucket["VersionStatus"] = versionStatus;

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["Bucket"] = jBucket;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(_url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "s3"},
                          {"x-awsmock-action", "UpdateBucket"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadBucketListSignal();
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().TimerSignal("UpdateBucket", timer.elapsed());
                      });
}


void S3Service::DeleteBucket(const QString &bucketName) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest;
    jRequest["Bucket"] = bucketName;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(_url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "s3"},
                          {"x-awsmock-action", "DeleteBucketCounter"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, QByteArray, int, const QString &error) {
                          if (success) {
                              emit ReloadBucketListSignal();
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().TimerSignal("DeleteBucket", timer.elapsed());
                      });
}

void S3Service::GetBucketDetails(const QString &bucketName) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["bucketName"] = bucketName;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(_url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "s3"},
                          {"x-awsmock-action", "GetBucket"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              const QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                              S3GetBucketDetailsResponse bucketResponse;
                              bucketResponse.FromJson(jsonDoc);
                              emit GetBucketDetailsSignal(bucketResponse);
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().TimerSignal("GetBucketDetails", timer.elapsed());
                      });
}

void S3Service::ListObjects(const QString &bucketName, const QString &prefix) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jSorting;
    jSorting["sortDirection"] = -1;
    jSorting["column"] = "messages";

    QJsonArray jSortingArray;
    jSortingArray.append(jSorting);

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["bucket"] = bucketName;
    jRequest["prefix"] = prefix;
    jRequest["pageSize"] = -1;
    jRequest["pageIndex"] = -1;
    jRequest["sortColumns"] = jSortingArray;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(_url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "s3"},
                          {"x-awsmock-action", "ListObjectCounters"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              // The API returns an array od objects
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  S3ListObjectsResponse s3Response;
                                  s3Response.FromJson(jsonDoc);
                                  emit ListObjectsSignal(s3Response);
                              } else {
                                  qCritical() << "Response is not an object!";
                              }
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().TimerSignal("ListObjects", timer.elapsed());
                      });
}

void S3Service::GetObjectDetails(const QString &objectId) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["oid"] = objectId;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(_url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "s3"},
                          {"x-awsmock-action", "GetObjectCounter"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              // The API returns an JSON document
                              const QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                              S3GetObjectDetailsResponse objectDetailsResponse;
                              objectDetailsResponse.FromJson(jsonDoc["objectCounter"].toObject());
                              emit GetObjectDetailsSignal(objectDetailsResponse);
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().TimerSignal("GetObjectDetails", timer.elapsed());
                      });
}

void S3Service::UploadObject(const QString &bucketName, const QString &bucketArn, const QString &key, const QByteArray &content, const QMap<QString, QString> &metadata) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jMetadata;
    for (const auto &k: metadata.keys()) {
        jMetadata[k] = metadata[k];
    }

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["bucketArn"] = bucketArn;
    jRequest["bucketName"] = bucketName;
    jRequest["objectKey"] = key;
    jRequest["content"] = QString(content.toBase64());
    jRequest["contentType"] = "application/json";
    jRequest["metadata"] = jMetadata;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(_url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "s3"},
                          {"x-awsmock-action", "UploadObjectCounter"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadObjectsSignal();
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().TimerSignal("UploadObject", timer.elapsed());
                      });
}

void S3Service::UpdateObject(const QString &region, const QString &bucketName, const QString &key, const QByteArray &content, const QString &storageClass, const QMap<QString, QString> &metadata) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jMetadata;
    for (const auto &k: metadata.keys()) {
        jMetadata[k] = metadata[k];
    }

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["region"] = region;
    jRequest["bucket"] = bucketName;
    jRequest["key"] = key;
    jRequest["content"] = QString(content.toBase64());
    jRequest["storageClass"] = storageClass;
    jRequest["contentType"] = "application/json";
    jRequest["metadata"] = jMetadata;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(_url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "s3"},
                          {"x-awsmock-action", "UpdateObject"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadObjectsSignal();
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().TimerSignal("UploadObject", timer.elapsed());
                      });
}


void S3Service::DeleteObject(const QString &bucketName, const QString &key) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["Bucket"] = bucketName;
    jRequest["Key"] = key;
    jRequest["VersionId"] = "";
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(_url,
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "s3"},
                          {"x-awsmock-action", "DeleteObjectCounter"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              emit ReloadObjectsSignal();
                          } else {
                              qCritical() << error;
                          }
                          emit EventBus::instance().TimerSignal("DeleteObject", timer.elapsed());
                      });
}

