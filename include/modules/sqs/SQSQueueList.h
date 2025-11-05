#ifndef SQS_QUEUE_LIST_H
#define SQS_QUEUE_LIST_H

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
#include <modules/sqs/SQSService.h>
#include <modules/sqs/SQSQueueDetailsDialog.h>

/**
 * @brief Helper widget for the content area.
 * Displays a simple message based on the section selected.
 */
class SQSQueueList : public BasePage
{
    Q_OBJECT

public:
    /**
     * @brief SQSQueueList
     *
     * @param title widget title
     * @param parent parent widget
     */
    SQSQueueList(const QString& title, QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~SQSQueueList();

    /**
     * @brief Load page content
     */
    void LoadContent() override;

signals:
    void StatusUpdateRequested(const QString &text);
    void ShowMessages(const QString &QueueArn, const QString &QueueUrl);

private slots:
    void ShowContextMenu(const QPoint &pos);

    void NotifyStatusBar() {
        QString msg = "Last update: " + QDateTime::currentDateTime().toString("hh:mm:ss");
        emit StatusUpdateRequested(msg);
    }

private:

    /**
     * @brief Qt network manager
     */
    QTableWidget* tableWidget;
    QString prefixValue = "";
    QTimer *autoUpdateTimer;
    SQSService* sqsService;
};

#endif // SQS_QUEUE_LIST_H
