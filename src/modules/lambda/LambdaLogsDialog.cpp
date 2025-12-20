//
// Created by vogje01 on 11/27/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LambdaLOgsDialog.h" resolved

#include <modules/lambda/LambdaLogsDialog.h>
#include "ui_LambdaLogsDialog.h"

LambdaLogsDialog::LambdaLogsDialog(const QString &oid, QWidget *parent) : QDialog(parent), _ui(new Ui::LambdaLogsDialog) {

    // Lambda REST service
    _lambdaService = new LambdaService();
    connect(_lambdaService, &LambdaService::GetLambdaResultSignal, this, &LambdaLogsDialog::HandleGetResult);

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &LambdaLogsDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &LambdaLogsDialog::HandleReject);

    // Pretty print
    _ui->requestPrettyButton->setText(nullptr);
    _ui->requestPrettyButton->setIcon(IconUtils::GetIcon("pretty"));
    connect(_ui->requestPrettyButton, &QPushButton::toggled, this, &LambdaLogsDialog::HandleRequestPrettyButton);

    // Get lambda Logs
    _lambdaService->GetLambdaResult(oid);
}

LambdaLogsDialog::~LambdaLogsDialog() {
    delete _ui;
}

void LambdaLogsDialog::HandleAccept() {
    accept();
}

void LambdaLogsDialog::HandleReject() {
    accept();
}

void LambdaLogsDialog::HandleGetResult(const LambdaGetResultsResponse &lambdaLogsResult) const {

    // Header
    _ui->regionEdit->setText(lambdaLogsResult.lambdaLogCounter.region);
    _ui->functionEdit->setText(lambdaLogsResult.lambdaLogCounter.lambdaName);
    _ui->timestampEdit->setText(DateTimeUtils::GetDateTimeFormat(lambdaLogsResult.lambdaLogCounter.timestamp));

    // Tabs
    _ui->requestEdit->setText(lambdaLogsResult.lambdaLogCounter.requestBody);
    _ui->outputEdit->setText(lambdaLogsResult.lambdaLogCounter.responseBody);
    _ui->logsEdit->setText(StringUtils::CleanupLogString(lambdaLogsResult.lambdaLogCounter.logMessages));
}

void LambdaLogsDialog::HandleRequestPrettyButton(const bool checked) const {
    if (checked) {
        const QByteArray body = _ui->requestEdit->toPlainText().toUtf8();
        const QJsonDocument jDoc = QJsonDocument::fromJson(body);
        _ui->requestEdit->clear();
        _ui->requestEdit->setPlainText(jDoc.toJson(QJsonDocument::Indented));
    } else {
        const QByteArray body = _ui->requestEdit->toPlainText().toUtf8();
        const QJsonDocument jDoc = QJsonDocument::fromJson(body);
        _ui->requestEdit->clear();
        _ui->requestEdit->setPlainText(jDoc.toJson(QJsonDocument::Compact));
    }
    _ui->requestEdit->updateGeometry();
    QTextCursor cursor = _ui->requestEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    _ui->requestEdit->setTextCursor(cursor);
    _ui->requestEdit->ensureCursorVisible();
}
