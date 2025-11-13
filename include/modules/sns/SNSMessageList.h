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
#include <utils/IconUtils.h>

/**
 * @brief Helper widget for the content area.
 * Displays a simple message based on the section selected.
 */
class SNSMessageList : public BasePage {
    Q_OBJECT

public:
    /**
     * @brief SQSQueueList
     *
     * @param title widget title
     * @param topicArn ARN of the parent topic
     * @param parent parent widget
     */
    SNSMessageList(const QString &title, const QString &topicArn, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~SNSMessageList() override;

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
     * @brief Sent a show SNS message to the main window
     *
     * @param topicArn Topic ARN of the topic for the messages
     */
    void ShowMessages(const QString &topicArn);

    /**
     * @brief Sends a back message to the main window
     */
    void BackToTopicList();

private slots:
    /**
     * @brief Row context menu
     *
     * @param pos position in table
     */
    void ShowContextMenu(const QPoint &pos) const;

    void OnBackClicked() {
        StopAutoUpdate();
        emit BackToTopicList();
    }

private:
    /**
     * @brief Parent topic ARN
     */
    QString topicArn;

    /**
     * @brief Qt network manager
     */
    QTableWidget *tableWidget;

    /**
     * @brief REST service handler
     */
    SNSService *_snsService;

    /**
     * @brief Prefix search
     */
    QString prefixValue = "";

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

#endif // SNS_MESSAGE_LIST_H
