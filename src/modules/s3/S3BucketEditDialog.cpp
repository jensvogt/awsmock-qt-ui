//
// Created by vogje01 on 11/12/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_S3BucketEditDialog.h" resolved

#include <modules/s3/S3BucketEditDialog.h>
#include "ui_S3BucketEditDialog.h"

S3BucketEditDialog::S3BucketEditDialog(const QString &bucketName, QWidget *parent) : BaseDialog(parent), _ui(new Ui::S3BucketEditDialog), _bucket(bucketName) {

    _s3Service = new S3Service();

    _s3Service->GetBucketDetails(bucketName);
    connect(_s3Service, &S3Service::GetBucketDetailsSignal, this, &S3BucketEditDialog::UpdateBucket);

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &S3BucketEditDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &S3BucketEditDialog::HandleReject);

    // Versioned check box
    connect(_ui->versionedCheckBox, &QCheckBox::checkStateChanged, this, [this](const Qt::CheckState &state) {
        this->_changed = true;
    });

    // Setup tabs
    SetupLambdaNotifications();
    SetupQueueNotifications();
    SetupTopicNotifications();
    SetupLifecycles();
    SetupDefaultMetadataTab();

    _ui->tabWidget->setCurrentIndex(0);
}

S3BucketEditDialog::~S3BucketEditDialog() {
    delete _ui;
}

void S3BucketEditDialog::UpdateBucket(const S3GetBucketDetailsResponse &bucketGetResponse) {

    // Save REST response
    _bucketGetResponse = bucketGetResponse;

    // Update fields
    _ui->regionEdit->setText(bucketGetResponse.region);
    _ui->nameEdit->setText(bucketGetResponse.bucketName);
    _ui->arnEdit->setText(bucketGetResponse.bucketArn);
    _ui->ownerEdit->setText(bucketGetResponse.owner);
    _ui->keysEdit->setText(QString::number(bucketGetResponse.objectCount));
    _ui->sizeEdit->setText(QString::number(bucketGetResponse.size));
    _ui->versionedCheckBox->setCheckState(bucketGetResponse.versioned ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    _ui->createdEdit->setText(DateTimeUtils::GetDateTimeFormat(bucketGetResponse.created));
    _ui->modifiedEdit->setText(DateTimeUtils::GetDateTimeFormat(bucketGetResponse.modified));

    // Default metadata
    for (auto it = bucketGetResponse.defaultMetadata.cbegin(); it != bucketGetResponse.defaultMetadata.cend(); ++it) {
        const int row = _defaultMetadataDataModel->rowCount();
        SetColumn(_defaultMetadataDataModel, row, 0, it.key());
        SetColumn(_defaultMetadataDataModel, row, 1, bucketGetResponse.defaultMetadata[it.key()]);
    }

    // Lambda notifications
    for (auto it = bucketGetResponse.lambdaNotifications.begin(); it != bucketGetResponse.lambdaNotifications.cend(); ++it) {
        const int row = _lambdaNotificationDataModel->rowCount();
        SetColumn(_lambdaNotificationDataModel, row, 0, it->id);
        SetColumn(_lambdaNotificationDataModel, row, 1, it->lambdaArn);
    }

    // Queue notifications
    for (auto it = bucketGetResponse.queueNotifications.begin(); it != bucketGetResponse.queueNotifications.cend(); ++it) {
        const int row = _queueNotificationDataModel->rowCount();
        SetColumn(_queueNotificationDataModel, row, 0, it->id);
        SetColumn(_queueNotificationDataModel, row, 1, it->queueArn);
    }

    // Lifecycle rules
    for (auto it = bucketGetResponse.lifecycleRules.begin(); it != bucketGetResponse.lifecycleRules.cend(); ++it) {
        const int row = _lifecycleDataModel->rowCount();
        SetColumn(_lifecycleDataModel, row, 0, it->id);
        SetColumn(_lifecycleDataModel, row, 1, it->status);
    }
}

void S3BucketEditDialog::SetupDefaultMetadataTab() {

    const QStringList headers = QStringList() = {tr("Key"), tr("Value")};

    // Table
    _defaultMetadataDataModel = new QStandardItemModel(this);
    _defaultMetadataDataModel->setHorizontalHeaderLabels(headers);
    _defaultMetadataDataModel->setColumnCount(static_cast<int>(headers.count()));
    _ui->defaultMetadataTable->setModel(_defaultMetadataDataModel);

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
        S3BucketMetadataDialog metadataAdd(this);
        metadataAdd.exec();
        const int row = _defaultMetadataDataModel->rowCount();
        SetColumn(_defaultMetadataDataModel, row, 0, metadataAdd.GetKey());
        SetColumn(_defaultMetadataDataModel, row, 1, metadataAdd.GetValue());
        this->_changed = true;
    });

    // Connect double-click
    connect(_ui->defaultMetadataTable, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {
        const QString key = _defaultMetadataDataModel->item(index.row(), 0)->text();
        const QString value = _defaultMetadataDataModel->item(index.row(), 1)->text();
        S3BucketMetadataDialog dialog(nullptr, key, value);
        dialog.exec();
        _defaultMetadataDataModel->item(index.row(), 1)->setText(dialog.GetValue());
        _changed = true;
    });

    // Add default metadata context menu
    _ui->defaultMetadataTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_ui->defaultMetadataTable, &QTableWidget::customContextMenuRequested, this, &S3BucketEditDialog::ShowDefaultMetadataContextMenu);
}

