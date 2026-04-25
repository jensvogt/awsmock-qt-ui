//
// Created by jensv on 25/04/2026.
//

#ifndef AWSMOCK_QT_UI_S3_QUEUE_NOTIFICATION_DIALOG_H
#define AWSMOCK_QT_UI_S3_QUEUE_NOTIFICATION_DIALOG_H

// Qt includes
#include <qcompleter.h>
#include <QDialog>
#include <QDialogButtonBox>

// Awsmock includes
#include <modules/sqs/SQSService.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class S3QueueNotificationDialog;
}

QT_END_NAMESPACE

class S3QueueNotificationDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor for a new notification
     *
     * @param parent parent widget
     */
    explicit S3QueueNotificationDialog(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~S3QueueNotificationDialog() override;

    /**
     * @brief Returns the selected queue ARN
     *
     * @return selected SQS queue ARN
     */
    QString GetQueueArn() {
        return _selectedQueueArn;
    }

    /**
     * @brief Returns the selected queue ARN
     *
     * @return selected S3 event
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
    Ui::S3QueueNotificationDialog *_ui;

    /**
     * @brief SQS REST service
     */
    SQSService *_sqsService;

    /**
     * @brief Selected queue ARN
     */
    QString _selectedQueueArn;

    /**
     * @brief Selected S3 events
     */
    QStringList _selectedS3Event;

    /**
     * @brief List of possible S3 events
     */
    static inline const QList<QString> _s3Events = {
        "s3:ObjectCreated:Put",
        "s3:ObjectCreated:Post",
        "s3:ObjectCreated:Copy",
        "s3:ObjectCreated:CompleteMultipartUpload",
        "s3:ObjectCreated:*",
        "s3:ObjectRemoved:Delete",
        "s3:ObjectRemoved:DeleteMarkerCreated",
        "s3:ObjectRemoved:*",
        "s3:ObjectRestore:Post",
        "s3:ObjectRestore:Completed",
        "s3:ObjectRestore:Delete",
        "s3:Replication:OperationFailedReplication",
        "s3:Replication:OperationMissedThreshold",
        "s3:Replication:OperationReplicatedAfterThreshold",
        "s3:Replication:OperationNotTracked",
        "s3:LifecycleTransition",
        "s3:LifecycleExpiration",
        "s3:LifecycleExpiration:Delete",
        "s3:LifecycleExpiration:DeleteMarkerCreated",
        "s3:ObjectAcl:Put",
        "s3:ObjectTagging:Put",
        "s3:ObjectTagging:Delete",
        "s3:ObjectAccessed:*",
        "s3:ReducedRedundancyLostObject"
    };
};


#endif //AWSMOCK_QT_UI_S3_QUEUE_NOTIFICATION_DIALOG_H
