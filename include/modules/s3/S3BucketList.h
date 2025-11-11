#ifndef AWSMOCK_QT_UI_S3_BUCKET_LIST_H
#define AWSMOCK_QT_UI_S3_BUCKET_LIST_H

#include <QLabel>
#include <QMenu>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QListWidget>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QLineEdit>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QToolBar>
#include <QPushButton>
#include <QInputDialog>
#include <QFormLayout>
#include <QTreeWidget>
#include <QTimer>

#include <utils/BasePage.h>
#include <utils/IconUtils.h>
#include <modules/s3/S3Service.h>
#include <dto/s3/S3ListBucketResult.h>

class S3BucketList final : public BasePage {
    Q_OBJECT

public:
    /**
     * @brief S3 bucket list
     *
     * @param title widget title
     * @param parent parent widget
     */
    explicit S3BucketList(const QString &title, QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~S3BucketList() override;

    /**
     * @brief Load page content
     */
    void LoadContent() override;

    /**
     * @brief Load page content
     *
     * @param listBucketResult bucket counter list
     */
    void HandleListBucketSignal(const S3ListBucketResult &listBucketResult);

signals:
    //void ShowSnsMessages(const QString &topicArn);

private slots:
    /**
     * @brief Context menu callback
     */
    void ShowContextMenu(const QPoint &pos) const;

private:
    /**
     * @brief AWS region
     */
    QString _region;

    /**
     * @brief Qt network manager
     */
    QTableWidget *tableWidget;

    /**
     * @brief Topic prefix search
     */
    QString prefixValue = "";

    /**
     * @brief REST service handler
     */
    S3Service *_s3Service;

    /**
     * @brief Sort column index
     *
     * @par Default sort column is 'messages', index=1
     */
    int _sortColumn = 1;

    /**
     * @brief Sort order
     */
    Qt::SortOrder _sortOrder = Qt::DescendingOrder;
};

#endif // AWSMOCK_QT_UI_S3_BUCKET_LIST_H
