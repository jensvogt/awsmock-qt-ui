#ifndef SNS_MESSAGE_LIST_H
#define SNS_MESSAGE_LIST_H

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
#include <dto/sns/SNSListMessagesResult.h>
#include <modules/sns/SNSService.h>
#include <modules/sns/SNSMessageDetailsDialog.h>

/**
 * @brief Helper widget for the content area.
 * Displays a simple message based on the section selected.
 */
class SNSMessageList : public BasePage
{
    Q_OBJECT

public:
    /**
     * @brief SQSQueueList
     *
     * @param title widget title
     * @param topicArn ARN of the parent topic
     * @param parent parent widget
     */
    SNSMessageList(const QString& title, const QString& topicArn, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~SNSMessageList();

    /**
     * @brief ListQueues
     */
    void LoadContent() override;

    /**
     * @brief Handle message list request
     *
     * @param listMessageResult message counter list
     */
    void HandleListMessageSignal(const SNSListMessagesResult &listMessageResult);

    /**
     * @brief Handle message reload
     */
    void HandleReloadMessageSignal();

signals:
    /**
     * @brief Send a status bar update message to the main window
     *
     * @param text status text
     */
    void StatusUpdateRequested(const QString &text);

    /**
     * @brief Sent a show SNS message to the main window
     *
     * @param topicArn Topic ARN of the topic for the messages
     */
    void ShowMessages(const QString &topicArn);

    /**
     * @brief Sents a back message to the main window
     *
     * @param QueueArn Queue ARN of the Queue for the messages
     */
    void BackToTopicList();

private slots:

    /**
     * @brief Row context menu
     *
     * @param pos position in table
     */
    void ShowContextMenu(const QPoint &pos);

    /**
     * @brief Notify status bar
     */
    void NotifyStatusBar() {
        QString msg = "Last update: " + QDateTime::currentDateTime().toString("hh:mm:ss");
        emit StatusUpdateRequested(msg);
    }

    void OnBackClicked() {
        StopAutoUpdate();
        emit BackToTopicList();
    }

private:

    /**
     * @brief Qt network manager
     */
    QTableWidget* tableWidget;
    SNSService* snsService;
    QString prefixValue = "";
    QString topicArn;
};

#endif // SNS_MESSAGE_LIST_H
