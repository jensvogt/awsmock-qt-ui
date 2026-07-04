//
// Created by vogje01 on 6/27/26.
//

#pragma once

// C++ includes
#include <utility>

// Qt includes
#include <QDialog>

// Awsmock includes
#include <modules/lambda/LambdaService.h>
#include <utils/BaseDialog.h>
#include <utils/IconUtils.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class LambdaInstanceDialog;
}

QT_END_NAMESPACE

class LambdaInstanceDialog : public BaseDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param lambdaArn parent lambda ARN
     * @param instanceId instance ID
     * @param parent parent widget
     */
    explicit LambdaInstanceDialog(QString lambdaArn, QString instanceId, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~LambdaInstanceDialog() override;

    /**
     * @brief Handle OK button
     */
    void handleAccept();

    /**
     * @brief Handle CANCEL button
     */
    void handleReject();

    /**
     * @brief Handle content reloading load
     */
    void LoadContent() override;

    /**
     * @brief Update dialog fields
     *
     * @param lambdaResponse response from REST call
     */
    void handleUpdate(const LambdaGetInstanceResponse &lambdaResponse) const;

    void StartInstance() const;

private:
    /**
     * @brief UI components
     */
    Ui::LambdaInstanceDialog *_ui;

    /**
     * @brief Lambda function ARN
     */
    QString _arn;

    /**
     * @brief Lambda function ARN
     */
    QString _instanceId;

    /**
     * @brief Lambda REST service
     */
    LambdaService *_lambdaService;
};
