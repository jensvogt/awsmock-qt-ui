#ifndef AWSMOCK_QT_UI_SQS_MESSAGE_LIST_H
#define AWSMOCK_QT_UI_SQS_MESSAGE_LIST_H

#include <QMenu>
#include <QLabel>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QToolBar>
#include <QPushButton>

// Awsmock includes
#include <utils/BasePage.h>
#include <utils/IconUtils.h>
#include <utils/EventBus.h>
#include <modules/sqs/SQSService.h>
#include <modules/sqs/SQSMessageDetailsDialog.h>
#include <modules/sqs/SQSMessageAddDialog.h>

/**
 * @brief Helper widget for the content area.
 * Displays a simple message based on the section selected.
 */
class SQSMessageList final : public BasePage {
    Q_OBJECT

public:
    /**
     * @brief SQSQueueList
     *
     * @param title widget title
     * @param parent parent widget
     */
    explicit SQSMessageList(const QString &title, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~SQSMessageList() override;

    /**
     * @brief ListQueues
     */
    void LoadContent() override;

signals:
    /**
     * @brief Sent a show SQS message to the main window
     *
     * @param queueArn Queue ARN of the Queue for the messages
     */
    void ShowMessages(const QString &queueArn);

    /**
     * @brief Sends a reload message list to the message list
     */
    void ReloadMessagesSignal();

private slots:
    /**
     * @brief Row context menu
     *
     * @param pos position in table
     */
    void ShowContextMenu(const QPoint &pos);

    /**
     * @brief Handle message list request
     *
     * @param listMessageResponse message counter list
     */
    void HandleListMessageSignal(const SQSListMessagesResponse &listMessageResponse);

    /**
     * @brief Handle message reload
     */
    void HandleReloadMessageSignal() const;

private:
    /**
     * @brief QT table
     */
    QTableWidget *tableWidget;

    /**
     * @brief REST service handler
     */
    SQSService *_sqsService;

    /**
     * @brief prefix search
     */
    QString prefixValue = "";

    /**
     * @brief SQS queue ARN
     */
    QString _queueArn;

    /**
     * @brief SQS queue URL
     */
    QString _queueUrl;

    /**
     * @brief DQL flag
     */
    bool _isDlq = false;

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
     * @brief Prefix clear button
     */
    QPushButton *prefixClear;
};

#endif // AWSMOCK_QT_UI_SQS_MESSAGE_LIST_H
