#include <modules/apigateway/RestApiKeyDialog.h>
#include "ui_RestApiKeyDialog.h"

RestApiKeyDialog::RestApiKeyDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::RestApiKeyDialog) {
    Initialize();
}

RestApiKeyDialog::RestApiKeyDialog(const QString &keyId, QWidget *parent) : QDialog(parent), _ui(new Ui::RestApiKeyDialog), _keyId(keyId), _editMode(true) {
    Initialize();
    setWindowTitle("Edit API Key");
    _apiGatewayService->GetApiKey(keyId);
}

RestApiKeyDialog::~RestApiKeyDialog() {
    delete _ui;
}

void RestApiKeyDialog::Initialize() {
    _apiGatewayService = new ApiGatewayService();
    connect(_apiGatewayService, &ApiGatewayService::GetApiKeySignal, this, &RestApiKeyDialog::HandleGetApiKeySignal);

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &RestApiKeyDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &RestApiKeyDialog::HandleReject);
}

void RestApiKeyDialog::HandleGetApiKeySignal(const RestApiKey &apiKey) {
    _ui->nameEdit->setText(apiKey.name);
    _ui->descriptionEdit->setText(apiKey.description);
    _ui->valueEdit->setText(apiKey.value);
    _ui->enabledCheck->setChecked(apiKey.enabled);
}

void RestApiKeyDialog::HandleAccept() {
    if (_editMode) {
        RestApiKeyUpdateRequest request;
        request.keyId = _keyId;
        request.name = _ui->nameEdit->text().trimmed();
        request.description = _ui->descriptionEdit->text().trimmed();
        request.value = _ui->valueEdit->text().trimmed();
        request.enabled = _ui->enabledCheck->isChecked();
        _apiGatewayService->UpdateApiKey(request);
    } else {
        RestApiKeyCreateRequest request;
        request.name = _ui->nameEdit->text().trimmed();
        request.description = _ui->descriptionEdit->text().trimmed();
        request.value = _ui->valueEdit->text().trimmed();
        request.enabled = _ui->enabledCheck->isChecked();
        _apiGatewayService->CreateApiKey(request);
    }
    accept();
}

void RestApiKeyDialog::HandleReject() {
    reject();
}
