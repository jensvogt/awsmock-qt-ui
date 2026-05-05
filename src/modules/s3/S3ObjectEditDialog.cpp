//
// Created by vogje01 on 11/24/25.
//

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

    // Storage type combo box
    connect(_ui->storageTypeCombo, &QComboBox::currentIndexChanged, this, [this]() {
        this->_changed = true;
    });

    // Plain text edit
    _plaintextEdit = new QPlainTextEdit();
    const QFontMetrics fm(_plaintextEdit->font());
    const int tabWidth = fm.horizontalAdvance(' ') * 2;
    _plaintextEdit->setTabStopDistance(tabWidth);
    _plaintextEdit->setMaximumBlockCount(0);

    // Image widget (inside scroll area)
    _imageLabel = new QLabel();
    _imageLabel->setAlignment(Qt::AlignCenter);
    _imageLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    _imageLabel->setScaledContents(false);
    _imageScrollArea = new QScrollArea();
    _imageScrollArea->setWidgetResizable(false);
    _imageScrollArea->setAlignment(Qt::AlignCenter);
    _imageScrollArea->setWidget(_imageLabel);
    _imageScrollArea->viewport()->installEventFilter(this);
    _imageLabel->installEventFilter(this);

    // Remove placeholder pages from the .ui so indices don't point at empty pages.
    while (_ui->stackedWidget->count() > 0) {
        QWidget *page = _ui->stackedWidget->widget(0);
        _ui->stackedWidget->removeWidget(page);
        delete page;
    }

    // Add to stacked widget
    _ui->stackedWidget->addWidget(_plaintextEdit);
    _ui->stackedWidget->addWidget(_imageScrollArea);

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
    _ui->bodyRefreshButton->setToolTip("Refresh the S3 object body");
    connect(_ui->bodyRefreshButton, &QAbstractButton::clicked, this, [this, objectId]() {
        _s3Service->GetObjectDetails(objectId);
    });

    // Body save button
    _ui->bodySaveButton->setText(nullptr);
    _ui->bodySaveButton->setIcon(IconUtils::GetIcon("save"));
    _ui->bodySaveButton->setToolTip("Save the data to a local file");
    connect(_ui->bodySaveButton, &QPushButton::clicked, this, &S3ObjectEditDialog::SaveToFile);

    // Window button
    _ui->windowButton->setText(nullptr);
    _ui->windowButton->setIcon(IconUtils::GetIcon("extern-window"));
    connect(_ui->windowButton, &QPushButton::clicked, this, [objectId]() {
        if (S3ObjectWindow dialog(objectId); dialog.exec() == Accepted) {
            logInfo << "Object window closed";
        }
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

    // Image zoom shortcuts
    new QShortcut(QKeySequence::ZoomIn, this, [this]() { SetImageZoom(_imageZoom * 1.1); });
    new QShortcut(QKeySequence::ZoomOut, this, [this]() { SetImageZoom(_imageZoom / 1.1); });
    /*    new QShortcut(QKeySequence::ZoomReset, this, [this]() {
            _imageFitToWindow = false;
            SetImageZoom(1.0);
        });*/
    new QShortcut(QKeySequence(tr("Ctrl+F")), this, [this]() {
        _imageFitToWindow = !_imageFitToWindow;
        FitImageToViewport();
    });
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
        _s3Service->UpdateObject(_ui->regionEdit->text(), _ui->bucketEdit->text(), _ui->keyEdit->text(), _plaintextEdit->toPlainText().toUtf8(), _ui->storageTypeCombo->currentText(), defaultMetadata);
    }
    accept();
}

void S3ObjectEditDialog::HandleReject() {
    accept();
}

