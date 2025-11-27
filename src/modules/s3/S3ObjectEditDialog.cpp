//
// Created by vogje01 on 11/24/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_S3ObjectEditDialog.h" resolved

#include <modules/s3/S3ObjectEditDialog.h>
#include "ui_S3ObjectEditDialog.h"

S3ObjectEditDialog::S3ObjectEditDialog(const QString &objectId, QWidget *parent) : BaseDialog(parent),
                                                                                   _ui(new Ui::S3ObjectEditDialog), _objectId(objectId) {
    _s3Service = new S3Service();

    _s3Service->GetObjectDetails(objectId);
    connect(_s3Service, &S3Service::GetObjectDetailsSignal, this, &S3ObjectEditDialog::UpdateObject);

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &S3ObjectEditDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &S3ObjectEditDialog::HandleReject);

    // Refresh button
    _ui->refreshButton->setText(nullptr);
    _ui->refreshButton->setIcon(IconUtils::GetIcon("refresh"));
    connect(_ui->refreshButton, &QAbstractButton::clicked, this, [this, objectId]() {
        _s3Service->GetObjectDetails(objectId);
    });

    // Set tab width
    const QFontMetrics fm(_ui->bodyTextEdit->font());
    const int tabWidth = fm.horizontalAdvance(' ') * 2;
    _ui->bodyTextEdit->setTabStopDistance(tabWidth);

    // Metadata table
    const QStringList headers = QStringList() = {tr("Key"), tr("Value")};

    _ui->metadataTable->setColumnCount(static_cast<int>(headers.count()));
    _ui->metadataTable->setShowGrid(true);
    _ui->metadataTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->metadataTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->metadataTable->setHorizontalHeaderLabels(headers);
    _ui->metadataTable->setSortingEnabled(true);
    _ui->metadataTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->metadataTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    _ui->metadataTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    // Body refresh button
    _ui->bodyRefreshButton->setText(nullptr);
    _ui->bodyRefreshButton->setIcon(IconUtils::GetIcon("refresh"));

    // Metadata add button
    _ui->metadataAddButton->setText(nullptr);
    _ui->metadataAddButton->setIcon(IconUtils::GetIcon("add"));

    // Set default tab
    _ui->tabWidget->setCurrentIndex(0);
}

S3ObjectEditDialog::~S3ObjectEditDialog() {
    delete _ui;
}

void S3ObjectEditDialog::HandleAccept() {
    accept();
}

void S3ObjectEditDialog::HandleReject() {
    accept();
}

void S3ObjectEditDialog::UpdateObject(const S3GetObjectDetailsResponse &objectDetailsResponse) const {
    _ui->regionEdit->setText(objectDetailsResponse.region);
    _ui->bucketEdit->setText(objectDetailsResponse.bucketName);
    _ui->keyEdit->setText(objectDetailsResponse.key);
    _ui->contentTypeEdit->setText(objectDetailsResponse.contentType);
    _ui->sizeEdit->setText(QString::number(objectDetailsResponse.size));
    _ui->createdEdit->setText(objectDetailsResponse.created.toString("yyyy-MM-dd hh:mm:ss"));
    _ui->modifiedEdit->setText(objectDetailsResponse.modified.toString("yyyy-MM-dd hh:mm:ss"));
    _ui->bodyTextEdit->setPlainText(objectDetailsResponse.body);

    // Meta data
    if (!objectDetailsResponse.metadata.empty()) {
        const int selectedRow = _ui->metadataTable->selectionModel()->currentIndex().row();
        _ui->metadataTable->setRowCount(0);
        _ui->metadataTable->setSortingEnabled(false);
        int r = 0, c = 0;
        for (const auto &metadataKey: objectDetailsResponse.metadata.keys()) {
            _ui->metadataTable->insertRow(r);
            SetColumn(_ui->metadataTable, r, c++, metadataKey);
            SetColumn(_ui->metadataTable, r, c, objectDetailsResponse.metadata[metadataKey]);
            r++;
            c = 0;
        }
        _ui->metadataTable->setRowCount(static_cast<int>(objectDetailsResponse.metadata.count()));
        _ui->metadataTable->setSortingEnabled(true);
        _ui->metadataTable->sortItems(_metadataSortColumn, _metadataSortOrder);
        _ui->metadataTable->selectRow(selectedRow);
    }
}
