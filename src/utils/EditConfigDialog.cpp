
#include <utils/EditConfigDialog.h>
#include "ui_EditConfigDialog.h"

EditConfigDialog::EditConfigDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::EditConfigDialog) {

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &EditConfigDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &EditConfigDialog::HandleReject);

    // Fill in combo box
    QStringList urls;
    const auto selectedUrl = Configuration::instance().GetValue<QString>("server.base-url", {});
    for (auto jsonArray = Configuration::instance().GetValue<QJsonArray>("server.base-urls", {}); const QJsonValue &url: jsonArray) {
        urls.append(url.toString());
    }
    _ui->baseUrlComboBox->addItems(urls);
    _ui->baseUrlComboBox->setCurrentText(selectedUrl);
    connect(_ui->baseUrlComboBox, &QComboBox::currentTextChanged, this, [this]() {
        Configuration::instance().SetValue("server.base-url", _ui->baseUrlComboBox->currentText());
    });

    // FTP settings
    _ui->ftpUserEdit->setText(Configuration::instance().GetValue<QString>("ui.default-ftp-user", "none"));
    connect(_ui->ftpUserEdit, &QLineEdit::textChanged, this, [this]() {
        Configuration::instance().SetValue("ui-default-ftp-user", _ui->ftpUserEdit->text());
    });
    _ui->ftpPasswordEdit->setText(Configuration::instance().GetValue<QString>("ui.default-ftp-password", "none"));
    connect(_ui->ftpPasswordEdit, &QLineEdit::textChanged, this, [this]() {
        Configuration::instance().SetValue("ui-default-ftp-password", _ui->ftpPasswordEdit->text());
    });
    _ui->ftpServerEdit->setText(Configuration::instance().GetValue<QString>("ui.default-ftp-server", "localhost"));
    connect(_ui->ftpServerEdit, &QLineEdit::textChanged, this, [this]() {
        Configuration::instance().SetValue("ui-default-ftp-server", _ui->ftpServerEdit->text());
    });
    _ui->ftpPortEdit->setText(Configuration::instance().GetValue<QString>("ui.default-ftp-port", "2121"));
    connect(_ui->ftpPortEdit, &QLineEdit::textChanged, this, [this]() {
        Configuration::instance().SetValue("ui-default-ftp-port", _ui->ftpPortEdit->text());
    });

    // AWS settings
    _ui->regionEdit->setText(Configuration::instance().GetValue<QString>("aws.region", "eu-central-1"));
    connect(_ui->regionEdit, &QLineEdit::textChanged, this, [this]() {
        Configuration::instance().SetValue("aws.region", _ui->regionEdit->text());
    });
    _ui->userEdit->setText(Configuration::instance().GetValue<QString>("aws.user", "none"));
    connect(_ui->userEdit, &QLineEdit::textChanged, this, [this]() {
        Configuration::instance().SetValue("aws.user", _ui->userEdit->text());
    });
    _ui->clientIdEdit->setText(Configuration::instance().GetValue<QString>("aws.client-id", "none"));
    connect(_ui->clientIdEdit, &QLineEdit::textChanged, this, [this]() {
        Configuration::instance().SetValue("aws.client-id", _ui->clientIdEdit->text());
    });
    _ui->accountIdEdit->setText(Configuration::instance().GetValue<QString>("aws.account-id", "none"));
    connect(_ui->accountIdEdit, &QLineEdit::textChanged, this, [this]() {
        Configuration::instance().SetValue("aws.account-id", _ui->accountIdEdit->text());
    });

    // UI settings
    const QStringList styles = {"Fusion", "Universal", "Material", "Basic", "Windows"};
    _ui->styleCombo->addItems(styles);
    _ui->styleCombo->setCurrentText(Configuration::instance().GetValue<QString>("ui.style", ""));
    connect(_ui->styleCombo, &QComboBox::currentTextChanged, this, [this]() {
        Configuration::instance().SetValue("ui.style", _ui->styleCombo->currentText());

    });
    const QStringList styleTypes = {"Dark", "Light"};
    _ui->styleTypeCombo->addItems(styleTypes);
    _ui->styleTypeCombo->setCurrentText(Configuration::instance().GetValue<QString>("ui.style-type", ""));
    connect(_ui->styleTypeCombo, &QComboBox::currentTextChanged, this, [this]() {
        Configuration::instance().SetValue("ui.style-type", _ui->styleTypeCombo->currentText());
    });

    // Default tab
    _ui->tabWidget->setCurrentIndex(0);
}

EditConfigDialog::~EditConfigDialog() {
    delete _ui;
}

void EditConfigDialog::HandleAccept() {
    accept();
}

void EditConfigDialog::HandleReject() {
    accept();
}
