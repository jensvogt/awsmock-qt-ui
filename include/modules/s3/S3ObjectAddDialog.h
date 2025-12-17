//
// Created by vogje01 on 12/13/25.
//

#ifndef AWSMOCK_QT_UI_S3_OBJECT_ADD_DIALOG_H
#define AWSMOCK_QT_UI_S3_OBJECT_ADD_DIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>

#include <utils/Configuration.h>
#include <utils/IconUtils.h>
#include <utils/BaseDialog.h>
#include <dto/s3/S3GetBucketDetailsResponse.h>
#include <modules/s3/S3ObjectMetadataDialog.h>
#include <modules/s3/S3Service.h>


QT_BEGIN_NAMESPACE

namespace Ui {
    class S3ObjectAddDialog;
}

QT_END_NAMESPACE

class S3ObjectAddDialog : public BaseDialog {
    Q_OBJECT

public:
    explicit S3ObjectAddDialog(const S3GetBucketDetailsResponse &bucket, QWidget *parent = nullptr);

    ~S3ObjectAddDialog() override;

    void BrowseSourceFile();

    void HandleAccept();

    void HandleReject();

    QString GetS3ObjectKey() {
        return _s3Key;
    }

    QString GetFilename() {
        return _fileName;
    }

    void LoadContent() override {
    }

private:
    Ui::S3ObjectAddDialog *_ui;

    /**
     * @brief Full qualified file name
     */
    QString _fileName;

    /**
     * @brief S3 object key
     */
    QString _s3Key;

    /**
     * @brief S3 bucket to upload
     */
    S3GetBucketDetailsResponse _bucket;

    /**
     *  @brief Table data model
     */
    QStandardItemModel *_dataModel;

    /**
     * @brief File info for the file to upload
     */
    QFileInfo _fileInfo;

    /**
     * @brief S3 REST service
     */
    S3Service *_s3Service;

    /**
     * @brief Metadata
     */
    QMap<QString, QString> _metadata;
};


#endif //AWSMOCK_QT_UI_S3_OBJECT_ADD_DIALOG_H
