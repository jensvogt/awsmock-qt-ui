//
// Created by vogje01 on 11/12/25.
//

#ifndef AWSMOCK_QT_UI_S3_BUCKET_EDIT_DIALOG_H
#define AWSMOCK_QT_UI_S3_BUCKET_EDIT_DIALOG_H

// Qt includes
#include <QDialog>
#include <QMenu>
#include <QStandardItemModel>
#include <QTableView>
#include <QDialogButtonBox>

// AwsMock includes
#include <modules/s3/S3BucketLambdaNotificationDialog.h>
#include <modules/s3/S3BucketLifecycleDialog.h>
#include <modules/s3/S3BucketMetadataDialog.h>
#include <modules/s3/S3ObjectMetadataDialog.h>
#include <modules/s3/S3QueueNotificationDialog.h>
#include <modules/s3/S3Service.h>
#include <utils/BaseDialog.h>
#include <utils/DateTimeUtils.h>
#include <utils/IconUtils.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class S3BucketEditDialog;
}

QT_END_NAMESPACE

class S3BucketEditDialog final : public BaseDialog {
    Q_OBJECT

public:
    explicit S3BucketEditDialog(const QString &bucketName, QWidget *parent = nullptr);

    ~S3BucketEditDialog() override;

    void UpdateBucket(const S3GetBucketDetailsResponse &bucketGetResponse);

    void SetupDefaultMetadataTab();

    void ShowDefaultMetadataContextMenu(const QPoint &pos);

    void SetupLambdaNotifications();

    void SetupQueueNotifications();

    void SetupTopicNotifications();

    void SetupLifecycles();

    void ShowLifecycleContextMenu(const QPoint &pos);

    void ShowQueueNotificationContextMenu(const QPoint &pos);

    void LoadContent() override {
    };

private slots:
    void HandleAccept();

    void HandleReject();

private:
    /**
     * @brief UI components
     */
    Ui::S3BucketEditDialog *_ui;

    /**
     * @REST service handler
     */
    S3Service *_s3Service;

    /**
     * @brief S3 bucket
     */
    QString _bucket;

    /**
     * @brief Changed flag
     */
    bool _changed = false;

    /**
     * @brief Get REST response
     */
    S3GetBucketDetailsResponse _bucketGetResponse;

    /**
     * @brief Default metadata table data model
     */
    QStandardItemModel *_defaultMetadataDataModel{};

    /**
     * @brief Lambda notification table model
     */
    QStandardItemModel *_lambdaNotificationDataModel{};

    /**
     * @brief Queue notification table model
     */
    QStandardItemModel *_queueNotificationDataModel{};

    /**
     * @brief Topic notification table model
     */
    QStandardItemModel *_topicNotificationDataModel{};

    /**
     * @brief Lifecycle rules
     */
    QStandardItemModel *_lifecycleDataModel{};
};


#endif //AWSMOCK_QT_UI_S3_BUCKET_EDIT_DIALOG_H
