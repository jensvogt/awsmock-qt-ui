//
// Created by vogje01 on 11/27/25.
//

#include <modules/s3/S3BucketMetadataDialog.h>
#include "ui_S3BucketMetadataDialog.h"

S3BucketMetadataDialog::S3BucketMetadataDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::S3BucketMetadataDialog) {

    Initialize();
}

S3BucketMetadataDialog::S3BucketMetadataDialog(QWidget *parent, const QString &name, const QString &value) : QDialog(parent), _ui(new Ui::S3BucketMetadataDialog) {

    Initialize();

    _ui->nameEdit->setText(name);
    _ui->nameEdit->setDisabled(true);
    _ui->valueEdit->setText(value);
}

S3BucketMetadataDialog::~S3BucketMetadataDialog() {
    delete _ui;
}

void S3BucketMetadataDialog::Initialize() {

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &S3BucketMetadataDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &S3BucketMetadataDialog::HandleReject);
}

void S3BucketMetadataDialog::HandleAccept() {
    accept();
}

void S3BucketMetadataDialog::HandleReject() {
    accept();
}

QString S3BucketMetadataDialog::GetKey() const {
    return _ui->nameEdit->text();
}

QString S3BucketMetadataDialog::GetValue() const {
    return _ui->valueEdit->text();
}
