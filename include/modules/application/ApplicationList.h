//
// Created by vogje01 on 11/7/25.
//

#ifndef AWSMOCK_QT_UI_APPLICATION_LIST_H
#define AWSMOCK_QT_UI_APPLICATION_LIST_H

// QT includes
#include <QTableWidget>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QPushButton>
#include <QHeaderView>
#include <QMenu>
#include <QHeaderView>

// Awsmock Qt includes
#include <utils/BasePage.h>
#include <utils/IconUtils.h>
#include <dto/sns/SNSListTopicResult.h>
#include <modules/application/ApplicationService.h>
#include <modules/application/ApplicationEditDialog.h>
#include <modules/application/ApplicationAddDialog.h>
#include <modules/application/ApplicationUploadCodeDialog.h>

/**
 * @brief Application list widget. The widget will be placed in the content pane.
 */
class ApplicationList final : public BasePage {
    Q_OBJECT

public:
    /**
     * @brief SQSQueueList
     *
     * @param title widget title
     * @param parent parent widget
     */
    explicit ApplicationList(const QString &title, QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~ApplicationList() override;

    /**
     * @brief Load page content
     */
    void LoadContent() override;

    /**
     * @brief Load page content
     *
     * @param listApplicationResponse application counter list
     */
    void HandleListApplicationsSignal(const ApplicationListResponse &listApplicationResponse);

private slots:
    /**
     * @brief Show context menu
     *
     * @param pos table position
     */
    void ShowContextMenu(const QPoint &pos);

private:
    /**
     * AWS region
     */
    QString _region;

    /**
     * @brief Table widget
     */
    QTableWidget *tableWidget;

    /**
     * @brief Prefix value
     */
    QString prefixValue = "";

    /**
     * @brief Service
     */
    ApplicationService *_applicationService;

    /**
     * @brief Sort column index
     */
    int _sortColumn = 0;

    /**
     * @brief Sort order
     */
    Qt::SortOrder _sortOrder = Qt::AscendingOrder;

    /**
     * @brief Prefix clear button
     */
    QPushButton *prefixClear;
};

#endif //AWSMOCK_QT_UI_APPLICATION_LIST_H
