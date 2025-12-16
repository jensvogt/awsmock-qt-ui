//
// Created by vogje01 on 11/12/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_S3BucketEditDialog.h" resolved

#include <modules/s3/S3BucketEditDialog.h>
#include "ui_S3BucketEditDialog.h"

S3BucketEditDialog::S3BucketEditDialog(const QString &bucketName, QWidget *parent) : BaseDialog(parent), _ui(new Ui::S3BucketEditDialog) {

    _s3Service = new S3Service();

    _s3Service->GetBucketDetails(bucketName);
    connect(_s3Service, &S3Service::GetBucketDetailsSignal, this, &S3BucketEditDialog::UpdateBucket);

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &S3BucketEditDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &S3BucketEditDialog::HandleReject);

    // Setup tabs
    SetupDefaultMetadataTab();

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

    for (auto it = bucketGetResponse.defaultMetadata.cbegin(); it != bucketGetResponse.defaultMetadata.cend(); ++it) {
        const int row = _dataModel->rowCount();
        SetColumn(_dataModel, row, 0, it.key());
        SetColumn(_dataModel, row, 1, bucketGetResponse.defaultMetadata[it.key()]);
    }
}

void S3BucketEditDialog::SetupDefaultMetadataTab() {

    const QStringList headers = QStringList() = {
                                    tr("Key"), tr("Value")
                                };

    // Table
    _dataModel = new QStandardItemModel(this);
    _dataModel->setHorizontalHeaderLabels(headers);
    _dataModel->setColumnCount(static_cast<int>(headers.count()));
    _ui->defaultMetadataTable->setModel(_dataModel);

    _ui->defaultMetadataTable->setShowGrid(true);
    _ui->defaultMetadataTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->defaultMetadataTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->defaultMetadataTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->defaultMetadataTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents); // name
    _ui->defaultMetadataTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch); //status

    // Enable sorting, default on and sorted ascending by name
    _ui->defaultMetadataTable->setSortingEnabled(true);
    _ui->defaultMetadataTable->sortByColumn(0, Qt::AscendingOrder);

    _ui->defaultMetadataAddButton->setText(nullptr);
    _ui->defaultMetadataAddButton->setIcon(IconUtils::GetIcon("add"));
    connect(_ui->defaultMetadataAddButton, &QAbstractButton::clicked, this, [this]() {
        S3ObjectMetadataDialog metadataAdd(this, true);
        metadataAdd.exec();
        const int row = _dataModel->rowCount();
        SetColumn(_dataModel, row, 0, metadataAdd.GetKey());
        SetColumn(_dataModel, row, 1, metadataAdd.GetValue());
        this->_changed = true;
    });
}

void S3BucketEditDialog::HandleAccept() {
    if (this->_changed) {
        QMap<QString, QString> defaultMetadata;
        for (int i = 0; i < _dataModel->rowCount(); i++) {
            QModelIndex col1 = _dataModel->index(i, 0);
            QModelIndex col2 = _dataModel->index(i, 1);
            defaultMetadata[_dataModel->data(col1).toString()] = _dataModel->data(col2).toString();
        }
        _s3Service->UpdateBucket(_ui->nameEdit->text(), defaultMetadata);
    }
    accept();
}


void S3BucketEditDialog::HandleReject() {
    accept();
}
