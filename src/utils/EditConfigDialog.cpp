
#include <utils/EditConfigDialog.h>
#include "ui_EditConfigDialog.h"

EditConfigDialog::EditConfigDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::EditConfigDialog) {
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &EditConfigDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &EditConfigDialog::HandleReject);

    // Server setting
    QStringList baseUrls;
    for (auto jsonArray = Configuration::instance().GetValue<QJsonArray>("server.base-urls", {}); const QJsonValue &url: jsonArray) {
        baseUrls.append(url.toString());
    }
    const auto selectedBaseUrl = Configuration::instance().GetValue<QString>("server.base-url", {});
    _ui->baseUrlComboBox->addItems(baseUrls);
    _ui->baseUrlComboBox->setCurrentText(selectedBaseUrl);
    connect(_ui->baseUrlComboBox, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        Configuration::instance().SetValue("server.base-url", text);
    });

    QStringList websocketUrls;
    for (auto jsonArray = Configuration::instance().GetValue<QJsonArray>("server.websocket-urls", {}); const QJsonValue &url: jsonArray) {
        websocketUrls.append(url.toString());
    }
    const auto selectedWebsocketUrl = Configuration::instance().GetValue<QString>("server.websocket-url", {});
    _ui->websocketUrlCombo->addItems(websocketUrls);
    _ui->websocketUrlCombo->setCurrentText(selectedBaseUrl);
    connect(_ui->websocketUrlCombo, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        Configuration::instance().SetValue("server.websocket-url", text);
    });

    // FTP settings
    _ui->ftpUserEdit->setText(Configuration::instance().GetValue<QString>("ui.default-ftp-user", "none"));
    connect(_ui->ftpUserEdit, &QLineEdit::textChanged, this, [this]() {
        Configuration::instance().SetValue("ui.default-ftp-user", _ui->ftpUserEdit->text());
    });
    _ui->ftpPasswordEdit->setText(Configuration::instance().GetValue<QString>("ui.default-ftp-password", "none"));
    connect(_ui->ftpPasswordEdit, &QLineEdit::textChanged, this, [this]() {
        Configuration::instance().SetValue("ui.default-ftp-password", _ui->ftpPasswordEdit->text());
    });
    _ui->ftpServerEdit->setText(Configuration::instance().GetValue<QString>("ui.default-ftp-server", "localhost"));
    connect(_ui->ftpServerEdit, &QLineEdit::textChanged, this, [this]() {
        Configuration::instance().SetValue("ui.default-ftp-server", _ui->ftpServerEdit->text());
    });
    _ui->ftpPortEdit->setText(Configuration::instance().GetValue<QString>("ui.default-ftp-port", "2121"));
    connect(_ui->ftpPortEdit, &QLineEdit::textChanged, this, [this]() {
        Configuration::instance().SetValue("ui.default-ftp-port", _ui->ftpPortEdit->text());
    });
    QStringList protocols = {"FTP", "SFTP"};
    _ui->ftpProtocolCombo->addItems(protocols);
    _ui->ftpProtocolCombo->setCurrentText(Configuration::instance().GetValue<QString>("ui.default-ftp-protocol"));
    connect(_ui->ftpProtocolCombo, &QComboBox::currentIndexChanged, this, [protocols](const int index) {
        Configuration::instance().SetValue<QString>("ui.default-ftp-protocol", protocols.at(index));
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
    for (auto jArray = Configuration::instance().GetValue<QJsonArray>("ui.locales"); const auto &locale: jArray) {
        _ui->localeCombo->addItem(locale.toString(), locale.toString());
    }
    const int index = _ui->localeCombo->findData(Configuration::instance().GetValue<QString>("ui.default-locale", ""));
    _ui->localeCombo->setCurrentIndex(index);
    connect(_ui->localeCombo, &QComboBox::currentTextChanged, this, [this]() {
        Configuration::instance().SetValue("ui.default-locale", _ui->localeCombo->currentText());
    });
    _ui->applicationLogLimitEdit->setText(QString::number(Configuration::instance().GetValue<long>("ui.application-log-limit", 1000)));
    connect(_ui->applicationLogLimitEdit, &QLineEdit::textChanged, this, [this]() {
        Configuration::instance().SetValue<long>("ui.application-log-limit", _ui->applicationLogLimitEdit->text().toLong());
    });

    const QFont font = QApplication::font();
    const int fontSize = font.pointSize();

    const QStringList fontSizes = {"8", "9", "10", "11", "12", "13", "14"};
    _ui->fontSizeCombo->addItems(fontSizes);
    _ui->fontSizeCombo->setCurrentText(QString::number(fontSize));
    connect(_ui->fontSizeCombo, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        qApp->setStyleSheet("QWidget { font-size: " + text + "pt; background-color: #2b2b2b; color: #dcdcdc;}");
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
