//
// Created by vogje01 on 11/25/25.
//

#pragma once

// C++ includes
#include <utility>

// Qt includes
#include <QDialog>
#include <QMenu>

// Awsmock includes
#include <components/Toast.h>
#include <dto/lambda/LambdaGetResponse.h>
#include <dto/lambda/LambdaListEnvironmentResponse.h>
#include <dto/lambda/LambdaListEventSourcesResponse.h>
#include <modules/lambda/LambdaEnvironmentDetailDialog.h>
#include <modules/lambda/LambdaEnvironmentDetailDialog.h>
#include <modules/lambda/LambdaEventSourceDetailDialog.h>
#include <modules/lambda/LambdaInstanceDialog.h>
#include <modules/lambda/LambdaResultListDialog.h>
#include <modules/lambda/LambdaService.h>
#include <utils/BaseDialog.h>
#include <utils/IconUtils.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class LambdaDetailsDialog;
}

QT_END_NAMESPACE

class LambdaDetailsDialog final : public BaseDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param lambdaArn lambda AWS ARN
     * @param parent parent widget
     */
    explicit LambdaDetailsDialog(QString lambdaArn, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~LambdaDetailsDialog() override;

    /**
     * @brief Update lambda dtails
     *
     * @param lambdaGetResponse lambda service response
     */
    void UpdateLambda(const LambdaGetResponse &lambdaGetResponse);

    /**
     * @brief Reload the lambda instances tab
     */
    void ReloadLambdaInstances();

    /**
     * @brief Initialize instances tab
     */
    void SetupInstancesTab();

    /**
     * @brief Update lambda instances
     *
     * @param listInstancesResponse lambda service response
     */
    void UpdateLambdaInstances(const LambdaListInstancesResponse &listInstancesResponse) const;

    /**
     * @brief Initialize environment tab
     */
    void SetupEnvironmentTab() const;

    /**
     * @brief Initialize lambda environment tab
     *
     * @param listEnvironmentResponse lambda service response
     */
    void UpdateLambdaEnvironment(const LambdaListEnvironmentResponse &listEnvironmentResponse) const;

    /**
     * @brief Show the environment context menu
     *
     * @param pos position in table
     */
    void ShowEnvironmentContextMenu(const QPoint &pos) const;

    /**
     * @brief Delete the currently selected environment variable
     */
    void DeleteSelectedEnvironment() const;

    /**
     * @brief Initialize event sources tab
     */
    void SetupEventSourcesTab() const;

    /**
     * @brief Update lambda event sources
     *
     * @param listEventSourcesResponse lambda service response
     */
    void UpdateLambdaEventSources(const LambdaListEventSourcesResponse &listEventSourcesResponse) const;

    /**
     * @brief Show the event source context menu
     *
     * @param pos position in table
     */
    void ShowEventSourceContextMenu(const QPoint &pos) const;

    /**
     * @brief Delete the currently selected event source mapping
     */
    void DeleteSelectedEventSource() const;

    /**
     * @brief Show the instance context menu
     *
     * @param pos position in table
     */
    void ShowInstanceContextMenu(const QPoint &pos) const;

    /**
     * @brief Kill the currently selected lambda instance
     */
    void KillSelectedInstance() const;

    void StartInstance() const;

    /**
     * @brief Handle OK button
     */
    void HandleAccept();

    /**
     * @brief Handle CANCEL button
     */
    void HandleReject();

    /**
     * @brief Load lamba details
     */
    void LoadContent() override;

private:
    /**
     * @brief UI components
     */
    Ui::LambdaDetailsDialog *_ui;

    /**
     * @brief Lambda REST service
     */
    LambdaService *_lambdaService;

    /**
     * @brief Lambda ARN
     */
    QString _lambdaArn;

    /**
     * @brief Changed flag
     */
    bool _changed = false;

    /**
     * @brief Instance sort column index
     */
    int _instanceSortColumn = 0;

    /**
     * @brief Instance sort order
     */
    Qt::SortOrder _instanceSortOrder = Qt::AscendingOrder;

    /**
     * @brief Environment sort column index
     */
    int _environmentSortColumn = 0;

    /**
     * @brief Environment sort order
     */
    Qt::SortOrder _environmentSortOrder = Qt::AscendingOrder;

    /**
     *  @brief Save the response
     */
    LambdaGetResponse _lambdaGetResponse;
};
