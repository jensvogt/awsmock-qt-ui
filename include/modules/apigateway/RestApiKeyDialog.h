#pragma once

// Qt includes
#include <QDialog>
#include <QDialogButtonBox>

// Awsmock includes
#include <dto/apigateway/RestApiKeyCreateRequest.h>
#include <dto/apigateway/RestApiKeyUpdateRequest.h>
#include <dto/apigateway/model/RestApiKey.h>
#include <modules/apigateway/ApiGatewayService.h>

QT_BEGIN_NAMESPACE
namespace Ui {
    class RestApiKeyDialog;
}

QT_END_NAMESPACE

/**
 * @brief A dialog interface for managing and configuring REST API keys.
 *
 * This dialog provides functionalities for adding and editing REST API keys.
 * Use the single-argument constructor (parent only) to create a new key, or
 * the keyId constructor to edit an existing key.
 */
class RestApiKeyDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Add API key constructor
     *
     * @param parent parent widget
     */
    explicit RestApiKeyDialog(QWidget *parent = nullptr);

    /**
     * @brief Edit API key constructor
     *
     * @param keyId ID of the API key to edit
     * @param parent parent widget
     */
    explicit RestApiKeyDialog(const QString &keyId, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~RestApiKeyDialog() override;

    /**
     * @brief Handles the acceptance action for a dialog or process.
     */
    void HandleAccept();

    /**
     * @brief Handles the rejection action for a dialog or process.
     */
    void HandleReject();

    /**
     * @brief Populates the form with data received from the server.
     *
     * @param apiKey API key model from server
     */
    void HandleGetApiKeySignal(const RestApiKey &apiKey);

private:
    void Initialize();

    Ui::RestApiKeyDialog *_ui;

    ApiGatewayService *_apiGatewayService{};

    QString _keyId;

    bool _editMode{false};
};
