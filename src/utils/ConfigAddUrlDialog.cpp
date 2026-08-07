//
// Created by jensv on 19/04/2026.
//

#include <utils/ConfigAddUrlDialog.h>
#include "ui_ConfigAddUrlDialog.h"

ConfigAddUrlDialog::ConfigAddUrlDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::ConfigAddUrlDialog) {

    // Setup UI components
    _ui->setupUi(this);

    // Connect button box
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &ConfigAddUrlDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &ConfigAddUrlDialog::HandleReject);

    // Connect text fields
    connect(_ui->hostnameEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        _host = text;
    });
    connect(_ui->portEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        _port = text.toInt();
    });
}

ConfigAddUrlDialog::~ConfigAddUrlDialog() {
    delete _ui;
}

void ConfigAddUrlDialog::HandleAccept() {
    if (_host.isEmpty()) {
        QMessageBox::warning(this, "Input error", "Host name cannot be empty!");
    } else if (_port <= 0) {
        QMessageBox::warning(this, "Input error", "Port must be greater than 0!");
    } else {
        accept();
    }
}

void ConfigAddUrlDialog::HandleReject() {
    reject();
}
