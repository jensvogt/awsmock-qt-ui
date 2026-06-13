//
// Created by vogje01 on 6/6/26.
//

#pragma once

// C*+ includes
#include <utility>

// Qt includes
#include <QDialog>

// Awsmock includes
#include <components/ContextMenu.h>
#include <dto/apigateway/RestApiCreateRequest.h>
#include <modules/apigateway/ApiGatewayService.h>
#include <modules/apigateway/ResourceDialog.h>
#include <utils/BaseDialog.h>
#include <utils/IconUtils.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class RestApiDetailsDialog;
}

QT_END_NAMESPACE

class RestApiDetailsDialog : public BaseDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param parent parent widget
     */
    explicit RestApiDetailsDialog(QWidget *parent = nullptr);

    /**
     * @brief Constructor
     *
     * @param name name of the REST API
     * @param parent parent widget
     */
    explicit RestApiDetailsDialog(QString name, QWidget *parent = nullptr);

    /**
     * @brief Initialization
     */
    void Initialize();

    /**
     * @brief Destructor
     */
    ~RestApiDetailsDialog() override;

    /**
     * @brief Handle the OK button clicks
     */
    void HandleAccept();

    /**
     * @brief Handle the CANCEL button clicks
     */
    void HandleReject();

    /**
     * @brief Handle the return from a get details call
     *
     * @param restApiGetResponse server response
     */
    void HandleGet(const RestApiGetResponse &restApiGetResponse);

    /**
     * @brief Show the context menu
     *
     * @param pos table position
     */
    void ShowResourcesContextMenu(const QPoint &pos);

    void UpdateResource() const;

    void CreateResource() const;

    /**
     * @brief Load content
     */
    void LoadContent() override {
    }

private:
    /**
     * @brief UI components
     */
    Ui::RestApiDetailsDialog *_ui;

    /**
     * @brief API gateway REST service
     */
    ApiGatewayService *_apiGatewayService{};

    /**
     * @brief Name of the REST api
     */
    QString _name;

    /**
     * @brief Changed flag
     */
    bool _changed = false;

    /**
     * @brief REST API
     */
    RestApiCounter _restApi;
};
