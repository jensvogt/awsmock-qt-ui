//
// Created by vogje01 on 1/11/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_S3BucketLifecycleDialog.h" resolved

#include <modules/s3/S3BucketLifecycleDialog.h>
#include "ui_S3BucketLifecycleDialog.h"

S3BucketLifecycleDialog::S3BucketLifecycleDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::S3BucketLifecycleDialog) {
    Initialize();
}

S3BucketLifecycleDialog::S3BucketLifecycleDialog(const LifecycleRule &lifecycleRule, QWidget *parent) : QDialog(parent), _ui(new Ui::S3BucketLifecycleDialog), _lifecycleRule(lifecycleRule) {
    Initialize();
}

S3BucketLifecycleDialog::~S3BucketLifecycleDialog() {
    delete _ui;
}

void S3BucketLifecycleDialog::Initialize() {

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &S3BucketLifecycleDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &S3BucketLifecycleDialog::HandleReject);

    // Setup attributes
    _ui->idEdit->setText(_lifecycleRule.id);
    _ui->prefixEdit->setText(_lifecycleRule.prefix);
}

void S3BucketLifecycleDialog::HandleAccept() {
    accept();
}


void S3BucketLifecycleDialog::HandleReject() {
    reject();
}