void S3ObjectEditDialog::UpdateObject(const S3GetObjectDetailsResponse &objectDetailsResponse) {

    _objectDetails = objectDetailsResponse;
    _ui->regionEdit->setText(objectDetailsResponse.region);
    _ui->bucketEdit->setText(objectDetailsResponse.bucketName);
    _ui->keyEdit->setText(objectDetailsResponse.key);
    _ui->ownerEdit->setText(objectDetailsResponse.owner);
    _ui->contentTypeEdit->setText(objectDetailsResponse.contentType);
    _ui->sizeEdit->setText(StringUtils::FormatSizeColumn(objectDetailsResponse.size, 1));
    _ui->internalNameEdit->setText(objectDetailsResponse.internalName);
    _ui->versionIdEdit->setText(objectDetailsResponse.versionId.size() > 0 ? objectDetailsResponse.versionId : "not versioned");
    _ui->createdEdit->setText(DateTimeUtils::GetDateTimeFormat(objectDetailsResponse.created));
    _ui->modifiedEdit->setText(DateTimeUtils::GetDateTimeFormat(objectDetailsResponse.modified));
    if (ImageUtils::IsImageContentType(objectDetailsResponse.contentType)) {
        QPixmap pix;
        pix.loadFromData(objectDetailsResponse.body);
        _imageOriginal = pix;
        _imageFitToWindow = false;
        _imageZoom = 1.0;
        ApplyImageScale();
        _ui->stackedWidget->setCurrentWidget(_imageScrollArea);
    } else {
        if (objectDetailsResponse.size > _maxSize) {
            _plaintextEdit->setPlainText(objectDetailsResponse.body.left(_maxSize) + "\n\n... [truncated]");
            _ui->statusLabel->setText("Body is truncated to " + StringUtils::FormatSizeColumn(_maxSize, 1));
        } else {
            _plaintextEdit->setPlainText(objectDetailsResponse.body);
        }
        _ui->stackedWidget->setCurrentWidget(_plaintextEdit);
    }

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

bool S3ObjectEditDialog::eventFilter(QObject *watched, QEvent *event) {
    if ((watched == _imageScrollArea->viewport() || watched == _imageLabel) && event->type() == QEvent::Wheel) {
        if (const auto *wheelEvent = dynamic_cast<QWheelEvent *>(event); wheelEvent->modifiers().testFlag(Qt::ControlModifier)) {
            _imageFitToWindow = false;
            if (const int delta = wheelEvent->angleDelta().y(); delta > 0) {
                SetImageZoom(_imageZoom * 1.1);
            } else if (delta < 0) {
                SetImageZoom(_imageZoom / 1.1);
            }
            return true;
        }
    }
    if ((watched == _imageScrollArea->viewport() || watched == _imageLabel) && event->type() == QEvent::Resize) {
        if (_imageFitToWindow) {
            FitImageToViewport();
        }
    }
    return BaseDialog::eventFilter(watched, event);
}

void S3ObjectEditDialog::SetImageZoom(const qreal zoom) {
    _imageZoom = qBound(0.1, zoom, 10.0);
    ApplyImageScale();
}

void S3ObjectEditDialog::ApplyImageScale() const {
    if (_imageOriginal.isNull()) {
        return;
    }
    if (_imageFitToWindow) {
        FitImageToViewport();
        return;
    }
    const QSizeF targetSize = (_imageOriginal.size() * _imageZoom).toSizeF();
    _imageLabel->setPixmap(_imageOriginal.scaled(targetSize.toSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    _imageLabel->adjustSize();
}

void S3ObjectEditDialog::FitImageToViewport() const {
    if (_imageOriginal.isNull() || !_imageScrollArea) {
        return;
    }
    const QSize viewportSize = _imageScrollArea->viewport()->size();
    if (viewportSize.isEmpty()) {
        return;
    }
    const QSize scaled = _imageOriginal.size().scaled(viewportSize, Qt::KeepAspectRatio);
    _imageLabel->setPixmap(_imageOriginal.scaled(scaled, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    _imageLabel->adjustSize();
}

void S3ObjectEditDialog::SaveToFile() const {

    if (const QString fileName = SelectFilename(); !fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::critical(nullptr, "Error", "Could not open file:" + fileName);
        }
        const long count = file.write(_objectDetails.body);
        file.close();
        logInfo << "S3 object body save to file: " << fileName << ", fileSize: " << count;
        _ui->statusLabel->setText("S3 object body saved to file: " + fileName + ", size: " + StringUtils::FormatSizeColumn(count, 0));
    }
}

QString S3ObjectEditDialog::SelectFilename() {

    // Create a QFileDialog set to select existing files
    const auto filter = "JSON Files (*.json);All Files (*.*)";
    const auto defaultDir = Configuration::instance().GetValue<QString>("ui.default-directory.S3SaveBodyToFile", "/usr/local/awsmock-qt-_ui");

    if (const QString filePath = QFileDialog::getSaveFileName(nullptr, "Open JSON Configuration File", defaultDir, filter); !filePath.isEmpty()) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadWrite)) {
            QMessageBox::critical(nullptr, "Error", "Could not open file:" + filePath);
            return {};
        }
        file.close();
        Configuration::instance().SetValue<QString>("ui.default-directory.S3SaveBodyToFile", QFileInfo(filePath).absolutePath());
        logDebug << "S3 object file path: " << filePath;
        return filePath;
    }
    return {};
}
