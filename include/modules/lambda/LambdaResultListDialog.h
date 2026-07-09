//
// Created by vogje01 on 11/28/25.
//

#pragma once

// C++ includes
#include <utility>

// Qt includes
#include <QDialog>

// Awsmock includes
#include <components/Toast.h>
#include <modules/lambda/LambdaLogsDialog.h>
#include <modules/lambda/LambdaService.h>
#include <utils/BaseDialog.h>
#include <utils/IconUtils.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class LambdaResultListDialog;
}

QT_END_NAMESPACE

class LambdaResultListDialog final : public BaseDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param lambdaArn lambda AWS ARN
     * @param parent parent widget
     */
    explicit LambdaResultListDialog(QString lambdaArn, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~LambdaResultListDialog() override;

    /**
     * @brief Handle lambda list
     *
     * @param listResultsResponse list lambda response
     */
    void UpdateResultTable(const LambdaListResultsResponse &listResultsResponse) const;

    /**
     * @brief Handle OK button
     */
    void HandleAccept();

    /**
     * @brief Handle CANCEL button
     */
    void HandleReject();

    /**
     * @brief Load content callback
     */
    void LoadContent() override {
    };

private:
    /**
     * @brief UI components
     */
    Ui::LambdaResultListDialog *_ui;

    /**
     * @brief Lambda ARN
     */
    QString _lambdaArn;

    /**
     *  @brief Lambda service
     */
    LambdaService *_lambdaService;

    /**
     * @brief Sort column index
     *
     * @par Default sort column is 'Timestamp', index=-1
     */
    int _sortColumn = 3;

    /**
     * @brief Sort order
     */
    Qt::SortOrder _sortOrder = Qt::DescendingOrder;
};
