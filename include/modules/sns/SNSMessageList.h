#ifndef AWSMOCK_QT_UI_SNS_MESSAGE_LIST_H
#define AWSMOCK_QT_UI_SNS_MESSAGE_LIST_H

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
#include <dto/sns/SNSListMessagesResult.h>
#include <modules/sns/SNSService.h>
#include <modules/sns/SNSMessageDetailsDialog.h>
#include <modules/sns/SNSMessageAddDialog.h>

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
     * @param parent parent widget
     */
    explicit SNSMessageList(const QString &title, QWidget *parent = nullptr);

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

private slots:
    /**
     * @brief Row context menu
     *
     * @param pos position in table
     */
    void ShowContextMenu(const QPoint &pos) const;

private:
    /**
     * @brief Parent topic ARN
     */
    QString _topicArn;

    /**
     * @brief Qt network manager
     */
    QTableWidget *_tableWidget;

    /**
     * @brief REST service handler
     */
    SNSService *_snsService;

    /**
     * @brief Prefix search
     */
    QString _prefixValue = "";

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

    /**
     * @brief Prefix clear button
     */
    QPushButton *_prefixClear;
};

#endif // AWSMOCK_QT_UI_SNS_MESSAGE_LIST_H
