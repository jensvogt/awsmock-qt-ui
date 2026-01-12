#ifndef AWSMOCK_QT_UI_S3_GET_BUCKET_DETAILS_RESPONSE_H
#define AWSMOCK_QT_UI_S3_GET_BUCKET_DETAILS_RESPONSE_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <utils/JsonUtils.h>

enum class NotificationEventType {
    REDUCED_REDUNDANCY_LOST_OBJECT,
    OBJECT_CREATED,
    OBJECT_REMOVED,
    OBJECT_RESTORED,
    REPLICATION,
    OBJECT_RESTORE,
    OBJECT_LIFECYCLE_TRANSITION,
    INTELLIGENT_TIERING,
    OBJECT_ACL,
    LIFECYCLE_EXPIRATION,
    OBJECT_TAGGING
};


static std::map<NotificationEventType, QString> EventTypeNames{
    {NotificationEventType::REDUCED_REDUNDANCY_LOST_OBJECT, "ReducedRedundancyLostObject"},
    {NotificationEventType::OBJECT_CREATED, "ObjectCreated"},
    {NotificationEventType::OBJECT_REMOVED, "ObjectRemoved"},
    {NotificationEventType::OBJECT_RESTORED, "ObjectRestore"},
    {NotificationEventType::REPLICATION, "Replication"},
    {NotificationEventType::OBJECT_RESTORE, "ObjectRestore"},
    {NotificationEventType::OBJECT_LIFECYCLE_TRANSITION, "LifecycleTransition"},
    {NotificationEventType::INTELLIGENT_TIERING, "IntelligentTiering"},
    {NotificationEventType::OBJECT_ACL, "ObjectAcl"},
    {NotificationEventType::LIFECYCLE_EXPIRATION, "LifecycleExpiration"},
    {NotificationEventType::OBJECT_TAGGING, "ObjectTagging"},
};

[[maybe_unused]] static QString EventTypeToString(const NotificationEventType eventType) {
    return EventTypeNames[eventType];
}

[[maybe_unused]] static NotificationEventType EventTypeFromString(const QString &nameType) {
    for (auto &[fst, snd]: EventTypeNames) {
        if (snd == nameType) {
            return fst;
        }
    }
    return NotificationEventType::OBJECT_CREATED;
}

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

struct FilterRule {
    QString name;

    QString filterValue;

    void FromJson(const QJsonObject &jsonObject) {
        name = jsonObject["name"].toString();
        filterValue = jsonObject["filterValue"].toString();
    }
};

struct LambdaNotification {
    QString id;

    QString lambdaArn;

    QList<FilterRule> filterRules;

    QList<NotificationEventType> events;

    void FromJson(const QJsonObject &jsonObject) {
        id = jsonObject["id"].toString();
        lambdaArn = jsonObject["lambdaArn"].toString();
        for (const auto &filterRule: jsonObject["filterRules"].toArray()) {
            FilterRule rule;
            rule.FromJson(filterRule.toObject());
        }
        for (const auto &event: jsonObject["events"].toArray()) {
            events.append(EventTypeFromString(event.toString()));
        }
    }
};

struct QueueNotification {
    QString id;

    QString queueArn;

    QList<FilterRule> filterRules;

    QList<NotificationEventType> events;

    void FromJson(const QJsonObject &jsonObject) {
        id = jsonObject["id"].toString();
        queueArn = jsonObject["queueArn"].toString();
        for (const auto &filterRule: jsonObject["filterRules"].toArray()) {
            FilterRule rule;
            rule.FromJson(filterRule.toObject());
        }
        for (const auto &event: jsonObject["events"].toArray()) {
            events.append(EventTypeFromString(event.toString()));
        }
    }
};

struct TopicNotification {
    QString id;

    QString topicArn;

    QList<FilterRule> filterRules;

    QList<NotificationEventType> events;

    void FromJson(const QJsonObject &jsonObject) {
        id = jsonObject["id"].toString();
        topicArn = jsonObject["topicArn"].toString();
        for (const auto &filterRule: jsonObject["filterRules"].toArray()) {
            FilterRule rule;
            rule.FromJson(filterRule.toObject());
        }
        for (const auto &event: jsonObject["events"].toArray()) {
            events.append(EventTypeFromString(event.toString()));
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
