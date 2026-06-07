//
// Created by vogje01 on 6/6/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RestApiDetailsDialog.h" resolved

#include <modules/apigateway/RestApiDetailsDialog.h>
#include "ui_RestApiDetailsDialog.h"

RestApiDetailsDialog::RestApiDetailsDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::RestApiDetailsDialog) {

    // Initialize
    Initialize();
}

RestApiDetailsDialog::RestApiDetailsDialog(const QString &name, QWidget *parent) : QDialog(parent), _ui(new Ui::RestApiDetailsDialog), _name(name) {

    // Initialize
    Initialize();

    // Name
    _ui->nameEdit->setText(_name);

    // Update
    _apiGatewayService->GetRestApi(_name);
}

void RestApiDetailsDialog::Initialize() {

    // Set up service
    _apiGatewayService = new ApiGatewayService();
    connect(_apiGatewayService, &ApiGatewayService::GetRestApiSignal, this, &RestApiDetailsDialog::HandleGet);

    // Set up UI components
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &RestApiDetailsDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &RestApiDetailsDialog::HandleReject);

    // Refresh
    _ui->refreshButton->setText(nullptr);
    _ui->refreshButton->setIcon(IconUtils::GetIcon("refresh"));
    connect(_ui->refreshButton, &QPushButton::clicked, this, [this]() {
        _apiGatewayService->GetRestApi(_name);
    });

    // Connect the status label
    connect(&EventBus::instance(), &EventBus::TimerSignal, this, [this](const QString &timerName, const qint64 elapsed) {
        if (timerName == "GetRestApi" || timerName == "UpdateRestApi") {
            _ui->statusLabel->setText("Last update: " + QDateTime::currentDateTime().toString("hh:mm:ss") + " [" + QString::number(elapsed) + "ms]");
        }
    });

    // Enabled
    connect(_ui->enabledCheck, &QCheckBox::checkStateChanged, this, [this]() {
        _changed = true;
    });
}

RestApiDetailsDialog::~RestApiDetailsDialog() {
    delete _ui;
}

void RestApiDetailsDialog::HandleAccept() {
    if (_changed && !_name.isEmpty()) {
        RestApiUpdateRequest request;
        request.restApi.name = _name;
        request.restApi.enabled = _ui->enabledCheck->isChecked();
        _apiGatewayService->UpdateRestApi(request);
    } else if (_changed) {
        RestApiCreateRequest request;
        request.name = _ui->nameEdit->text();
        request.enabled = _ui->enabledCheck->isChecked();
        request.description = _ui->descriptionEdit->toPlainText();
        _apiGatewayService->CreateRestApi(request);
    }
    accept();
}

void RestApiDetailsDialog::HandleReject() {
    reject();
}

void RestApiDetailsDialog::HandleGet(const RestApiGetResponse &restApiGetResponse) const {

    // Description
    _ui->descriptionEdit->setText(restApiGetResponse.restApiCounter.description);

    // Dates
    _ui->createdEdit->setText(DateTimeUtils::GetDateTimeFormat(restApiGetResponse.restApiCounter.created));
    _ui->modifiedEdit->setText(DateTimeUtils::GetDateTimeFormat(restApiGetResponse.restApiCounter.modified));
}
