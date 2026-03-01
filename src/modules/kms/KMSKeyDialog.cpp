//
// Created by vogje01 on 2/6/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_KMSKeyEditDIalog.h" resolved

#include <modules/kms/KMSKeyDialog.h>
#include "ui_KMSKeyDialog.h"
#include "dto/kms/Origin.h"

KMSKeyDialog::KMSKeyDialog(const QString &keyId, QWidget *parent) : BaseDialog(parent), _ui(new Ui::KMSKeyDialog) {

    // Initialize
    Initialize();

    // Get the key from the server
    _kmsService->GetKeyCounter(keyId);
}

KMSKeyDialog::KMSKeyDialog(QWidget *parent) : BaseDialog(parent), _ui(new Ui::KMSKeyDialog) {

    // Initialize
    Initialize();

    // Enable ID
    _ui->idLabel->setText("Custom ID:");
    _ui->idEdit->setDisabled(false);
}

KMSKeyDialog::~KMSKeyDialog() {
    delete _ui;
}

void KMSKeyDialog::Initialize() {
    _kmsService = new KMSService();
    connect(_kmsService, &KMSService::GetKeyCounterSignal, this, &KMSKeyDialog::HandleGetKeyCounterSignal);

    // Setup UI components
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &KMSKeyDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &KMSKeyDialog::HandleReject);

    // Combo boxes
    _ui->keySpecCombo->addItems(GetKeySpecNames());
    _ui->keyStateCombo->addItems(GetKeyStateNames());
    _ui->keyUsageCombo->addItems(GetKeyUsageNames());
    _ui->originCombo->addItems(GetOriginNames());

    // Origin
    connect(_ui->originCombo, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        _changed = true;
        _keyCounter.origin = OriginFromString(text);
    });

    // State
    connect(_ui->keyStateCombo, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        _changed = true;
        _keyCounter.keyState = KeyStateFromString(text);
    });

    // Enable description
    _ui->descriptionEdit->setDisabled(false);
    connect(_ui->descriptionEdit, &QTextEdit::textChanged, this, [this]() {
        _keyCounter.description = _ui->descriptionEdit->toPlainText();
        _changed = true;
    });
}

void KMSKeyDialog::HandleGetKeyCounterSignal(const KMSGetKeyCounterResponse &response) {

    _keyCounter = response.keyCounter;

    _ui->idEdit->setText(_keyCounter.keyId);
    _ui->arnEdit->setText(_keyCounter.arn);
    _ui->arnEdit->setText(_keyCounter.arn);
    _ui->descriptionEdit->setText(_keyCounter.description);
    _ui->createdEdit->setText(DateTimeUtils::GetDateTimeFormat(_keyCounter.created));
    _ui->modifiedEdit->setText(DateTimeUtils::GetDateTimeFormat(_keyCounter.modified));

    // Combo boxes
    _ui->keySpecCombo->setCurrentText(KeySpecToString(_keyCounter.keySpec));
    _ui->keyStateCombo->setCurrentText(KeyStateToString(_keyCounter.keyState));
    _ui->keyUsageCombo->setCurrentText(KeyUsageToString(_keyCounter.keyUsage));
    _ui->originCombo->setCurrentText(OriginToString(_keyCounter.origin));
}

void KMSKeyDialog::HandleAccept() {
    if (_changed) {
        KMSUpdateKeyCounterRequest request;
        request.keyCounter = _keyCounter;
        _kmsService->UpdateKeyCounter(request);
    }
    accept();
}


void KMSKeyDialog::HandleReject() {
    accept();
}
