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


static std::map<NotificationEventType, std::string> EventTypeNames{
    {NotificationEventType::REDUCED_REDUNDANCY_LOST_OBJECT, "ReducedRedundancyLostObject "},
    {NotificationEventType::OBJECT_CREATED, "ObjectCreated"},
    {NotificationEventType::OBJECT_REMOVED, "ObjectRemoved"},
    {NotificationEventType::OBJECT_RESTORED, "ObjectRestore"},
    {NotificationEventType::REPLICATION, "Replication"},
    {NotificationEventType::OBJECT_RESTORE, "ObjectRestore"},
    {NotificationEventType::OBJECT_LIFECYCLE_TRANSITION, "LifecycleTransition "},
    {NotificationEventType::INTELLIGENT_TIERING, "IntelligentTiering "},
    {NotificationEventType::OBJECT_ACL, "ObjectAcl"},
    {NotificationEventType::LIFECYCLE_EXPIRATION, "LifecycleExpiration"},
    {NotificationEventType::OBJECT_TAGGING, "ObjectTagging"},
};

[[maybe_unused]] static std::string EventTypeToString(const NotificationEventType eventType) {
    return EventTypeNames[eventType];
}

[[maybe_unused]] static NotificationEventType EventTypeFromString(const std::string &nameType) {
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

    QDateTime created;

    QDateTime modified;

    void FromJson(const QJsonDocument &jsonDoc) {
        region = jsonDoc["region"].toString();
        bucketName = jsonDoc["bucket"].toString();
        bucketArn = jsonDoc["arn"].toString();
        objectCount = jsonDoc["keys"].toInteger();
        size = jsonDoc["size"].toInteger();
        owner = jsonDoc["delay"].toString();
        created = QDateTime::fromString(jsonDoc["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonDoc["modified"].toString(), Qt::ISODate);

        if (!jsonDoc["lambdaConfigurations"].isNull() && jsonDoc["lambdaConfigurations"].isArray()) {
            for (const auto &element: jsonDoc["lambdaConfigurations"].toArray()) {
                LambdaNotification lambdaConfig;
                lambdaConfig.FromJson(element.toObject());
            }
        }
    }
};

#endif // AWSMOCK_QT_UI_S3_GET_BUCKET_DETAILS_RESPONSE_H
