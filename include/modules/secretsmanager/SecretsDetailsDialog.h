//
// Created by vogje01 on 12/16/25.
//

#ifndef AWSMOCK_QT_UI_SECRETS_DETAILS_DIALOG_H
#define AWSMOCK_QT_UI_SECRETS_DETAILS_DIALOG_H

// C*+ includes
#include <utility>

// Qt includes
#include <QDialog>
#include <QStringList>
#include <QDialogButtonBox>
#include <QStandardItemModel>

// AwsMock includes
#include <utils/BaseDialog.h>

QT_BEGIN_NAMESPACE

// AwsMock includes
#include <utils/IconUtils.h>
#include <modules/secretsmanager/SecretsManagerService.h>

namespace Ui {
    class SecretsDetailsDialog;
}

QT_END_NAMESPACE

class SecretsDetailsDialog : public BaseDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param secretArn Secret AWS ARN
     * @param parent parent widget
     */
    explicit SecretsDetailsDialog(QString secretArn, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~SecretsDetailsDialog() override;

    /**
     * @brief Handle ok button
     */
    void HandleAccept();

    /**
     * @brief Handle cancel button
     */
    void HandleReject();

    /**
     * @brief Load dialog content
     */
    void LoadContent() override;

    /**
     * @brief Update secret values
     *
     * @param secretCounter
     * @param secretCounter
     */
    void UpdateSecret(const SecretCounter &secretCounter);

    /**
     * @brief Pretty print toggle button callback
     *
     * @param checked toggle button state
     */
    void PrettyPrintClicked(bool checked) const;

    /**
     * @brief Tab widget current tab changed
     *
     * @param tabIndex current tab index
     */
    void HandleTabChanged(int tabIndex) const;

    /**
     * @brief Handle version list
     *
     * @param secretVersionResponse secrets version list
     */
    void HandleVersionsList(const SecretGetVersionResponse &secretVersionResponse);

private:
    /**
     * @brief UI components
     */
    Ui::SecretsDetailsDialog *_ui;

    /**
     * @brief Secret AWS ARN
     */
    QString _secretArn;

    /**
     * @brief Secret ID
     */
    QString _secretId;

    /**
     * @brief Service
     */
    SecretsManagerService *_secretsManagerService{};

    /**
     * @brief Changed flag
     */
    bool _changed = false;

    /**
     * @brief Secret counter
     */
    SecretCounter _secretCounter;

    /**
     * @brief Data model
     */
    QStandardItemModel *_versionDataModel{};
};


#endif //AWSMOCK_QT_UI_SECRETS_DETAILS_DIALOG_H
