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

    // Log text edits are read-only, but their content (or current selection) can still be copied
    _ui->requestEdit->setReadOnly(true);
    _ui->outputEdit->setReadOnly(true);
    _ui->logsEdit->setReadOnly(true);

    _ui->requestCopyButton->setText(nullptr);
    _ui->requestCopyButton->setIcon(IconUtils::GetIcon("clipboard"));
    _ui->requestCopyButton->setToolTip("Copy the request to the clipboard");
    connect(_ui->requestCopyButton, &QPushButton::clicked, this, [this]() {
        CopyToClipboard(_ui->requestEdit);
    });

    _ui->outputCopyButton->setText(nullptr);
    _ui->outputCopyButton->setIcon(IconUtils::GetIcon("clipboard"));
    _ui->outputCopyButton->setToolTip("Copy the output to the clipboard");
    connect(_ui->outputCopyButton, &QPushButton::clicked, this, [this]() {
        CopyToClipboard(_ui->outputEdit);
    });

    _ui->logsCopyButton->setText(nullptr);
    _ui->logsCopyButton->setIcon(IconUtils::GetIcon("clipboard"));
    _ui->logsCopyButton->setToolTip("Copy the logs to the clipboard");
    connect(_ui->logsCopyButton, &QPushButton::clicked, this, [this]() {
        CopyToClipboard(_ui->logsEdit);
    });

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
    _ui->durationEdit->setText(QString::number(lambdaLogsResult.lambdaLogCounter.duration) + " ms");
    _ui->instanceIdEdit->setText(lambdaLogsResult.lambdaLogCounter.instanceId);
    _ui->containerIdEdit->setText(lambdaLogsResult.lambdaLogCounter.containerId);
    _ui->httpStatusEdit->setText(lambdaLogsResult.lambdaLogCounter.httpStatusCode);

    // Tabs
    _ui->requestEdit->setText(lambdaLogsResult.lambdaLogCounter.requestBody);
    _ui->outputEdit->setText(lambdaLogsResult.lambdaLogCounter.responseBody);
    _ui->logsEdit->setText(lambdaLogsResult.lambdaLogCounter.logMessages);
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

void LambdaLogsDialog::CopyToClipboard(const QTextEdit *textEdit) {
    const QTextCursor cursor = textEdit->textCursor();
    const QString text = cursor.hasSelection() ? cursor.selectedText().replace(QChar(0x2029), '\n') : textEdit->toPlainText();
    QGuiApplication::clipboard()->setText(text);
}
