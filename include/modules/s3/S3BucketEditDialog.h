//
// Created by vogje01 on 11/12/25.
//

#ifndef AWSMOCK_QT_UI_S3_BUCKET_EDIT_DIALOG_H
#define AWSMOCK_QT_UI_S3_BUCKET_EDIT_DIALOG_H

#include <QDialog>

#include <modules/s3/S3Service.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class S3BucketEditDialog;
}

QT_END_NAMESPACE

class S3BucketEditDialog final : public QDialog {
    Q_OBJECT

public:
    explicit S3BucketEditDialog(const QString &bucketName, QWidget *parent = nullptr);

    ~S3BucketEditDialog() override;

    void UpdateBucket(const S3GetBucketDetailsResponse &bucketGetResponse) const;

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
    S3BucketCounter _bucket;

    /**
     * @brief Changed flag
     */
    bool _changed = false;
};


#endif //AWSMOCK_QT_UI_S3_BUCKET_EDIT_DIALOG_H
