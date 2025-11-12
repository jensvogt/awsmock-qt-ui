//
// Created by vogje01 on 11/12/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_S3BucketEditDialog.h" resolved

#include <modules/s3/S3BucketEditDialog.h>
#include "ui_S3BucketEditDialog.h"

S3BucketEditDialog::S3BucketEditDialog(const QString &bucketName, QWidget *parent) : QDialog(parent), _ui(new Ui::S3BucketEditDialog) {

    _s3Service = new S3Service();

    _s3Service->GetBucketDetails(bucketName);
    connect(_s3Service, &S3Service::GetBucketDetailsSignal, this, &S3BucketEditDialog::UpdateBucket);

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &S3BucketEditDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &S3BucketEditDialog::HandleReject);

    _ui->tabWidget->setCurrentIndex(0);
}

S3BucketEditDialog::~S3BucketEditDialog() {
    delete _ui;
}

void S3BucketEditDialog::UpdateBucket(const S3GetBucketDetailsResponse &bucketGetResponse) const {
    _ui->regionEdit->setText(bucketGetResponse.region);
    _ui->nameEdit->setText(bucketGetResponse.bucketName);
    _ui->arnEdit->setText(bucketGetResponse.bucketArn);
    _ui->ownerEdit->setText(bucketGetResponse.owner);
    _ui->keysEdit->setText(QString::number(bucketGetResponse.objectCount));
    _ui->sizeEdit->setText(QString::number(bucketGetResponse.size));
    _ui->createdEdit->setText(bucketGetResponse.created.toString("yyyy-MM-dd hh:mm:ss"));
    _ui->modifiedEdit->setText(bucketGetResponse.modified.toString("yyyy-MM-dd hh:mm:ss"));
}

void S3BucketEditDialog::HandleAccept() {
    if (this->_changed) {
        //_s3Service->UpdateBucket(_application);
    }
    accept();
}


void S3BucketEditDialog::HandleReject() {
    accept();
}
