#include <modules/apigateway/RestApiKeyDialog.h>
#include "ui_RestApiKeyDialog.h"

RestApiKeyDialog::RestApiKeyDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::RestApiKeyDialog) {
    _apiGatewayService = new ApiGatewayService();

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &RestApiKeyDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &RestApiKeyDialog::HandleReject);
}

RestApiKeyDialog::~RestApiKeyDialog() {
    delete _ui;
}

void RestApiKeyDialog::HandleAccept() {
    RestApiKeyCreateRequest request;
    request.name = _ui->nameEdit->text().trimmed();
    request.description = _ui->descriptionEdit->text().trimmed();
    request.value = _ui->valueEdit->text().trimmed();
    request.enabled = _ui->enabledCheck->isChecked();
    _apiGatewayService->CreateApiKey(request);
    accept();
}

void RestApiKeyDialog::HandleReject() {
    reject();
}
