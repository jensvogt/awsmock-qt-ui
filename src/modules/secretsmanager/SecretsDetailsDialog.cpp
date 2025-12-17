//
// Created by vogje01 on 12/16/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SecretsDetails.h" resolved

#include <modules/secretsmanager/SecretsDetailsDialog.h>
#include "ui_SecretsDetailsDialog.h"

SecretsDetailsDialog::SecretsDetailsDialog(QString secretArn, QWidget *parent) : QDialog(parent), _ui(new Ui::SecretsDetailsDialog), _secretArn(std::move(secretArn)) {

    // Initialize service
    _secretsManagerService = new SecretsManagerService();

    // Get secret details
    _secretsManagerService->GetSecret(_secretArn);
    connect(_secretsManagerService, &SecretsManagerService::GetSecretsDetailsSignal, this, &SecretsDetailsDialog::UpdateSecret);

    // Setup UI
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &SecretsDetailsDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &SecretsDetailsDialog::HandleReject);

    // Refresh button
    _ui->refreshButton->setText(nullptr);
    _ui->refreshButton->setIcon(IconUtils::GetIcon("refresh"));

    // Pretty print
    _ui->prettyButton->setText(nullptr);
    _ui->prettyButton->setIcon(IconUtils::GetIcon("pretty"));
    connect(_ui->prettyButton, &QPushButton::toggled, this, &SecretsDetailsDialog::PrettyPrintClicked);
    // Value edit
    connect(_ui->valueEdit, &QTextEdit::textChanged, this, [this]() {
        _changed = true;
    });
}

SecretsDetailsDialog::~SecretsDetailsDialog() {
    delete _ui;
}

void SecretsDetailsDialog::HandleAccept() {
    if (_changed) {
        _secretCounter.secretString = _ui->valueEdit->toPlainText().toUtf8();
        _secretsManagerService->UpdateSecret(_secretCounter);
    }
    accept();
}

void SecretsDetailsDialog::HandleReject() {
    accept();
}

void SecretsDetailsDialog::LoadContent() {

}

void SecretsDetailsDialog::UpdateSecret(const SecretCounter &secretCounter) {
    _secretCounter = secretCounter;
    _ui->regionEdit->setText(secretCounter.region);
    _ui->nameEdit->setText(secretCounter.name);
    _ui->arnEdit->setText(secretCounter.arn);
    _ui->secretIdEdit->setText(secretCounter.secretId);
    _ui->lastRotatedEdit->setText(secretCounter.lastRotatedDate.toString("yyyy-mm-dd HH:mm"));
    _ui->nextRotationEdit->setText(secretCounter.nextRotatedDate.toString("yyyy-mm-dd HH:mm"));
    _ui->lastAccessedEdit->setText(secretCounter.lastAccessedDate.toString("yyyy-mm-dd HH:mm"));
    _ui->deleteDateEdit->setText(secretCounter.deletedDate.toString("yyyy-mm-dd HH:mm"));
    _ui->createdEdit->setText(secretCounter.created.toString("yyyy-mm-dd HH:mm"));
    _ui->modifiedEdit->setText(secretCounter.modified.toString("yyyy-mm-dd HH:mm"));
    _ui->valueEdit->setText(secretCounter.secretString);
}

void SecretsDetailsDialog::PrettyPrintClicked(const bool checked) const {
    if (checked) {
        const QByteArray body = _ui->valueEdit->toPlainText().toUtf8();
        QJsonParseError error;
        const QJsonDocument jDoc = QJsonDocument::fromJson(body, &error);
        if (error.error == QJsonParseError::NoError) {
            _ui->valueEdit->clear();
            _ui->valueEdit->setPlainText(jDoc.toJson(QJsonDocument::Indented));
        } else {
            QMessageBox::warning(nullptr, "Warning", "Invalid file, error: " + error.errorString());
        }
    } else {
        const QByteArray body = _ui->valueEdit->toPlainText().toUtf8();
        QJsonParseError error;
        const QJsonDocument jDoc = QJsonDocument::fromJson(body, &error);
        if (error.error == QJsonParseError::NoError) {
            _ui->valueEdit->clear();
            _ui->valueEdit->setPlainText(jDoc.toJson(QJsonDocument::Compact));
        } else {
            QMessageBox::warning(nullptr, "Warning", "Invalid file, error: " + error.errorString());
        }
    }
    /*
    if (!_ui->searchEdit->text().isEmpty()) {
        const QString text = _ui->searchEdit->text();
        QTextCursor cursor(_ui->valueEdit->document());
        cursor.movePosition(QTextCursor::Start);
        _ui->valueEdit->setTextCursor(cursor);
        _ui->valueEdit->find(text, QTextDocument::FindWholeWords);
    }*/
}
