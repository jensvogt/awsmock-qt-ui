//
// Created by vogje01 on 2/6/26.
//

#ifndef AWSMOCK_QT_UI_KMS_KEY_DIALOG_H
#define AWSMOCK_QT_UI_KMS_KEY_DIALOG_H

// Awsmock includes
#include <utils/BaseDialog.h>

// Awsmock includes
#include <utils/DateTimeUtils.h>
#include <modules/kms/KMSService.h>
#include <dto/kms/KMSGetKeyCounterResponse.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class KMSKeyDialog;
}

QT_END_NAMESPACE

class KMSKeyDialog : public BaseDialog {
    Q_OBJECT

public:
    /**
     * @brief Edit KMS key constructor
     *
     * @param keyId KMS key ID
     * @param parent parent widget
     */
    explicit KMSKeyDialog(const QString &keyId, QWidget *parent = nullptr);

    /**
     * @brief Add KMS key constructor
     *
     * @param parent parent widget
     */
    explicit KMSKeyDialog(QWidget *parent = nullptr);

    /**
     * @brief Destructure
     */
    ~KMSKeyDialog() override;

    void Initialize();

    /**
     * @brief Handle response from server call
     *
     * @param response get key counter response
     */
    void HandleGetKeyCounterSignal(const KMSGetKeyCounterResponse &response);

    /**
     * @brief Load content
     */
    void LoadContent() override {
    }

    /**
     * @brief Handle OK button callback
     */
    void HandleAccept();

    /**
     * @brief Handle CANCEL button callback
     */
    void HandleReject();

private:
    /**
     * @brief UI components
     */
    Ui::KMSKeyDialog *_ui;

    /**
     * @brief UI components
     */
    KMSService *_kmsService{};

    /**
     * @brief Actual key counter
     */
    KeyCounter _keyCounter{};

    /**
     * @brief changed flag
     */
    bool _changed = false;
};


#endif // AWSMOCK_QT_UI_KMS_KEY_DIALOG_H
