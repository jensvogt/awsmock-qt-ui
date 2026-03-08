#ifndef AWSMOCK_QT_UI_SNS_QUEUE_LIST_H
#define AWSMOCK_QT_UI_SNS_QUEUE_LIST_H

// Qt includes
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

// Awsmock includes
#include <utils/BasePage.h>
#include <utils/IconUtils.h>
#include <utils/StringUtils.h>
#include <components/PageableTable.h>
#include <modules/sns/SNSService.h>
#include <modules/sns/SNSTopicDetailsDialog.h>

/**
 * @brief Helper widget for the content area.
 * Displays a simple message based on the section selected.
 */
class SNSTopicList : public BasePage {
    Q_OBJECT

public:
    /**
     * @brief SQSQueueList
     *
     * @param title widget title
     * @param parent parent widget
     */
    explicit SNSTopicList(const QString &title, QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~SNSTopicList() override;

    /**
     * @brief Clear the page content
     */
    void ClearContent() override {
        _tableView->Clear();
    }

    /**
     * @brief Load page content
     */
    void LoadContent() override;

    /**
     * @brief Load page content
     *
     * @param listTopicResult topic counter list
     */
    void HandleListTopicSignal(const SNSListTopicResult &listTopicResult) const;

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
    PageableTable *_tableView;

    /**
     * @brief REST service handler
     */
    SNSService *_snsService;
};

#endif // AWSMOCK_QT_UI_SNS_QUEUE_LIST_H
