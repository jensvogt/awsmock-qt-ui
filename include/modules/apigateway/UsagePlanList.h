#pragma once

// Qt includes
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMenu>
#include <QPushButton>

// Awsmock Qt includes
#include <components/ContextMenu.h>
#include <components/PageableTable.h>
#include <components/Toast.h>
#include <dto/apigateway/UsagePlanListResponse.h>
#include <modules/apigateway/ApiGatewayService.h>
#include <modules/apigateway/UsagePlanDetailDialog.h>
#include <utils/BasePage.h>
#include <utils/IconUtils.h>

/**
 * @brief Usage plan list page placed in the content pane.
 */
class UsagePlanList final : public BasePage {
    Q_OBJECT

public:
    explicit UsagePlanList(const QString &title, QWidget *parent = nullptr);
    ~UsagePlanList() override;

    void ClearContent() override {
    }

    void LoadContent() override;

    void HandleListUsagePlansSignal(const UsagePlanListResponse &response) const;

private slots:
    void ShowContextMenu(const QPoint &pos);

private:
    QString _region;
    PageableTable *_tableView;
    ApiGatewayService *_apiGatewayService;
};
