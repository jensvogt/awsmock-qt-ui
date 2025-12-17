//
// Created by vogje01 on 12/17/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SecretAddDialog.h" resolved

#include <modules/secretsmanager/SecretAddDialog.h>
#include "ui_SecretAddDialog.h"

SecretAddDialog::SecretAddDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::SecretAddDialog) {

    // Initialize services
    _secretsManagerService = new SecretsManagerService();
    _kmsService = new KMSService();
    connect(_kmsService, &KMSService::ListKeysSignal, this, [this](const KMSKeyListResponse &kmsResponse) {
        QStringList keys;
        for (const auto &key: kmsResponse.keyCounters) {
            keys.append(key.arn);
        }
        _ui->kmsKeyCombo->addItems(keys);
    });

    // Setup UI
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &SecretAddDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &SecretAddDialog::HandleReject);

    // Fill in KMS key combo box
    _kmsService->ListKmsKeys();
}

SecretAddDialog::~SecretAddDialog() {
    delete _ui;
}

void SecretAddDialog::HandleAccept() {
    _secretCounter.name = _ui->nameEdit->text();
    _secretCounter.secretString = _ui->secretEdit->toPlainText();
    _secretsManagerService->CreateSecret(_secretCounter);
    accept();
}

void SecretAddDialog::HandleReject() {
    accept();
}
