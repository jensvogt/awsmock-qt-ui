//
// Created by vogje01 on 1/11/26.
//

#ifndef AWSMOCK_QT_UI_S3_BUCKET_LIFECYCLE_DIALOG_H
#define AWSMOCK_QT_UI_S3_BUCKET_LIFECYCLE_DIALOG_H

// Qt includes
#include <QDialog>

// AwsMock includes
#include <dto/s3/S3GetBucketDetailsResponse.h>
QT_BEGIN_NAMESPACE

namespace Ui {
    class S3BucketLifecycleDialog;
}

QT_END_NAMESPACE

class S3BucketLifecycleDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Default constructor
     *
     * @param parent parent widget
     */
    explicit S3BucketLifecycleDialog(QWidget *parent);

    /**
     * @brief Constructor in edit mode
     *
     * @param lifecycleRule life cycle rule
     * @param parent parent widget
     */
    explicit S3BucketLifecycleDialog(const LifecycleRule &lifecycleRule, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~S3BucketLifecycleDialog() override;

    /**
     * Initialize UI components
     */
    void Initialize();

    void HandleAccept();

    void HandleReject();

    /**
     * @brief Returns the changed/added lifecycle rule
     */
    LifecycleRule GetLifecycleRule() { return _lifecycleRule; }

private:
    /**
     * UI components
     */
    Ui::S3BucketLifecycleDialog *_ui;

    /**
     * Lifecycle rule
     */
    LifecycleRule _lifecycleRule;
};


#endif // AWSMOCK_QT_UI_S3_BUCKET_LIFECYCLE_DIALOG_H
