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
#include <dto/apigateway/RestApiKeyListResponse.h>
#include <modules/apigateway/ApiGatewayService.h>
#include <modules/apigateway/RestApiKeyDialog.h>
#include <utils/BasePage.h>
#include <utils/IconUtils.h>

/**
 * @brief REST API key list page placed in the content pane.
 */
class RestApiKeyList final : public BasePage {
    Q_OBJECT

public:
    explicit RestApiKeyList(const QString &title, QWidget *parent = nullptr);
    ~RestApiKeyList() override;

    void ClearContent() override {
    }

    void LoadContent() override;

    void HandleListApiKeysSignal(const RestApiKeyListResponse &response) const;

private slots:
    void ShowContextMenu(const QPoint &pos);

    /**
     * @brief Delete the currently selected API key
     */
    void DeleteSelected();

private:
    QString _region;
    PageableTable *_tableView;
    ApiGatewayService *_apiGatewayService;
};
