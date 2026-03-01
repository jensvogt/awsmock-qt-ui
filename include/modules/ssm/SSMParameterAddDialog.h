//
// Created by vogje01 on 12/19/25.
//

#ifndef AWSMOCK_QT_UI_SSM_PARAMETER_ADD_DIALOG_H
#define AWSMOCK_QT_UI_SSM_PARAMETER_ADD_DIALOG_H

// Qt includes
#include <QDialog>

// AwsMock includes
#include <utils/BaseDialog.h>
#include <modules/ssm/SSMService.h>
#include <modules/kms/KMSService.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class SSMParameterAddDialog;
}

QT_END_NAMESPACE

class SSMParameterAddDialog : public BaseDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param parent parent widget
     */
    explicit SSMParameterAddDialog(QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~SSMParameterAddDialog() override;

    /**
     * @brief Handle ok button
     */
    void HandleAccept();

    /**
     * @brief Handle cancel button
     */
    void HandleReject();

    /**
     * @brief Load content
     */
    void LoadContent() override;

private:
    /**
     * @brief UI components
     */
    Ui::SSMParameterAddDialog *_ui;

    /**
     * @brief REST service handler
     */
    SSMService *_ssmService;

    /**
     * @brief KMS service
     */
    KMSService *_kmsService{};
};


#endif // AWSMOCK_QT_UI_SSM_PARAMETER_ADD_DIALOG_H
