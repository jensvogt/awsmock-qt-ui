//
// Created by vogje01 on 18/07/26.
//

#ifndef AWSMOCK_QT_UI_S3_TOPIC_NOTIFICATION_DIALOG_H
#define AWSMOCK_QT_UI_S3_TOPIC_NOTIFICATION_DIALOG_H

// Qt includes
#include <qcompleter.h>
#include <QDialog>
#include <QDialogButtonBox>

// Awsmock includes
#include <modules/sns/SNSService.h>
#include <dto/s3/S3GetBucketDetailsResponse.h>
#include <utils/BaseDialog.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class S3TopicNotificationDialog;
}

QT_END_NAMESPACE

class S3TopicNotificationDialog : public BaseDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor for a new notification
     *
     * @param parent parent widget
     */
    explicit S3TopicNotificationDialog(QWidget *parent = nullptr);

    /**
     * @brief Constructor for editing an existing notification.
     *
     * @param notification notification
     * @param parent parent widget
     */
    explicit S3TopicNotificationDialog(const TopicNotification &notification, QWidget *parent = nullptr);

    /**
     * @brief Common initialization
     */
    void Initialize();

    /**
     * @brief Load data content
     */
    void LoadContent() override {
    }

    /**
     * @brief Destructor
     */
    ~S3TopicNotificationDialog() override;

    /**
     * @brief Returns the selected topic ARN
     *
     * @return selected SNS topic ARN
     */
    QString GetTopicArn() {
        return _selectedTopicArn;
    }

    /**
     * @brief Returns the selected S3 events
     *
     * @return selected S3 events
     */
    QStringList GetS3Event() {
        return _selectedS3Event;
    }

    /**
     * @brief Handle OK button
     */
    void HandleAccept();

    /**
     * @brief Handle CANCEL button
     */
    void HandleReject();

private:
    /**
     * @brief UI components
     */
    Ui::S3TopicNotificationDialog *_ui;

    /**
     * @brief SNS REST service
     */
    SNSService *_snsService{};

    /**
     * @brief Selected topic ARN
     */
    QString _selectedTopicArn;

    /**
     * @brief Selected S3 events
     */
    QStringList _selectedS3Event;

    /**
     * @brief Filter data model
     */
    QStandardItemModel *_filterDataModel{};

    /**
     * @brief List of possible S3 events
     */
    static inline const QList<QString> _s3Events = {
        "ObjectCreated:Put",
        "ObjectCreated:Post",
        "ObjectCreated:Copy",
        "ObjectCreated:CompleteMultipartUpload",
        "ObjectCreated:*",
        "ObjectRemoved:Delete",
        "ObjectRemoved:DeleteMarkerCreated",
        "ObjectRemoved:*",
        "ObjectRestore:Post",
        "ObjectRestore:Completed",
        "ObjectRestore:Delete",
        "Replication:OperationFailedReplication",
        "Replication:OperationMissedThreshold",
        "Replication:OperationReplicatedAfterThreshold",
        "Replication:OperationNotTracked",
        "LifecycleTransition",
        "LifecycleExpiration",
        "LifecycleExpiration:Delete",
        "LifecycleExpiration:DeleteMarkerCreated",
        "ObjectAcl:Put",
        "ObjectTagging:Put",
        "ObjectTagging:Delete",
        "ObjectAccessed:*",
        "ReducedRedundancyLostObject"
    };
};


#endif //AWSMOCK_QT_UI_S3_TOPIC_NOTIFICATION_DIALOG_H
