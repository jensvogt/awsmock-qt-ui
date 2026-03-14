#ifndef AWSMOCK_QT_UI_SQS_QUEUE_LIST_H
#define AWSMOCK_QT_UI_SQS_QUEUE_LIST_H

// Qt includes
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

// Awsmock include
#include <utils/BasePage.h>
#include <utils/IconUtils.h>
#include <utils/EventBus.h>
#include <components/PageableTable.h>
#include <dto/sqs/SQSListQueueResponse.h>
#include <modules/sqs/SQSService.h>
#include <modules/sqs/SQSQueueDetailsDialog.h>
#include <modules/sqs/SQSMessageAddDialog.h>

/**
 * @brief Helper widget for the content area.
 * Displays a simple message based on the section selected.
 */
class SQSQueueList final : public BasePage {
    Q_OBJECT

public:
    /**
     * @brief SQSQueueList
     *
     * @param title widget title
     * @param parent parent widget
     */
    explicit SQSQueueList(const QString &title, QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~SQSQueueList() override;

    /**
     * @brief Load page content
     */
    void LoadContent() override;

    /**
     * @brief Clear the page content
     */
    void ClearContent() override {
        _tableView->Clear();
    };

    /**
     * @brief Load page content
     *
     * @param queueListResponse queue counter list
     */
    void HandleListQueueSignal(const SQSQueueListResponse &queueListResponse) const;

    signals:
    

    void ShowMessages(const QString &QueueArn, const QString &QueueUrl, bool isDql);

private
    slots:
    

    void ShowContextMenu(const QPoint &pos) const;

private:
    /**
     * @brief Qt network manager
     */
    PageableTable *_tableView;

    /**
     * @brief REST service handler
     */
    SQSService *_sqsService;

    /**
     * @brief Sort column index
     *
     * @par Default sort column is 'Available', index=1
     */
    int _sortColumn = 1;

    /**
     * @brief Sort order
     */
    Qt::SortOrder _sortOrder = Qt::DescendingOrder;

    /**
     * @brief Table header
     */
    QHeaderView *_tableHeader{};

    /**
     * @brief Prefix clear button
     */
    QPushButton *prefixClear{};
};
#endif // AWSMOCK_QT_UI_SQS_QUEUE_LIST_H