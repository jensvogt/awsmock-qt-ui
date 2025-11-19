//
// Created by vogje01 on 11/17/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_EditConfigDialog.h" resolved

#include <utils/EditConfigDialog.h>
#include "ui_EditConfigDialog.h"

EditConfigDialog::EditConfigDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::EditConfigDialog) {

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &EditConfigDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &EditConfigDialog::HandleReject);

    // Client settings
    _ui->regionEdit->setText(Configuration::instance().GetValue<QString>("aws.region", "eu-central-1"));
    connect(_ui->regionEdit, &QLineEdit::textChanged, this, [this]() {
        Configuration::instance().SetValue("aws.region", _ui->regionEdit->text());
        emit ConfigChangedSignal();
    });

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
        emit ConfigChangedSignal();
    });

    // FTP settings
    _ui->ftpUserEdit->setText(Configuration::instance().GetValue<QString>("ui.default-ftp-user", "none"));
    connect(_ui->ftpUserEdit, &QLineEdit::textChanged, this, [this]() {
        Configuration::instance().SetValue("ui-default-ftp-user", _ui->ftpUserEdit->text());
        emit ConfigChangedSignal();
    });
    _ui->ftpPasswordEdit->setText(Configuration::instance().GetValue<QString>("ui.default-ftp-password", "none"));
    connect(_ui->ftpPasswordEdit, &QLineEdit::textChanged, this, [this]() {
        Configuration::instance().SetValue("ui-default-ftp-password", _ui->ftpPasswordEdit->text());
        emit ConfigChangedSignal();
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
