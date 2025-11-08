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
    explicit SNSTopicList(const QString& title, QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~SNSTopicList() override;

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
     * @brief AWS region
     */
    QString _region;

    /**
     * @brief Qt network manager
     */
    QTableWidget* tableWidget;

    /**
     * @brief Topic prefix search
     */
    QString prefixValue = "";

    /**
     * @brief REST service handler
     */
    SNSService* snsService;

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

#endif // SNS_TOPIC_LIST_H