void S3BucketEditDialog::ShowDefaultMetadataContextMenu(const QPoint &pos) {
    const QModelIndex index = _ui->defaultMetadataTable->indexAt(pos);
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
    const QString key = _defaultMetadataDataModel->item(index.row(), 0)->text();
    const QString value = _defaultMetadataDataModel->item(index.row(), 1)->text();

    // Context menu callbacks
    if (const QAction *selectedAction = menu.exec(_ui->defaultMetadataTable->viewport()->mapToGlobal(pos)); selectedAction == editAction) {
        S3BucketMetadataDialog dialog(nullptr, key, value);
        dialog.exec();
        _changed = true;
    } else if (selectedAction == deleteAction) {
        _defaultMetadataDataModel->removeRow(index.row());
        _changed = true;
    }
}

void S3BucketEditDialog::SetupLambdaNotifications() {

    const QStringList headers = QStringList() = {tr("ID"), tr("ARN")};

    // Table
    _lambdaNotificationDataModel = new QStandardItemModel(this);
    _lambdaNotificationDataModel->setHorizontalHeaderLabels(headers);
    _lambdaNotificationDataModel->setColumnCount(static_cast<int>(headers.count()));
    _ui->lambdaNotificationTable->setModel(_lambdaNotificationDataModel);

    _ui->lambdaNotificationTable->setShowGrid(true);
    _ui->lambdaNotificationTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->lambdaNotificationTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->lambdaNotificationTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->lambdaNotificationTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents); // name
    _ui->lambdaNotificationTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch); //status

    // Enable sorting, default on and sorted ascending by name
    _ui->lambdaNotificationTable->setSortingEnabled(true);
    _ui->lambdaNotificationTable->sortByColumn(0, Qt::AscendingOrder);

    // Connect double-click
    connect(_ui->lambdaNotificationTable, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {

        LambdaNotification notification{};
        const QString id = _lambdaNotificationDataModel->item(index.row(), 0)->text();
        for (const auto &n: _bucketGetResponse.lambdaNotifications) {
            if (n.id == id) {
                notification = n;
            }
        }
        S3LambdaConfiguration configuration{};
        configuration.lambdaArn = notification.lambdaArn;
        configuration.filterRules = notification.filterRules;
        configuration.events = notification.events;
        S3BucketLambdaNotificationDialog lambdaNotificationDialog(_bucket, configuration, this);
        lambdaNotificationDialog.exec();
    });

    _ui->lambdaNotificationAddButton->setText(nullptr);
    _ui->lambdaNotificationAddButton->setIcon(IconUtils::GetIcon("add"));
    connect(_ui->lambdaNotificationAddButton, &QAbstractButton::clicked, this, [this]() {
        S3BucketLambdaNotificationDialog lambdaNotificationDialog(_bucket, this);
        lambdaNotificationDialog.exec();
        this->_changed = true;
    });
}

