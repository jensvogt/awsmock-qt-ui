#ifndef AWSMOCK_QT_UI_S3_GET_BUCKET_DETAILS_RESPONSE_H
#define AWSMOCK_QT_UI_S3_GET_BUCKET_DETAILS_RESPONSE_H

// Qt includes
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

// Awsmock includes
#include <dto/s3/S3FilterRule.h>
#include <dto/s3/S3NotificationEventType.h>

enum class StorageClass {
    STANDARD,
    STANDARD_IA,
    ONEZONE_IA,
    EXPRESS_ONEZONE,
    GLACIER,
    GLACIER_IR,
    DEEP_ARCHIVE,
    INTELLIGENT_TIERING,
    REDUCED_REDUNDANCY,
    UNKNOWN,
};


static std::map<StorageClass, QString> StorageClassNames{
    {StorageClass::STANDARD, "STANDARD"},
    {StorageClass::STANDARD_IA, "STANDARD_IA"},
    {StorageClass::ONEZONE_IA, "ONEZONE_IA"},
    {StorageClass::EXPRESS_ONEZONE, "EXPRESS_ONEZONE"},
    {StorageClass::GLACIER, "GLACIER"},
    {StorageClass::GLACIER_IR, "GLACIER_IR"},
    {StorageClass::DEEP_ARCHIVE, "DEEP_ARCHIVE"},
    {StorageClass::INTELLIGENT_TIERING, "INTELLIGENT_TIERING"},
    {StorageClass::REDUCED_REDUNDANCY, "REDUCED_REDUNDANCY"},
    {StorageClass::UNKNOWN, "UNKNOWN"}
};

[[maybe_unused]] static QString StorageClassToString(const StorageClass storageClass) {
    return StorageClassNames[storageClass];
}

[[maybe_unused]] static StorageClass StorageClassFromString(const QString &storageClass) {
    for (auto &[fst, snd]: StorageClassNames) {
        if (snd == storageClass) {
            return fst;
        }
    }
    return StorageClass::STANDARD;
}

struct LambdaNotification {
    QString id;

    QString lambdaArn;

    QList<S3FilterRule> filterRules;

    QList<S3NotificationEventType> events;

    void FromJson(const QJsonObject &jsonObject) {
        id = jsonObject["id"].toString();
        lambdaArn = jsonObject["lambdaArn"].toString();
        for (const auto &filterRule: jsonObject["filterRules"].toArray()) {
            S3FilterRule rule;
            rule.FromJson(filterRule.toObject());
        }
        for (const auto &event: jsonObject["events"].toArray()) {
            events.append(S3NotificationEventFromString(event.toString()));
        }
    }
};

struct QueueNotification {
    QString id;

    QString queueArn;

    QList<S3FilterRule> filterRules;

    QList<S3NotificationEventType> events;

    void FromJson(const QJsonObject &jsonObject) {
        id = jsonObject["id"].toString();
        queueArn = jsonObject["queueArn"].toString();
        for (const auto &filterRule: jsonObject["filterRules"].toArray()) {
            S3FilterRule rule;
            rule.FromJson(filterRule.toObject());
            filterRules.append(rule);
        }
        for (const auto &event: jsonObject["events"].toArray()) {
            events.append(S3NotificationEventFromString(event.toString()));
        }
    }
};

struct TopicNotification {
    QString id;

    QString topicArn;

    QList<S3FilterRule> filterRules;

    QList<S3NotificationEventType> events;

    void FromJson(const QJsonObject &jsonObject) {
        id = jsonObject["id"].toString();
        topicArn = jsonObject["_topicArn"].toString();
        for (const auto &filterRule: jsonObject["filterRules"].toArray()) {
            S3FilterRule rule;
            rule.FromJson(filterRule.toObject());
        }
        for (const auto &event: jsonObject["events"].toArray()) {
            events.append(S3NotificationEventFromString(event.toString()));
        }
    }
};

struct LifecycleTransition {

    QDateTime date;

    int days;

    StorageClass storageClass;

    void FromJson(const QJsonObject &jsonObject) {
        date = QDateTime::fromString(jsonObject["Date"].toString(), Qt::ISODate);
        days = jsonObject["Days"].toInt();
        storageClass = StorageClassFromString(jsonObject["StorageClass"].toString());
    }

