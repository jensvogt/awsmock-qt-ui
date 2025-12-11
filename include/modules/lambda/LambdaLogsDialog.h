//
// Created by vogje01 on 11/27/25.
//

#ifndef AWSMOCK_QT_UI_LAMBDA_LOGS_DIALOG_H
#define AWSMOCK_QT_UI_LAMBDA_LOGS_DIALOG_H

#include <QDialog>
#include <QDialogButtonBox>

#include <modules/lambda/LambdaService.h>
#include <utils/IconUtils.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class LambdaLogsDialog;
}

QT_END_NAMESPACE

class LambdaLogsDialog final : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param oid Lambda function result OID
     * @param parent parent widget
     */
    explicit LambdaLogsDialog(const QString &oid, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~LambdaLogsDialog() override;

    /**
     * @brief Handle dialog accept
     */
    void HandleAccept();

    /**
     * @brief Handle dialog reject
     */
    void HandleReject();

    void HandleGetResult(const LambdaGetResultsResponse &lambdaLogsResult) const;

    void HandleRequestPrettyButton(bool checked) const;

private:
    /**
     * @brief UI components
     */
    Ui::LambdaLogsDialog *_ui;

    /**
     *  @brief Lambda service
     */
    LambdaService *_lambdaService;
};


#endif //AWSMOCK_QT_UI_LAMBDA_LOGS_DIALOG_H
