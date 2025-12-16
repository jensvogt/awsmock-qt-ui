//
// Created by vogje01 on 12/13/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_S3ObjectAddDIalog.h" resolved

#include <modules/s3/S3ObjectAddDialog.h>
#include "ui_S3ObjectAddDialog.h"

S3ObjectAddDialog::S3ObjectAddDialog(const S3GetBucketDetailsResponse &bucket, QWidget *parent) : BaseDialog(parent), _ui(new Ui::S3ObjectAddDialog), _bucket(bucket) {
    // S3 REST service
    _s3Service = new S3Service();

    // Setup UI
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &S3ObjectAddDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &S3ObjectAddDialog::HandleReject);

    // Connect browse button
    _ui->fileBrowseButton->setToolTip("Search for the file");
    _ui->fileBrowseButton->setIcon(IconUtils::GetIcon("search"));
    connect(_ui->fileBrowseButton, &QPushButton::clicked, this, &S3ObjectAddDialog::BrowseSourceFile);

    // Metadata
    _ui->tabWidget->setCurrentIndex(0);
    _ui->tabWidget->removeTab(1);
    _ui->metadataAddButton->setText(nullptr);
    _ui->metadataAddButton->setIcon(IconUtils::GetIcon("add"));
    connect(_ui->metadataAddButton, &QPushButton::clicked, this, [this]() {
        S3ObjectMetadataDialog dialog;
        dialog.exec();
        const int row = _dataModel->rowCount();
        SetColumn(_dataModel, row, 0, dialog.GetKey());
        SetColumn(_dataModel, row, 1, dialog.GetValue());
        _metadata[dialog.GetKey()] = dialog.GetValue();
    });

    // Table
    const QStringList headers = QStringList() = {tr("Key"), tr("Value")};

    _dataModel = new QStandardItemModel(this);
    _dataModel->setHorizontalHeaderLabels(headers);
    _dataModel->setColumnCount(static_cast<int>(headers.count()));
    _ui->metadataTable->setModel(_dataModel);

    _ui->metadataTable->setShowGrid(true);
    _ui->metadataTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->metadataTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->metadataTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->metadataTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents); // name
    _ui->metadataTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch); //status

    // Fill in metadata table
    int r = 0;
    for (const auto &key: bucket.defaultMetadata.keys()) {
        SetColumn(_dataModel, r, 0, key);
        SetColumn(_dataModel, r, 1, bucket.defaultMetadata[key]);
        r++;
    }

    // Copy metadata
    _metadata = _bucket.defaultMetadata;
}

S3ObjectAddDialog::~S3ObjectAddDialog() {
    delete _ui;
}

void S3ObjectAddDialog::BrowseSourceFile() {
    // Create a QFileDialog set to select existing files
    const auto filter = "All Files (*.*)";
    const auto defaultDir = Configuration::instance().GetValue<QString>(
        "ui.default-directory.S3ObjectAddDialog", "/usr/local/awsmock-qt-ui");

    if (const QString filePath = QFileDialog::getOpenFileName(nullptr, "Open source file", defaultDir, filter); !
        filePath.isEmpty()) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(nullptr, "Error", "Could not open file:" + filePath);
            return;
        }
        _fileInfo = QFileInfo(file.fileName());
        _ui->fileEdit->setText(file.fileName());
        _ui->s3KeyEdit->setText(_fileInfo.baseName() + "." + _fileInfo.suffix());
        Configuration::instance().SetValue<QString>("ui.default-directory.S3ObjectAddDialog", _fileInfo.absolutePath());
    }
}

void S3ObjectAddDialog::HandleAccept() {
    QFile file(_fileInfo.absoluteFilePath());
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << file.errorString();
        return;
    }

    const QByteArray binaryData = file.readAll();
    file.close();

    _s3Service->UploadObject(_bucket.bucketName, _bucket.bucketArn, _ui->s3KeyEdit->text(), binaryData, _metadata);

    accept();
}

void S3ObjectAddDialog::HandleReject() {
    accept();
}
