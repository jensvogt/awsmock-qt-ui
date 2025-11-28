//
// Created by vogje01 on 11/28/25.
//

#ifndef AWSMOCK_QT_UI_LAMBDA_RESULT_LIST_DIALOG_H
#define AWSMOCK_QT_UI_LAMBDA_RESULT_LIST_DIALOG_H

#include <QDialog>

#include <modules/lambda/LambdaService.h>

#include "utils/BaseDialog.h"

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
};


#endif //AWSMOCK_QT_UI_LAMBDA_RESULT_LIST_DIALOG_H
