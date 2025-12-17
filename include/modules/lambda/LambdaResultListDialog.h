//
// Created by vogje01 on 11/28/25.
//

#ifndef AWSMOCK_QT_UI_LAMBDA_RESULT_LIST_DIALOG_H
#define AWSMOCK_QT_UI_LAMBDA_RESULT_LIST_DIALOG_H

#include <QDialog>

#include <utils/BaseDialog.h>
#include <utils/IconUtils.h>
#include <modules/lambda/LambdaService.h>
#include <modules/lambda/LambdaLogsDialog.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class LambdaResultListDialog;
}

QT_END_NAMESPACE

class LambdaResultListDialog final : public BaseDialog {
    Q_OBJECT

public:
    explicit LambdaResultListDialog(const QString &lambdaArn, QWidget *parent = nullptr);

    ~LambdaResultListDialog() override;

    void UpdateResultTable(const LambdaListResultsResponse &listResultsResponse) const;

    void HandleAccept();

    void HandleReject();

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


#endif //AWSMOCK_QT_UI_LAMBDA_RESULT_LIST_DIALOG_H
