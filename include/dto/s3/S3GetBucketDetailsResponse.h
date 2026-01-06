#ifndef AWSMOCK_QT_UI_S3_GET_BUCKET_DETAILS_RESPONSE_H
#define AWSMOCK_QT_UI_S3_GET_BUCKET_DETAILS_RESPONSE_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

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
    }
};

#endif // AWSMOCK_QT_UI_S3_GET_BUCKET_DETAILS_RESPONSE_H
