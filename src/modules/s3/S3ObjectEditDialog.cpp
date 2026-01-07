//
// Created by vogje01 on 11/24/25.
//

#include <QMenu>
#include <modules/s3/S3ObjectEditDialog.h>
#include "ui_S3ObjectEditDialog.h"
#include "modules/s3/S3ObjectMetadataDialog.h"

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

    // Storage type combo box
    connect(_ui->storageTypeCombo, &QComboBox::currentIndexChanged, this, [this]() {
        this->_changed = true;
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

    // Connect double-click
    connect(_ui->metadataTable, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {
        const QString key = _ui->metadataTable->item(index.row(), 0)->text();
        const QString value = _ui->metadataTable->item(index.row(), 1)->text();
        S3ObjectMetadataDialog dialog(key, value);
        dialog.exec();
        _ui->metadataTable->item(index.row(), 1)->setText(dialog.GetValue());
        _changed = true;
    });

    // Body refresh button
    _ui->bodyRefreshButton->setText(nullptr);
    _ui->bodyRefreshButton->setIcon(IconUtils::GetIcon("refresh"));
    connect(_ui->bodyRefreshButton, &QAbstractButton::clicked, this, [this, objectId]() {
        _s3Service->GetObjectDetails(objectId);
    });

    // Metadata add button
    _ui->metadataAddButton->setText(nullptr);
    _ui->metadataAddButton->setIcon(IconUtils::GetIcon("add"));
    connect(_ui->metadataAddButton, &QAbstractButton::clicked, this, [this]() {
        S3ObjectMetadataDialog metadataAdd(this);
        metadataAdd.exec();
        const int row = _ui->metadataTable->rowCount();
        _ui->metadataTable->insertRow(row);
        SetColumn(_ui->metadataTable, row, 0, metadataAdd.GetKey());
        SetColumn(_ui->metadataTable, row, 1, metadataAdd.GetValue());
        _changed = true;
    });

    // Add default metadata context menu
    _ui->metadataTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_ui->metadataTable, &QTableWidget::customContextMenuRequested, this, &S3ObjectEditDialog::ShowDefaultMetadataContextMenu);

    // Set default tab
    _ui->tabWidget->setCurrentIndex(0);
}

void S3ObjectEditDialog::ShowDefaultMetadataContextMenu(const QPoint &pos) {
    const QModelIndex index = _ui->metadataTable->indexAt(pos);
    if (!index.isValid()) {
        return;
    }

    // Context menu
    QMenu menu;
    QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit Metadata");
    editAction->setToolTip("Edit the bucket default metadata");
    menu.addSeparator();
    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Metadata");
    deleteAction->setToolTip("Delete the bucket default metadata");

    // Get the metadata attributes
    const QString key = _ui->metadataTable->item(index.row(), 0)->text();
    const QString value = _ui->metadataTable->item(index.row(), 1)->text();

    // Context menu callbacks
    if (const QAction *selectedAction = menu.exec(_ui->metadataTable->viewport()->mapToGlobal(pos)); selectedAction == editAction) {
        S3ObjectMetadataDialog dialog(key, value);
        dialog.exec();
        _changed = true;
    } else if (selectedAction == deleteAction) {
        _ui->metadataTable->removeRow(index.row());
        _changed = true;
    }
}

S3ObjectEditDialog::~S3ObjectEditDialog() {
    delete _ui;
}

void S3ObjectEditDialog::HandleAccept() {
    if (_changed) {
        QMap<QString, QString> defaultMetadata;
        for (int i = 0; i < _ui->metadataTable->rowCount(); i++) {
            const QString key = _ui->metadataTable->item(i, 0)->text();
            const QString value = _ui->metadataTable->item(i, 1)->text();
            defaultMetadata[key] = value;
        }
        _s3Service->UpdateObject(_ui->regionEdit->text(), _ui->bucketEdit->text(), _ui->keyEdit->text(), _ui->bodyTextEdit->toPlainText().toUtf8(), _ui->storageTypeCombo->currentText(), defaultMetadata);
    }
    accept();
}

void S3ObjectEditDialog::HandleReject() {
    accept();
}

void S3ObjectEditDialog::UpdateObject(const S3GetObjectDetailsResponse &objectDetailsResponse) const {

    _ui->regionEdit->setText(objectDetailsResponse.region);
    _ui->bucketEdit->setText(objectDetailsResponse.bucketName);
    _ui->keyEdit->setText(objectDetailsResponse.key);
    _ui->ownerEdit->setText(objectDetailsResponse.owner);
    _ui->contentTypeEdit->setText(objectDetailsResponse.contentType);
    _ui->sizeEdit->setText(QString::number(objectDetailsResponse.size));
    _ui->createdEdit->setText(DateTimeUtils::GetDateTimeFormat(objectDetailsResponse.created));
    _ui->modifiedEdit->setText(DateTimeUtils::GetDateTimeFormat(objectDetailsResponse.modified));
    _ui->bodyTextEdit->setPlainText(objectDetailsResponse.body);

    // Storage classes
    const QStringList storageClasses = {"STANDARD", "STANDARD_IA", "ONEZONE_IA", "EXPRESS_ONEZONE", "GLACIER", "GLACIER_IR", "DEEP_ARCHIVE", "INTELLIGENT_TIERING", "REDUCED_REDUNDANCY"};
    _ui->storageTypeCombo->addItems(storageClasses);
    _ui->storageTypeCombo->setCurrentText(objectDetailsResponse.storageClass);

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
