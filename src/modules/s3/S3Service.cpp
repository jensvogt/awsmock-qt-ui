#include <modules/s3/S3Service.h>

void S3Service::ListBuckets(const QString &prefix, const long pageSize, const long pageIndex, const QString &sortAttribute, const int sortDirection) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jSorting;
    jSorting["sortDirection"] = sortDirection;
    jSorting["column"] = sortAttribute;

    QJsonArray jSortingArray;
    jSortingArray.append(jSorting);

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["prefix"] = prefix;
    jRequest["pageSize"] = static_cast<qint64>(pageSize);
    jRequest["pageIndex"] = static_cast<qint64>(pageIndex);
    jRequest["sortColumns"] = jSortingArray;

    _restManager.post(GetBaseUrl(),
                      QJsonDocument(jRequest).toJson(),
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
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("ListBuckets", timer.elapsed());
                      });
}

void S3Service::PurgeBucket(const QString &bucketName) {
    QElapsedTimer timer;
    timer.start();
    QJsonObject jRequest = CreateBaseRequest();
    jRequest["bucketName"] = bucketName;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
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
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("PurgeBucket", timer.elapsed());
                      });
}

void S3Service::PurgeAllBuckets() {
    QElapsedTimer timer;
    timer.start();

    _restManager.post(GetBaseUrl(),
                      nullptr,
                      {
                          {"x-awsmock-target", "s3"},
                          {"x-awsmock-action", "PurgeBucket"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, QByteArray, int, const QString &error) {
                          if (success) {
                              emit ReloadBucketListSignal();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("PurgeAllBuckets", timer.elapsed());
                      });
}

void S3Service::AddBucket(const QString &bucketName) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["Name"] = bucketName;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
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
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("AddBucket", timer.elapsed());
                      });
}

void S3Service::UpdateBucket(const QString &bucketName, QMap<QString, QString> &metadata, QVector<LifecycleRule> &lifecycleRules, const QString &versionStatus) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jMetadata;
    for (const auto &k: metadata.keys()) {
        jMetadata[k] = metadata[k];
    }

    QJsonArray jLifecycleRules;
    for (auto &lifecycleRule: lifecycleRules) {
        jLifecycleRules.append(lifecycleRule.ToJson());
    }

    QJsonObject jBucket = CreateBaseRequest();
    jBucket["Region"] = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");
    jBucket["BucketName"] = bucketName;
    jBucket["DefaultMetadata"] = jMetadata;
    jBucket["VersionStatus"] = versionStatus;
    jBucket["LifecycleRules"] = jLifecycleRules;

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["Bucket"] = jBucket;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
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
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("UpdateBucket", timer.elapsed());
                      });
}

void S3Service::ResetObjectCounters() {
    QElapsedTimer timer;
    timer.start();

    _restManager.post(GetBaseUrl(),
                      nullptr,
                      {
                          {"x-awsmock-target", "s3"},
                          {"x-awsmock-action", "ReloadAllCounters"},
                          {"content-type", "application/json"}
                      },
                      [timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (!success) {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("ResetObjectCounters", timer.elapsed());
                      });
}

void S3Service::DeleteBucket(const QString &bucketName) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest;
    jRequest["Bucket"] = bucketName;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
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
                              logError << error;
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

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "s3"},
                          {"x-awsmock-action", "GetBucket"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              const QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                              //std::cerr << JsonUtils::WriteJsonToString(jsonDoc.object()).toStdString() << std::endl;
                              S3GetBucketDetailsResponse bucketResponse;
                              bucketResponse.FromJson(jsonDoc);
                              emit GetBucketDetailsSignal(bucketResponse);
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("GetBucketDetails", timer.elapsed());
                      });
}

