//
// Created by vogje01 on 11/25/25.
//

#ifndef AWSMOCK_QT_UI_LAMBDA_DETAILS_DIALOG_H
#define AWSMOCK_QT_UI_LAMBDA_DETAILS_DIALOG_H

#include <QDialog>

#include <utils/IconUtils.h>
#include <utils/BaseDialog.h>
#include <dto/lambda/LambdaGetResponse.h>
#include <dto/lambda/LambdaListEnvironmentResponse.h>
#include <modules/lambda/LambdaService.h>
#include <modules/lambda/LambdaEnvironmentDetailDialog.h>

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
    explicit LambdaDetailsDialog(const QString &lambdaArn, QWidget *parent = nullptr);

    ~LambdaDetailsDialog() override;

    void UpdateLambda(const LambdaGetResponse &lambdaGetResponse) const;

    void SetupInstancesTab() const;

    void UpdateLambdaInstances(const LambdaListInstancesResponse &listInstancesResponse) const;

    void SetupEnvironmentTab() const;

    void UpdateLambdaEnvironment(const LambdaListEnvironmentResponse &listInstancesResponse) const;

    void ShowEnvironmentContextMenu(const QPoint &pos) const;

    void HandleAccept();

    void HandleReject();

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
};


#endif //AWSMOCK_QT_UI_LAMBDA_DETAILS_DIALOG_H
