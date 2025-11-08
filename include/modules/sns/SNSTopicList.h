#ifndef SNS_TOPIC_LIST_H
#define SNS_TOPIC_LIST_H

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
#include <modules/sns/SNSService.h>
#include <modules/sns/SNSTopicDetailsDialog.h>
#include <utils/IconUtils.h>

/**
 * @brief Helper widget for the content area.
 * Displays a simple message based on the section selected.
 */
class SNSTopicList : public BasePage
{
    Q_OBJECT

public:
    /**
     * @brief SQSQueueList
     *
     * @param title widget title
     * @param parent parent widget
     */
    SNSTopicList(const QString& title, QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~SNSTopicList();

    /**
     * @brief Load page content
     */
    void LoadContent() override;

    /**
     * @brief Load page content
     *
     * @param listTopicResult topic counter list
     */
    void HandleListTopicSignal(const SNSListTopicResult &listTopicResult);

signals:
    void ShowSnsMessages(const QString &topicArn);

private slots:
    void ShowContextMenu(const QPoint &pos) const;

private:

    /**
     * @brief Qt network manager
     */
    QTableWidget* tableWidget;
    QString prefixValue = "";
    QTimer *autoUpdateTimer{};
    SNSService* snsService;
    QString _region;
};

#endif // SNS_TOPIC_LIST_H
