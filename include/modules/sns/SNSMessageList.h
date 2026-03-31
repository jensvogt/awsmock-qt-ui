#ifndef AWSMOCK_QT_UI_SNS_MESSAGE_LIST_H
#define AWSMOCK_QT_UI_SNS_MESSAGE_LIST_H

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
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include <components/ContextMenu.h>
#include <components/PageableTable.h>
#include <dto/sns/SNSListMessagesResult.h>
#include <modules/sns/SNSMessageAddDialog.h>
#include <modules/sns/SNSMessageDetailsDialog.h>
#include <modules/sns/SNSService.h>
#include <utils/BasePage.h>
#include <utils/IconUtils.h>

/**
 * @brief SNS message list
 *
 * @author jens.vogt\@opitz-consulting.com
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
     * @brief Handle message list request
     *
     * @param listMessageResult message counter list
     */
    void HandleListMessageSignal(const SNSListMessagesResult &listMessageResult) const;

    /**
     * @brief Handle message reload
     */
    void HandleReloadMessageSignal() const;

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
    void ShowContextMenu(const QPoint &pos);

private:
    /**
     * @brief Parent topic ARN
     */
    QString _topicArn;

    /**
     * @brief Qt network manager
     */
    PageableTable *_tableView;

    /**
     * @brief REST service handler
     */
    SNSService *_snsService;
};

#endif // AWSMOCK_QT_UI_SNS_MESSAGE_LIST_H
