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
#include <modules/sqs/SQSService.h>
#include <modules/sqs/SQSMessageDetailsDialog.h>

/**
 * @brief Helper widget for the content area.
 * Displays a simple message based on the section selected.
 */
class SQSMessageList final : public BasePage
{
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
    SQSMessageList(const QString& title, const QString& queueArn, const QString &queueUrl, QWidget *parent = nullptr);

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

private:

    /**
     * @brief Qt network manager
     */
    QNetworkAccessManager *m_netManager;
    QTableWidget* tableWidget;
    SQSService sqsService;
    QString prefixValue = "";
    QString _queueArn;
    QString _queueUrl;
};

#endif // SQSMESSAGELIST_H