void S3BucketEditDialog::SetupQueueNotifications() {

    const QStringList headers = QStringList() = {tr("ID"), tr("ARN")};

    // Table
    _queueNotificationDataModel = new QStandardItemModel(this);
    _queueNotificationDataModel->setHorizontalHeaderLabels(headers);
    _queueNotificationDataModel->setColumnCount(static_cast<int>(headers.count()));
    _ui->queueNotificationTable->setModel(_queueNotificationDataModel);

    _ui->queueNotificationTable->setShowGrid(true);
    _ui->queueNotificationTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->queueNotificationTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->queueNotificationTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->queueNotificationTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents); // name
    _ui->queueNotificationTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch); //status

    // Enable sorting, default on and sorted ascending by name
    _ui->queueNotificationTable->setSortingEnabled(true);
    _ui->queueNotificationTable->sortByColumn(0, Qt::AscendingOrder);

    _ui->queueNotificationAddButton->setText(nullptr);
    _ui->queueNotificationAddButton->setIcon(IconUtils::GetIcon("add"));
    connect(_ui->queueNotificationAddButton, &QAbstractButton::clicked, this, [this]() {

        if (S3QueueNotificationDialog queueNotificationDialog(this); queueNotificationDialog.exec() == Accepted) {
            const QString queueArn = queueNotificationDialog.GetQueueArn();
            const QStringList s3Events = queueNotificationDialog.GetS3Event();
            const QString id = StringUtils::GenerateRandomNumericString(12);

            const int row = _queueNotificationDataModel->rowCount();
            SetColumn(_queueNotificationDataModel, row, 0, id);
            SetColumn(_queueNotificationDataModel, row, 1, queueArn);

            S3QueueConfiguration configuration{};
            configuration.queueArn = queueArn;
            for (const auto &event: s3Events) {
                configuration.events.append(S3NotificationEventFromString(event));
            }
            S3PutBucketNotificationConfigurationRequest request{};
            request.bucket = _bucket;
            request.queueConfigurations.append(configuration);
            _s3Service->PutBucketNotificationConfiguration(request);
            this->_changed = true;
        }
    });

    // Context menu
    _ui->queueNotificationTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_ui->queueNotificationTable, &QTableView::customContextMenuRequested, this, &S3BucketEditDialog::ShowQueueNotificationContextMenu);
}

void S3BucketEditDialog::SetupTopicNotifications() {

    const QStringList headers = QStringList() = {tr("ID"), tr("ARN")};

    // Table
    _topicNotificationDataModel = new QStandardItemModel(this);
    _topicNotificationDataModel->setHorizontalHeaderLabels(headers);
    _topicNotificationDataModel->setColumnCount(static_cast<int>(headers.count()));
    _ui->topicNotificationTable->setModel(_topicNotificationDataModel);

    _ui->topicNotificationTable->setShowGrid(true);
    _ui->topicNotificationTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->topicNotificationTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->topicNotificationTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->topicNotificationTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents); // name
    _ui->topicNotificationTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch); //status

    // Enable sorting, default on and sorted ascending by name
    _ui->topicNotificationTable->setSortingEnabled(true);
    _ui->topicNotificationTable->sortByColumn(0, Qt::AscendingOrder);

    _ui->topicNotificationAddButton->setText(nullptr);
    _ui->topicNotificationAddButton->setIcon(IconUtils::GetIcon("add"));
    connect(_ui->topicNotificationAddButton, &QAbstractButton::clicked, this, [this]() {
        /*S3ObjectMetadataDialog metadataAdd(this, true);
        metadataAdd.exec();
        const int row = _topicNotificationDataModel->rowCount();
        SetColumn(_topicNotificationDataModel, row, 0, metadataAdd.GetKey());
        SetColumn(_topicNotificationDataModel, row, 1, metadataAdd.GetValue());*/
        this->_changed = true;
    });
}

void S3BucketEditDialog::SetupLifecycles() {

    const QStringList headers = QStringList() = {tr("ID"), tr("Status")};

    // Table
    _lifecycleDataModel = new QStandardItemModel(this);
    _lifecycleDataModel->setHorizontalHeaderLabels(headers);
    _lifecycleDataModel->setColumnCount(static_cast<int>(headers.count()));
    _ui->lifecycleTable->setModel(_lifecycleDataModel);

    _ui->lifecycleTable->setShowGrid(true);
    _ui->lifecycleTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->lifecycleTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->lifecycleTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->lifecycleTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch); // name
    _ui->lifecycleTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents); //status

    // Enable sorting, default on and sorted ascending by name
    _ui->lifecycleTable->setSortingEnabled(true);
    _ui->lifecycleTable->sortByColumn(0, Qt::AscendingOrder);

    _ui->lifecycleAddButton->setText(nullptr);
    _ui->lifecycleAddButton->setIcon(IconUtils::GetIcon("add"));
    connect(_ui->lifecycleAddButton, &QAbstractButton::clicked, this, [this]() {
        if (S3BucketLifecycleDialog dialog(nullptr); dialog.exec() == Accepted) {
            const LifecycleRule lifecycleRule = dialog.GetLifecycleRule();
            const int row = _lifecycleDataModel->rowCount();
            SetColumn(_lifecycleDataModel, row, 0, lifecycleRule.id);
            SetColumn(_lifecycleDataModel, row, 1, lifecycleRule.status);
            this->_changed = true;
        }
    });

    // Connect double-click
    connect(_ui->lifecycleTable, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {
        LifecycleRule lifecycleRule = _bucketGetResponse.lifecycleRules.at(index.row());
        S3BucketLifecycleDialog dialog(lifecycleRule, nullptr);
        dialog.exec();
        lifecycleRule = dialog.GetLifecycleRule();
        _lifecycleDataModel->item(index.row(), 0)->setText(lifecycleRule.id);
        _lifecycleDataModel->item(index.row(), 1)->setText(lifecycleRule.status);
        _bucketGetResponse.lifecycleRules[index.row()] = lifecycleRule;
        _changed = true;
    });

    // Add lifecycle context menu
    _ui->lifecycleTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_ui->lifecycleTable, &QTableWidget::customContextMenuRequested, this, &S3BucketEditDialog::ShowLifecycleContextMenu);
}

