//
// Created by vogje01 on 11/25/25.
//

#ifndef AWSMOCK_QT_UI_LAMBDA_DETAILS_DIALOG_H
#define AWSMOCK_QT_UI_LAMBDA_DETAILS_DIALOG_H

#include <QDialog>

#include <utils/IconUtils.h>
#include <modules/lambda/LambdaService.h>
#include <dto/lambda/LambdaGetResponse.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class LambdaDetailsDialog;
}

QT_END_NAMESPACE

class LambdaDetailsDialog final : public QDialog {
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
};


#endif //AWSMOCK_QT_UI_LAMBDA_DETAILS_DIALOG_H
