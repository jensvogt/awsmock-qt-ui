#ifndef AWSMOCK_QT_UI_S3_OBJECT_LIST_H
#define AWSMOCK_QT_UI_S3_OBJECT_LIST_H

#include <QFileDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QTableWidgetItem>

#include <components/ContextMenu.h>
#include <components/PageableTable.h>
#include <components/Toast.h>
#include <dto/s3/S3ListObjectResponse.h>
#include <modules/s3/S3ObjectAddDialog.h>
#include <modules/s3/S3ObjectEditDialog.h>
#include <modules/s3/S3Service.h>
#include <utils/BasePage.h>
#include <utils/IconUtils.h>
#include <utils/PrefixFilterModel.h>
#include <utils/StringUtils.h>

/**
 * @brief S3 object table.
 */
class S3ObjectList final : public BasePage {
    Q_OBJECT

public:
    /**
     * @brief SQSQueueList
     *
     * @param title widget title
     * @param parent parent widget
     */
    explicit S3ObjectList(const QString &title, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~S3ObjectList() override;

    /**
     * @brief Handles the bucket details signal
     *
     * @param bucketDetailsResponse bucker response
     */
    void HandleBucketDetailsSignal(const S3GetBucketDetailsResponse &bucketDetailsResponse);

    /**
     * @brief Clear the page content
     */
    void ClearContent() override {
        _tableView->Clear();
    }

    /**
     * @brief ListQueues
     */
    void LoadContent() override;

    /**
     * @brief Handle object list request
     *
     * @param listObjectResponse object counter list
     */
    void HandleListObjectSignal(const S3ListObjectsResponse &listObjectResponse) const;

    /**
     * @brief Handle object reload
     */
    void HandleReloadObjectSignal();

    void HandleBulkDelete(const QModelIndexList &sourceIndices) const;

    void HandleBulkTouch(const QModelIndexList &sourceIndices) const;

signals:
    /**
     * @brief Sent a show S3 object to the main window
     *
     * @param topicArn Topic ARN of the topic for the objects
     */
    void ShowObjects(const QString &topicArn);

    /**
     * @brief Sends a back object to the main window
     */
    void BackNavigationSignal();

private slots:
    /**
     * @brief Row context menu
     *
     * @param pos position in table
     */
    void ShowContextMenu(const QPoint &pos);

private:
    /**
     * @brief Widget label
     */
    QLabel *_titleLabel;

    /**
     * @brief Parent bucket name
     */
    QString _bucketName;

    /**
     * @brief Object table
     */
    PageableTable *_tableView;

    /**
     * @brief REST service handler
     */
    S3Service *_s3Service;

    /**
     * @brief Bucket details
     */
    S3GetBucketDetailsResponse _bucketDetailsResponse;
};

#endif // AWSMOCK_QT_UI_S3_OBJECT_LIST_H
