//
// Created by vogje01 on 11/27/25.
//

#ifndef AWSMOCK_QT_UI_LAMBDA_LOGS_DIALOG_H
#define AWSMOCK_QT_UI_LAMBDA_LOGS_DIALOG_H

#include <QDialog>
#include <QDialogButtonBox>

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
     * @param name Lambda function name
     * @param containerId lambda container ID
     * @param parent parent widget
     */
    explicit LambdaLogsDialog(const QString &name, const QString &containerId, QWidget *parent = nullptr);

    /**
     * @brief Destruuctor
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

private:
    /**
     * @brief UI components
     */
    Ui::LambdaLogsDialog *_ui;
};


#endif //AWSMOCK_QT_UI_LAMBDA_LOGS_DIALOG_H
