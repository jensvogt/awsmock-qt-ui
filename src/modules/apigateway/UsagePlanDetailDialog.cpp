#include <modules/apigateway/UsagePlanDetailDialog.h>
#include "ui_UsagePlanDetailDialog.h"

UsagePlanDetailDialog::UsagePlanDetailDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::UsagePlanDetailDialog) {
    Initialize();
}

UsagePlanDetailDialog::UsagePlanDetailDialog(const QString &planId, QWidget *parent) : QDialog(parent), _ui(new Ui::UsagePlanDetailDialog), _planId(planId), _editMode(true) {
    Initialize();
    setWindowTitle("Edit Usage Plan");
    _apiGatewayService->GetUsagePlan(planId);
}

UsagePlanDetailDialog::~UsagePlanDetailDialog() {
    delete _ui;
}

void UsagePlanDetailDialog::Initialize() {
    _apiGatewayService = new ApiGatewayService();
    connect(_apiGatewayService, &ApiGatewayService::GetUsagePlanSignal, this, &UsagePlanDetailDialog::HandleGetUsagePlanSignal);

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &UsagePlanDetailDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &UsagePlanDetailDialog::HandleReject);
}

void UsagePlanDetailDialog::HandleGetUsagePlanSignal(const UsagePlan &usagePlan) {
    _ui->nameEdit->setText(usagePlan.name);
    _ui->descriptionEdit->setText(usagePlan.description);
    _ui->rateLimitSpin->setValue(usagePlan.throttleRateLimit);
    _ui->burstLimitSpin->setValue(usagePlan.throttleBurstLimit);
    _ui->quotaLimitSpin->setValue(usagePlan.quotaLimit);
    const int periodIndex = _ui->quotaPeriodCombo->findText(usagePlan.quotaPeriod);
    if (periodIndex >= 0) {
        _ui->quotaPeriodCombo->setCurrentIndex(periodIndex);
    }
}

void UsagePlanDetailDialog::HandleAccept() {
    if (_editMode) {
        UsagePlanUpdateRequest request;
        request.planId = _planId;
        request.name = _ui->nameEdit->text().trimmed();
        request.description = _ui->descriptionEdit->text().trimmed();
        request.throttleRateLimit = _ui->rateLimitSpin->value();
        request.throttleBurstLimit = _ui->burstLimitSpin->value();
        request.quotaLimit = _ui->quotaLimitSpin->value();
        request.quotaPeriod = _ui->quotaPeriodCombo->currentText();
        _apiGatewayService->UpdateUsagePlan(request);
    } else {
        UsagePlanCreateRequest request;
        request.name = _ui->nameEdit->text().trimmed();
        request.description = _ui->descriptionEdit->text().trimmed();
        request.throttleRateLimit = _ui->rateLimitSpin->value();
        request.throttleBurstLimit = _ui->burstLimitSpin->value();
        request.quotaLimit = _ui->quotaLimitSpin->value();
        request.quotaPeriod = _ui->quotaPeriodCombo->currentText();
        _apiGatewayService->CreateUsagePlan(request);
    }
    accept();
}

void UsagePlanDetailDialog::HandleReject() {
    reject();
}
