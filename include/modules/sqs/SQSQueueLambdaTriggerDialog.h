//
// Created by jensv on 01/08/2026.
//

#ifndef AWSMOCK_QT_UI_SQS_QUEUE_LAMBDA_TRIGGER_DIALOG_H
#define AWSMOCK_QT_UI_SQS_QUEUE_LAMBDA_TRIGGER_DIALOG_H

// Qt includes
#include <QDialog>
#include <QMessageBox>
#include <QDialogButtonBox>

// Awsmock includes
#include <dto/lambda/LambdaListArnsResponse.h>
#include <modules/lambda/LambdaService.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class SQSQueueLambdaTriggerDialog;
}

QT_END_NAMESPACE

class SQSQueueLambdaTriggerDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Adds a new lambda trigger for a SQS queue
     *
     * @param parent parent widget
     */
    explicit SQSQueueLambdaTriggerDialog(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~SQSQueueLambdaTriggerDialog() override;

    /**
     * @brief Returns the selected lambda function ARN
     *
     * @return lambda function ARN
     */
    QString GetLambdaArn() const {
        return _lambdaArn;
    }

    /**
     * @brief Returns the enabled flag
     *
     * @return enabled flag
     */
    bool GetEnabled() const {
        return _enabled;
    }

    /**
     * @brief Handle OK button
     */
    void HandleAccept();

    /**
     * @brief Handle CANCEL button
     */
    void HandleReject();

private:
    /**
     * @brief UI components
     */
    Ui::SQSQueueLambdaTriggerDialog *_ui;

    /**
     * @brief Lambda REST service, used to fill in the list of lambda ARNs
     */
    LambdaService *_lambdaService;

    /**
     * @brief Selected lambda function ARN
     */
    QString _lambdaArn{};

    /**
     * @brief Enabled flag
     */
    bool _enabled = true;
};


#endif // AWSMOCK_QT_UI_SQS_QUEUE_LAMBDA_TRIGGER_DIALOG_H
