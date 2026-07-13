#pragma once

// Qt includes
#include <QDialog>
#include <QDialogButtonBox>

// Awsmock includes
#include <dto/apigateway/RestApiKeyCreateRequest.h>
#include <modules/apigateway/ApiGatewayService.h>

QT_BEGIN_NAMESPACE
namespace Ui {
    class RestApiKeyDialog;
}

QT_END_NAMESPACE

/**
 * @brief A dialog interface for managing and configuring REST API keys.
 *
 * This dialog provides functionalities for adding, editing, and removing
 * REST API keys. It facilitates users in handling authentication keys
 * used for interacting with external RESTful services. The implementation
 * ensures secure handling of sensitive information.
 */
class RestApiKeyDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param parent parent widget
     */
    explicit RestApiKeyDialog(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~RestApiKeyDialog() override;

    /**
     * @brief Handles the acceptance action for a dialog or process.
     *
     * This method is typically invoked when the user confirms an action
     * or submits data, triggering the necessary logic to process the acceptance.
     */
    void HandleAccept();

    /**
     * @brief Handles the rejection action for a dialog or process.
     *
     * This method is typically called when the user cancels or dismisses the dialog,
     * closing it without applying any changes.
     */
    void HandleReject();

private:
    /**
     * @brief Pointer to the UI components of the RestApiKeyDialog.
     *
     * This member variable is used to manage and interact with the user interface elements
     * defined in the RestApiKeyDialog. It is initialized in the constructor and cleaned up
     * in the destructor to ensure proper resource management.
     */
    Ui::RestApiKeyDialog *_ui;

    /**
     * @brief Pointer to the ApiGatewayService instance.
     *
     * This member variable is used to interact with the ApiGatewayService, which
     * provides functionalities for managing REST APIs and API keys. It is
     * initialized during the construction of the RestApiKeyDialog and ensures
     * communication with the backend services for creating, updating, and deleting
     * APIs and API keys.
     */
    ApiGatewayService *_apiGatewayService{};
};
