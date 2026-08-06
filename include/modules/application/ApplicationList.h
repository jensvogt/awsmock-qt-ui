//
// Created by vogje01 on 11/7/25.
//

#ifndef AWSMOCK_QT_UI_APPLICATION_LIST_H
#define AWSMOCK_QT_UI_APPLICATION_LIST_H

// QT includes
#include <QHBoxLayout>
#include <QHeaderView>
#include <QHeaderView>
#include <QInputDialog>
#include <QMenu>
#include <QPushButton>
#include <QTableWidget>

// Awsmock Qt includes
#include <components/ContextMenu.h>
#include <components/PageableTable.h>
#include <components/Toast.h>
#include <dto/sns/SNSListTopicResult.h>
#include <modules/application/ApplicationAddDialog.h>
#include <modules/application/ApplicationEditDialog.h>
#include <modules/application/ApplicationLogsDialog.h>
#include <modules/application/ApplicationService.h>
#include <modules/application/ApplicationUploadCodeDialog.h>
#include <utils/BasePage.h>
#include <utils/IconUtils.h>

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
     * @brief Clear the page content
     */
    void ClearContent() override {
    }

    /**
     * @brief Load page content
     */
    void LoadContent() override;

    /**
     * @brief Load page content
     *
     * @param listApplicationResponse application counter list
     */
    void HandleListApplicationsSignal(const ApplicationListResponse &listApplicationResponse) const;

private slots:
    /**
     * @brief Show context menu
     *
     * @param pos table position
     */
    void ShowContextMenu(const QPoint &pos);

    /**
     * @brief Delete the currently selected application(s)
     */
    void DeleteSelected();

private:
    /**
     * AWS region
     */
    QString _region;

    /**
     * @brief Table widget
     */
    PageableTable *_tableView;

    /**
     * @brief Service
     */
    ApplicationService *_applicationService;
};

#endif //AWSMOCK_QT_UI_APPLICATION_LIST_H
