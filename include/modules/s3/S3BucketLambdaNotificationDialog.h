//
// Created by vogje01 on 2/21/26.
//

#ifndef AWSMOCK_QT_UI_S3_BUCKET_LAMBDA_NOTIFICATION_DIALOG_H
#define AWSMOCK_QT_UI_S3_BUCKET_LAMBDA_NOTIFICATION_DIALOG_H

// Qt includes
#include <QDialog>
#include <QStandardItemModel>

// Awsmock includes
#include <utils/IconUtils.h>
#include <utils/StringUtils.h>
#include <dto/s3/S3LambdaConfiguration.h>
#include <dto/s3/S3PutBucketNotificationConfigurationRequest.h>
#include <modules/lambda/LambdaService.h>
#include <modules/s3/S3Service.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class S3BucketLambdaNotificationDialog;
}

QT_END_NAMESPACE

class S3BucketLambdaNotificationDialog : public QDialog {
    Q_OBJECT

public:
    explicit S3BucketLambdaNotificationDialog(QString bucket, QWidget *parent = nullptr);

    explicit S3BucketLambdaNotificationDialog(QString bucket, S3LambdaConfiguration lambdaConfiguration, QWidget *parent = nullptr);

    ~S3BucketLambdaNotificationDialog() override;

    void Initialize();

    void SetupListButtons();

    void HandleAccept();

    void HandleReject();

    S3LambdaConfiguration GetLambdaConfiguration() const { return _lambdaConfiguration; }

private:
    /**
     * UI Components
     */
    Ui::S3BucketLambdaNotificationDialog *_ui;

    /**
     * @brief Bucket name
     */
    QString _bucket;

    /**
     * @brief S3 REST API connection
     */
    S3Service *_s3Service;

    /**
     * @brief Lambda REST API connection
     */
    LambdaService *_lambdaService{};

    /**
     * @brief Data model
     */
    QStandardItemModel *_leftDataModel{};

    /**
     * @brief Data model
     */
    QStandardItemModel *_rightDataModel{};

    /**
     * @brief S3 lambda notification
     */
    S3LambdaConfiguration _lambdaConfiguration;
};


#endif // AWSMOCK_QT_UI_S3_BUCKET_LAMBDA_NOTIFICATION_DIALOG_H
