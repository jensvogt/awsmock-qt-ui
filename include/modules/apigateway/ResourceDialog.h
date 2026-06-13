//
// Created by vogje01 on 6/13/26.
//

#pragma once

// QT includes
#include <QDialog>
#include <QDialogButtonBox>

// Awsmock includes
#include <modules/apigateway/ApiGatewayService.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class ResourceDialog;
}

QT_END_NAMESPACE

class ResourceDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param parent parent widget
     */
    explicit ResourceDialog(QWidget *parent = nullptr);

    /**
     * @brief Constructor
     *
     * @param restApi REST API resource
     * @param resourceId ID of the resource
     * @param parent parent widget
     */
    ResourceDialog(const RestApiCounter &restApi, const QString &resourceId, QWidget *parent);

    /**
     * @brief Destructor
     */
    ~ResourceDialog() override;

    /**
     * @brief Initialization
     */
    void Initialize();

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
    Ui::ResourceDialog *_ui;

    /**
     * @brief Service
     */
    ApiGatewayService *_apiGatewayService;
};