void S3Service::ListObjects(const QString &bucketName, const QString &prefix, const long pageSize, const long pageIndex, const QString &sortAttribute, const int sortDirection) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jSorting;
    jSorting["sortDirection"] = sortDirection;
    jSorting["column"] = sortAttribute;

    QJsonArray jSortingArray;
    jSortingArray.append(jSorting);

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["bucket"] = bucketName;
    jRequest["prefix"] = prefix;
    jRequest["pageSize"] = static_cast<qint64>(pageSize);
    jRequest["pageIndex"] = static_cast<qint64>(pageIndex);
    jRequest["sortColumns"] = jSortingArray;

    _restManager.post(GetBaseUrl(),
                      QJsonDocument(jRequest).toJson(),
                      {
                          {"x-awsmock-target", "s3"},
                          {"x-awsmock-action", "ListObjectCounters"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  S3ListObjectsResponse s3Response;
                                  s3Response.FromJson(jsonDoc);
                                  emit ListObjectsSignal(s3Response);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
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

    _restManager.post(GetBaseUrl(),
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
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("GetObjectDetails", timer.elapsed());
                      });
}

void S3Service::UploadObject(const QString &bucketName, const QString &bucketArn, const QString &key, const QByteArray &content, const QString &contentType, const QMap<QString, QString> &metadata) {
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
    jRequest["contentType"] = contentType;
    jRequest["metadata"] = jMetadata;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
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
                              logError << error;
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

    _restManager.post(GetBaseUrl(),
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
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("UploadObject", timer.elapsed());
                      });
}

void S3Service::PutBucketNotificationConfiguration(const S3PutBucketNotificationConfigurationRequest &request) {
    QElapsedTimer timer;
    timer.start();

    _restManager.put(GetBaseUrl(request.bucket),
                     request.ToXml().toUtf8(),
                     {
                         {"x-awsmock-target", "s3"},
                         {"x-awsmock-action", "PutBucketNotificationConfiguration"},
                         {"x-awsmock-region", Configuration::instance().GetValue<QString>("aws.region", "eu-central-1")},
                         {"content-type", "application/xml; charset=utf-8"}
                     },
                     [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                         if (success) {
                             emit ReloadObjectsSignal();
                         } else {
                             logError << error;
                         }
                         emit EventBus::instance().TimerSignal("PutBucketNotificationConfiguration", timer.elapsed());
                     });
}

void S3Service::PutBucketNotificationConfiguration(const QString &bucket,
                                                   const QList<LambdaNotification> &lambdaNotifications,
                                                   const QList<QueueNotification> &queueNotifications,
                                                   const QList<TopicNotification> &topicNotifications) {
    S3PutBucketNotificationConfigurationRequest request;
    request.region = bucket;
    request.bucket = bucket;

    for (const auto &[id, lambdaArn, filterRules, events]: lambdaNotifications) {
        S3LambdaConfiguration config;
        config.id = id;
        config.lambdaArn = lambdaArn;
        config.filterRules = QVector<S3FilterRule>(filterRules.cbegin(), filterRules.cend());
        config.events = QVector<S3NotificationEventType>(events.cbegin(), events.cend());
        request.lambdaConfigurations.append(config);
    }

    for (const auto &[id, queueArn, filterRules, events]: queueNotifications) {
        S3QueueConfiguration config;
        config.id = id;
        config.queueArn = queueArn;
        config.filterRules = QVector<S3FilterRule>(filterRules.cbegin(), filterRules.cend());
        config.events = QVector<S3NotificationEventType>(events.cbegin(), events.cend());
        request.queueConfigurations.append(config);
    }

    for (const auto &[id, topicArn, filterRules, events]: topicNotifications) {
        S3TopicConfiguration config;
        config.id = id;
        config.topicArn = topicArn;
        config.filterRules = QVector<S3FilterRule>(filterRules.cbegin(), filterRules.cend());
        config.events = QVector<S3NotificationEventType>(events.cbegin(), events.cend());
        request.topicConfigurations.append(config);
    }

    PutBucketNotificationConfiguration(request);
}

void S3Service::TouchObject(const QString &bucketName, const QString &key) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["bucket"] = bucketName;
    jRequest["key"] = key;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "s3"},
                          {"x-awsmock-action", "TouchObject"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadObjectsSignal();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("DeleteObject", timer.elapsed());
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

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "s3"},
                          {"x-awsmock-action", "DeleteObjectCounter"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &, int, const QString &error) {
                          if (success) {
                              emit ReloadObjectsSignal();
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("DeleteObject", timer.elapsed());
                      });
}