    QJsonObject ToJson() const {
        QJsonObject jsonObject;
        jsonObject["Date"] = date.toString(Qt::ISODate);
        jsonObject["Days"] = days;
        jsonObject["StoreClass"] = StorageClassToString(storageClass);
        return jsonObject;
    }
};

struct LifecycleRule {

    QString id = QUuid::createUuid().toString(QUuid::WithoutBraces);

    QString status;

    QString prefix;

    QVector<LifecycleTransition> transitions;

    void FromJson(const QJsonObject &jsonObject) {
        id = jsonObject["ID"].toString();
        status = jsonObject["Status"].toString();
        prefix = jsonObject["Prefix"].toString();
        if (jsonObject.contains("Transitions") && jsonObject["Transitions"].isArray()) {
            for (const auto &transitionElement: jsonObject["Transitions"].toArray()) {
                LifecycleTransition transition;
                transition.FromJson(transitionElement.toObject());
                transitions.append(transition);
            }
        }
    }

    QJsonObject ToJson() {
        QJsonObject jsonObject;
        jsonObject["ID"] = id;
        jsonObject["Status"] = status;
        jsonObject["Prefix"] = prefix;
        if (!transitions.isEmpty()) {
            QJsonArray jsonArray;
            for (const auto &transition: transitions) {
                jsonArray.append(transition.ToJson());
            }
            jsonObject["Transitions"] = jsonArray;
        }
        return jsonObject;
    }
};

struct S3GetBucketDetailsResponse {

    QString region;

    QString bucketName;

    QString bucketArn;

    QString versioningStatus;

    long objectCount;

    long size;

    QString owner;

    bool versioned;

    QMap<QString, QString> defaultMetadata;

    QList<LambdaNotification> lambdaNotifications;

    QList<QueueNotification> queueNotifications;

    QList<TopicNotification> topicNotifications;

    QList<LifecycleRule> lifecycleRules;

    QDateTime created;

    QDateTime modified;

    void FromJson(const QJsonDocument &jsonDoc) {
        region = jsonDoc["region"].toString();
        bucketName = jsonDoc["bucket"].toString();
        bucketArn = jsonDoc["arn"].toString();
        objectCount = jsonDoc["keys"].toInteger();
        size = jsonDoc["size"].toInteger();
        owner = jsonDoc["owner"].toString();
        versioningStatus = jsonDoc["versionStatus"].toString();
        versioned = jsonDoc["versionStatus"].toString() == "enabled";
        created = QDateTime::fromString(jsonDoc["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonDoc["modified"].toString(), Qt::ISODate);

        if (!jsonDoc["lambdaConfigurations"].isNull() && jsonDoc["lambdaConfigurations"].isArray()) {
            for (const auto &element: jsonDoc["lambdaConfigurations"].toArray()) {
                LambdaNotification lambdaConfig;
                lambdaConfig.FromJson(element.toObject());
                lambdaNotifications.push_back(lambdaConfig);
            }
        }

        if (!jsonDoc["queueConfigurations"].isNull() && jsonDoc["queueConfigurations"].isArray()) {
            for (const auto &element: jsonDoc["queueConfigurations"].toArray()) {
                QueueNotification queueConfig;
                queueConfig.FromJson(element.toObject());
                queueNotifications.push_back(queueConfig);
            }
        }

        if (!jsonDoc["topicConfigurations"].isNull() && jsonDoc["topicConfigurations"].isArray()) {
            for (const auto &element: jsonDoc["topicConfigurations"].toArray()) {
                TopicNotification topicConfig;
                topicConfig.FromJson(element.toObject());
                topicNotifications.push_back(topicConfig);
            }
        }

        if (!jsonDoc["defaultMetadata"].isNull() && jsonDoc["defaultMetadata"].isObject()) {
            for (const auto &key: jsonDoc["defaultMetadata"].toObject().keys()) {
                defaultMetadata[key] = jsonDoc["defaultMetadata"].toObject()[key].toString();
            }
        }

        if (!jsonDoc["lifecycleRules"].isNull() && jsonDoc["lifecycleRules"].isArray()) {
            for (const auto &element: jsonDoc["lifecycleRules"].toArray()) {
                LifecycleRule lifecycleRule;
                lifecycleRule.FromJson(element.toObject());
                lifecycleRules.push_back(lifecycleRule);
            }
        }
    }
};

#endif // AWSMOCK_QT_UI_S3_GET_BUCKET_DETAILS_RESPONSE_H
