//
// Created by vogje01 on 11/7/25.
//

#pragma once

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
#include <modules/apigateway/RestApiDetailsDialog.h>
#include <modules/application/ApplicationAddDialog.h>
#include <modules/application/ApplicationEditDialog.h>
#include <modules/application/ApplicationLogsDialog.h>
#include <modules/application/ApplicationService.h>
#include <modules/application/ApplicationUploadCodeDialog.h>
#include <utils/BasePage.h>
#include <utils/IconUtils.h>

#include "ApiGatewayService.h"

/**
 * @brief REST api list. The widget will be placed in the content pane.
 */
class RestApiList final : public BasePage {
    Q_OBJECT

public:
    /**
     * @brief SQSQueueList
     *
     * @param title widget title
     * @param parent parent widget
     */
    explicit RestApiList(const QString &title, QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~RestApiList() override;

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
     * @brief Handle the list REST API response from the server.
     *
     * @param restApiListResponse REST API counter list
     */
    void HandleListApplicationsSignal(const RestApiListResponse &restApiListResponse) const;

private slots:
    /**
     * @brief Show the context menu
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
    PageableTable *_tableView;

    /**
     * @brief Service
     */
    ApiGatewayService *_apiGatewayService;
};
