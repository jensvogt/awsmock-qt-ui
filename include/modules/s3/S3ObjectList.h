#ifndef AWSMOCK_QT_UI_S3_OBJECT_LIST_H
#define AWSMOCK_QT_UI_S3_OBJECT_LIST_H

#include <QMenu>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QPushButton>
#include <QTableWidgetItem>

#include <utils/BasePage.h>
#include <dto/s3/S3ListObjectResponse.h>
#include <modules/s3/S3Service.h>
//#include <modules/s3/S3ObjectDetailsDialog.h>
#include <utils/IconUtils.h>

/**
 * @brief Helper widget for the content area.
 * Displays a simple object based on the section selected.
 */
class S3ObjectList final : public BasePage {
    Q_OBJECT

public:
    /**
     * @brief SQSQueueList
     *
     * @param title widget title
     * @param bucketName ARN of the parent topic
     * @param parent parent widget
     */
    S3ObjectList(const QString &title, const QString &bucketName, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~S3ObjectList() override;

    /**
     * @brief ListQueues
     */
    void LoadContent() override;

    /**
     * @brief Handle object list request
     *
     * @param listObjectResult object counter list
     */
    void HandleListObjectSignal(const S3ListObjectsResult &listObjectResult);

    /**
     * @brief Handle object reload
     */
    void HandleReloadObjectSignal();

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
    void BackToBucketList();

private slots:
    /**
     * @brief Row context menu
     *
     * @param pos position in table
     */
    void ShowContextMenu(const QPoint &pos) const;

private:
    /**
     * @brief Parent bucket name
     */
    QString bucketName;

    /**
     * @brief Object table
     */
    QTableWidget *tableWidget;

    /**
     * @brief REST service handler
     */
    S3Service *_s3Service;

    /**
     * @brief Prefix search
     */
    QString prefixValue = "";

    /**
     * @brief Sort column index
     *
     * @par Default sort column is 'objects', index=1
     */
    int _sortColumn = 1;

    /**
     * @brief Sort order
     */
    Qt::SortOrder _sortOrder = Qt::DescendingOrder;
};

#endif // AWSMOCK_QT_UI_S3_OBJECT_LIST_H
