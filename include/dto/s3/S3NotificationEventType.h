//
// Created by vogje01 on 2/21/26.
//

#ifndef AWSMOCK_QT_UI_S3_NOTIFICATION_EVENT_TYPE_H
#define AWSMOCK_QT_UI_S3_NOTIFICATION_EVENT_TYPE_H

// Qt includes
#include <QMap>
#include <QString>

/**
 * @brief S3 notification events
 *
 * @author jens.vogt\@opitz-consulting.com
 */
enum class S3NotificationEventType {
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

static QMap<S3NotificationEventType, QString> EventTypeNames{
    {S3NotificationEventType::REDUCED_REDUNDANCY_LOST_OBJECT, "ReducedRedundancyLostObject "},
    {S3NotificationEventType::OBJECT_CREATED, "ObjectCreated"},
    {S3NotificationEventType::OBJECT_REMOVED, "ObjectRemoved"},
    {S3NotificationEventType::OBJECT_RESTORED, "ObjectRestore"},
    {S3NotificationEventType::REPLICATION, "Replication"},
    {S3NotificationEventType::OBJECT_RESTORE, "ObjectRestore"},
    {S3NotificationEventType::OBJECT_LIFECYCLE_TRANSITION, "LifecycleTransition "},
    {S3NotificationEventType::INTELLIGENT_TIERING, "IntelligentTiering "},
    {S3NotificationEventType::OBJECT_ACL, "ObjectAcl"},
    {S3NotificationEventType::LIFECYCLE_EXPIRATION, "LifecycleExpiration"},
    {S3NotificationEventType::OBJECT_TAGGING, "ObjectTagging"},
};

[[maybe_unused]] static QString S3NotificationEventToString(const S3NotificationEventType &eventType) {
    return EventTypeNames[eventType];
}

[[maybe_unused]] static S3NotificationEventType S3NotificationEventFromString(const QString &eventType) {
    return EventTypeNames.key(eventType, S3NotificationEventType::OBJECT_CREATED);
}

[[maybe_unused]] static QStringList S3EventTypeNameList() {
    QStringList eventTypeNames;
    for (auto &name: EventTypeNames.values()) {
        eventTypeNames.append(name);
    }
    return eventTypeNames;
}

#endif // AWSMOCK_QT_UI_S3_NOTIFICATION_EVENT_TYPE_H
