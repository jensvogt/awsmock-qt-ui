//
// Created by vogje01 on 11/7/25.
//

#ifndef AWSMOCK_QT_UI_LAMBDA_LIST_H
#define AWSMOCK_QT_UI_LAMBDA_LIST_H

// QT includes
#include <QMenu>
#include <QLabel>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QPushButton>
#include <QHeaderView>
#include <QHeaderView>

// Awsmock includes
#include <utils/BasePage.h>
#include <utils/IconUtils.h>
#include <dto/sns/SNSListTopicResult.h>
#include <modules/lambda/LambdaService.h>
#include <modules/lambda/LambdaDetailsDialog.h>
#include <modules/lambda/LambdaLogsDialog.h>
#include <modules/lambda/LambdaResultListDialog.h>
#include <modules/lambda/LambdaUploadCodeDialog.h>
#include <modules/docker/DockerService.h>

#include "components/PageableTable.h"

/**
 * @brief Lambda list widget. The widget will be placed in the content pane.
 */
class LambdaList final : public BasePage {
    Q_OBJECT

public:
    /**
     * @brief SQSQueueList
     *
     * @param title widget title
     * @param parent parent widget
     */
    explicit LambdaList(const QString &title, QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~LambdaList() override;

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
     * @param listLambdaResponse lambda counter list
     */
    void HandleListLambdasSignal(const LambdaListResponse &listLambdaResponse) const;

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
    PageableTable *_tableView;

    /**
     * @brief Lambda service
     */
    LambdaService *_lambdaService;

    /**
     * @brief Container service
     */
    DockerService *_containerService;
};

#endif //AWSMOCK_QT_UI_LAMBDA_LIST_H
