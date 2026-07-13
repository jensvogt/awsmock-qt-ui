#pragma once

// Qt includes
#include <QDialog>
#include <QDialogButtonBox>

// Awsmock includes
#include <modules/apigateway/ApiGatewayService.h>

QT_BEGIN_NAMESPACE
namespace Ui {
    class RestApiKeyDialog;
}
QT_END_NAMESPACE

class RestApiKeyDialog : public QDialog {
    Q_OBJECT

public:
    explicit RestApiKeyDialog(QWidget *parent = nullptr);
    ~RestApiKeyDialog() override;

    void HandleAccept();
    void HandleReject();

private:
    Ui::RestApiKeyDialog *_ui;
    ApiGatewayService *_apiGatewayService{};
};
