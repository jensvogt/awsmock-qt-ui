#ifndef SQSMESSAGELIST_H
#define SQSMESSAGELIST_H

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
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <utils/BasePage.h>
#include <utils/IconUtils.h>
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
     * @param queueArn queue ARN
     * @param queueUrl queue URL
     * @param parent parent widget
     */
    SQSMessageList(const QString &title, QString queueArn, const QString &queueUrl, QWidget *parent = nullptr);

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
     * @param QueueArn Queue ARN of the Queue for the messages
     */
    void ShowMessages(const QString &QueueArn);

    /**
     * @brief Sends a back message to the main window
     */
    void BackToQueueList();

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
    void ShowContextMenu(const QPoint &pos) const;

    void OnBackClicked() {
        StopAutoUpdate();
        emit BackToQueueList();
    }

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

#endif // SQSMESSAGELIST_H
