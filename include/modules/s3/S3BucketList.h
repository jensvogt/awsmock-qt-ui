#pragma once

// Qt includes
#include <QFormLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMenu>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTimer>
#include <QToolBar>
#include <QTreeWidget>
#include <QVBoxLayout>

// Awsmock includes
#include <components/ContextMenu.h>
#include <components/PageableTable.h>
#include <components/Toast.h>
#include <dto/s3/S3ListBucketResult.h>
#include <modules/s3/S3BucketEditDialog.h>
#include <modules/s3/S3ObjectAddDialog.h>
#include <modules/s3/S3Service.h>
#include <utils/BasePage.h>
#include <utils/IconUtils.h>

class S3BucketList final : public BasePage {
    Q_OBJECT

public:
    /**
     * @brief S3 Bucket List
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
     * @brief Clear the page content
     */
    void ClearContent() override {
        _tableView->Clear();
    }

    /**
     * @brief Load page content
     */
    void LoadContent() override;

    /**
     * @brief Load page content
     *
     * @param listBucketResult bucket counter list
     */
    void HandleListBucketSignal(const S3ListBucketResult &listBucketResult) const;

signals:
    /**
     * @brief Signaled when the bucket details are available
     *
     * @param bucketName name of the bucket
     */
    void ShowS3Objects(const QString &bucketName);

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
    PageableTable *_tableView;

    /**
     * @brief REST service handler
     */
    S3Service *_s3Service;
};