void S3BucketEditDialog::ShowLifecycleContextMenu(const QPoint &pos) {
    const QModelIndex index = _ui->lifecycleTable->indexAt(pos);
    if (!index.isValid()) {
        return;
    }

    // Context menu
    QMenu menu;
    QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit Lifecycle");
    editAction->setToolTip("Edit the bucket lifecycle rules");
    menu.addSeparator();
    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Lifecycle");
    deleteAction->setToolTip("Delete the bucket lifecycle rule");

    // Get the metadata attributes
    const QString key = _lifecycleDataModel->item(index.row(), 0)->text();
    const QString value = _lifecycleDataModel->item(index.row(), 1)->text();

    // Context menu callbacks
    if (const QAction *selectedAction = menu.exec(_ui->lifecycleTable->viewport()->mapToGlobal(pos)); selectedAction == editAction) {
        LifecycleRule lifecycleRule = _bucketGetResponse.lifecycleRules.at(index.row());
        S3BucketLifecycleDialog dialog(lifecycleRule, nullptr);
        dialog.exec();
        lifecycleRule = dialog.GetLifecycleRule();
        _lifecycleDataModel->item(index.row(), 1)->setText(lifecycleRule.status);
        _changed = true;
    } else if (selectedAction == deleteAction) {
        _defaultMetadataDataModel->removeRow(index.row());
        _changed = true;
    }
}

void S3BucketEditDialog::ShowQueueNotificationContextMenu(const QPoint &pos) {

    const QModelIndex index = _ui->queueNotificationTable->indexAt(pos);
    if (!index.isValid()) {
        return;
    }

    // Context menu
    QMenu menu;
    QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit Queue Notification");
    editAction->setToolTip("Edit the bucket queue notification");
    menu.addSeparator();
    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Queue Notification");
    deleteAction->setToolTip("Delete the bucket queue notification");

    // Get the metadata attributes
    const QString id = _queueNotificationDataModel->item(index.row(), 0)->text();
    const QString queueArn = _queueNotificationDataModel->item(index.row(), 1)->text();

    // Context menu callbacks
    if (const QAction *selectedAction = menu.exec(_ui->queueNotificationTable->viewport()->mapToGlobal(pos)); selectedAction == editAction) {
        // LifecycleRule lifecycleRule = _bucketGetResponse.lifecycleRules.at(index.row());
        // S3BucketLifecycleDialog dialog(lifecycleRule, nullptr);
        // dialog.exec();
        // lifecycleRule = dialog.GetLifecycleRule();
        // _lifecycleDataModel->item(index.row(), 1)->setText(lifecycleRule.status);
        _changed = true;
    } else if (selectedAction == deleteAction) {
        _queueNotificationDataModel->removeRow(index.row());
        _changed = true;
    }
}

void S3BucketEditDialog::HandleAccept() {
    if (this->_changed) {
        QMap<QString, QString> defaultMetadata;
        for (int i = 0; i < _defaultMetadataDataModel->rowCount(); i++) {
            QModelIndex col1 = _defaultMetadataDataModel->index(i, 0);
            QModelIndex col2 = _defaultMetadataDataModel->index(i, 1);
            defaultMetadata[_defaultMetadataDataModel->data(col1).toString()] = _defaultMetadataDataModel->data(col2).toString();
        }
        _s3Service->UpdateBucket(_ui->nameEdit->text(), defaultMetadata, _bucketGetResponse.lifecycleRules, _ui->versionedCheckBox->checkState() ? "enabled" : "disabled");
    }
    accept();
}


void S3BucketEditDialog::HandleReject() {
    accept();
}
