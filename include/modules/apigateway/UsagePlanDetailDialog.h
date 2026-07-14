#pragma once

// Qt includes
#include <QDialog>
#include <QDialogButtonBox>

// Awsmock includes
#include <dto/apigateway/UsagePlanCreateRequest.h>
#include <dto/apigateway/UsagePlanUpdateRequest.h>
#include <dto/apigateway/model/UsagePlan.h>
#include <modules/apigateway/ApiGatewayService.h>

QT_BEGIN_NAMESPACE
namespace Ui {
    class UsagePlanDetailDialog;
}

QT_END_NAMESPACE

/**
 * @brief Dialog for adding and editing usage plans.
 *
 * Use the single-argument constructor (parent only) to create a new plan, or
 * the planId constructor to edit an existing plan.
 */
class UsagePlanDetailDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Add usage plan constructor
     *
     * @param parent parent widget
     */
    explicit UsagePlanDetailDialog(QWidget *parent = nullptr);

    /**
     * @brief Edit usage plan constructor
     *
     * @param planId ID of the usage plan to edit
     * @param parent parent widget
     */
    explicit UsagePlanDetailDialog(const QString &planId, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~UsagePlanDetailDialog() override;

    /**
     * @brief Handles the acceptance action.
     */
    void HandleAccept();

    /**
     * @brief Handles the rejection action.
     */
    void HandleReject();

    /**
     * @brief Populates the form with data received from the server.
     *
     * @param usagePlan usage plan model from server
     */
    void HandleGetUsagePlanSignal(const UsagePlan &usagePlan);

private:
    void Initialize();

    Ui::UsagePlanDetailDialog *_ui;

    ApiGatewayService *_apiGatewayService{};

    QString _planId;

    bool _editMode{false};
};
