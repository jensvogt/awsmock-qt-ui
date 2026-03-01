//
// Created by vogje01 on 12/17/25.
//

#ifndef AWSMOCK_QT_UI_SECRET_ADD_DIALOG_H
#define AWSMOCK_QT_UI_SECRET_ADD_DIALOG_H

// Qt includes
#include <QDialog>
#include <QDialogButtonBox>

// AwsMock includes
#include <modules/secretsmanager/SecretsManagerService.h>
#include <modules/kms/KMSService.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class SecretAddDialog;
}

QT_END_NAMESPACE

class SecretAddDialog : public QDialog {
    Q_OBJECT

public:
    explicit SecretAddDialog(QWidget *parent = nullptr);

    ~SecretAddDialog() override;

    void HandleAccept();

    void HandleReject();

private:
    /**
     *  @brief UI components
     */
    Ui::SecretAddDialog *_ui;

    /**
     * @brief Secrets manager service
     */
    SecretsManagerService *_secretsManagerService{};

    /**
     * @brief KMS service
     */
    KMSService *_kmsService{};

    /**
     * @brief Secret counter
     */
    SecretCounter _secretCounter;
};


#endif //AWSMOCK_QT_UI_SECRET_ADD_DIALOG_H
