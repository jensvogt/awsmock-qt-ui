//
// Created by vogje01 on 11/27/25.
//

#include <modules/s3/S3ObjectMetadataDialog.h>
#include "ui_S3ObjectMetadataDialog.h"

S3ObjectMetadataDialog::S3ObjectMetadataDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::S3ObjectMetadataDialog) {

    Initialize();
}

S3ObjectMetadataDialog::S3ObjectMetadataDialog(const QString &key, const QString &value, QWidget *parent) : QDialog(parent), _ui(new Ui::S3ObjectMetadataDialog) {
    Initialize();
    _ui->nameEdit->setText(key);
    _ui->nameEdit->setDisabled(true);
    _ui->valueEdit->setText(value);
}

S3ObjectMetadataDialog::~S3ObjectMetadataDialog() {
    delete _ui;
}

void S3ObjectMetadataDialog::Initialize() {

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &S3ObjectMetadataDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &S3ObjectMetadataDialog::HandleReject);
}

void S3ObjectMetadataDialog::HandleAccept() {
    accept();
}

void S3ObjectMetadataDialog::HandleReject() {
    accept();
}

QString S3ObjectMetadataDialog::GetKey() const {
    return _ui->nameEdit->text();
}

QString S3ObjectMetadataDialog::GetValue() const {
    return _ui->valueEdit->text();
}
