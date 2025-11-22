#include <modules/sqs/SQSQueueDetailsDialog.h>
#include "ui_SQSQueueDetailsDialog.h"
#include "dto/sqs/SQSListQueueDefaultAttribtesResponse.h"
#include "utils/IconUtils.h"

SQSQueueDetailsDialog::SQSQueueDetailsDialog(const QString &queueArn, QWidget *parent) : BaseDialog(parent), _ui(new Ui::SQSQueueDetailsDialog), _queueArn(queueArn) {

    _ui->setupUi(this);

    _sqsService = new SQSService();

    // Connect service
    _sqsService->GetQueueDetails(queueArn);
    connect(_sqsService, &SQSService::GetQueueDetailsSignal, this, &SQSQueueDetailsDialog::UpdateQueueDetails);
    connect(_sqsService, &SQSService::ListQueueAttributesSignal, this, &SQSQueueDetailsDialog::UpdateQueueAttributes);
    connect(_sqsService, &SQSService::ListQueueLambdaTriggersSignal, this, &SQSQueueDetailsDialog::UpdateQueueLambdaTriggers);
    connect(_sqsService, &SQSService::ListQueueDefaultAttributesSignal, this, &SQSQueueDetailsDialog::UpdateDefaultAttributes);

    // Setup attributes tab
    SetupAttributesTab();

    // Setup lambda triggers tab
    SetupLambdaTriggersTab();

    // Setup default attributes tab
    SetupDefaultAttributesTab();

    // Set default tab
    _ui->tabWidget->setCurrentIndex(0);
}

SQSQueueDetailsDialog::~SQSQueueDetailsDialog() {
    delete _ui;
}

void SQSQueueDetailsDialog::UpdateQueueDetails(const SQSGetQueueDetailsResponse &response) {

    _ui->queueNameEdit->setText(response.queueName);
    _ui->queueArnEdit->setText(response.queueArn);
    _ui->queueUrlEdit->setText(response.queueUrl);
    _ui->dlqArnEdit->setText(response.dlqArn);
    _ui->dlqMaxReceiveEdit->setText(QString::number(response.dlqMaxReceived));
    _ui->visibilityEdit->setText(QString::number(response.visibilityTimeout));
    _ui->delayEdit->setText(QString::number(response.delay));
    _ui->maxMessageSizeEdit->setText(QString::number(response.maxMessageSize / 1024));
    _ui->availableEdit->setText(QString::number(response.available));
    _ui->invisibleEdit->setText(QString::number(response.invisible));
    _ui->delayedEdit->setText(QString::number(response.delayed));
    _ui->retentionPeriodEdit->setText(QString::number(response.retentionPeriod));
    _ui->messageCountEdit->setText(QString::number(response.messageCount));
    _ui->messageSizeEdit->setText(QString::number(response.size / 1024));
    _ui->createdEdit->setText(response.created.toString());
    _ui->modifiedEdit->setText(response.modified.toString());

    connect(_ui->delayEdit, &QLineEdit::editingFinished, this, [&]() { this->changed = true; });
    connect(_ui->retentionPeriodEdit, &QLineEdit::editingFinished, this, [&]() { this->changed = true; });
    connect(_ui->visibilityEdit, &QLineEdit::editingFinished, this, [&]() { this->changed = true; });
}

void SQSQueueDetailsDialog::on_sqsQueueDetailsButtonBox_accepted() {
    if (this->changed) {
        SQSQueueUpdateRequest updateQueueRequest;
        updateQueueRequest.queueArn = _queueArn;
        updateQueueRequest.delay = _ui->delayEdit->text().toLong();
        updateQueueRequest.retentionPeriod = _ui->retentionPeriodEdit->text().toLong();
        updateQueueRequest.visibilityTimeout = _ui->visibilityEdit->text().toLong();
        _sqsService->UpdateQueue(updateQueueRequest);
    }
    accept();
}

void SQSQueueDetailsDialog::on_sqsQueueDetailsButtonBox_rejected() {
    accept();
}

void SQSQueueDetailsDialog::SetupAttributesTab() const {

    // Refresh button
    _ui->attributeRefreshButton->setText(nullptr);
    _ui->attributeRefreshButton->setIcon(IconUtils::GetIcon("dark", "refresh"));
    _ui->attributeRefreshButton->setToolTip(tr("Refresh attribute list"));
    connect(_ui->attributeRefreshButton, &QPushButton::clicked, [this]() {
        _sqsService->ListQueueAttributes(_queueArn, "");
    });

    // Table
    const QStringList headers = QStringList() << tr("Name")
                                << tr("Value");

    _ui->attributesTable->setColumnCount(static_cast<int>(headers.count()));
    _ui->attributesTable->setShowGrid(true);
    _ui->attributesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->attributesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->attributesTable->setHorizontalHeaderLabels(headers);
    _ui->attributesTable->setSortingEnabled(true);
    _ui->attributesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->attributesTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    _ui->attributesTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    _sqsService->ListQueueAttributes(_queueArn, "");
}

void SQSQueueDetailsDialog::UpdateQueueAttributes(const SQSQueueAttributeListResponse &response) const {
    _ui->attributesTable->setRowCount(0);
    _ui->attributesTable->setSortingEnabled(false); // stop sorting
    for (auto r = 0, c = 0; r < response.queueAttributeCounters.count(); r++, c = 0) {
        _ui->attributesTable->insertRow(r);
        SetColumn(_ui->attributesTable, r, c++, response.queueAttributeCounters.at(r).key);
        SetColumn(_ui->attributesTable, r, c, response.queueAttributeCounters.at(r).value);
    }
    _ui->attributesTable->setRowCount(static_cast<int>(response.queueAttributeCounters.count()));
    _ui->attributesTable->setSortingEnabled(true);
    _ui->attributesTable->sortItems(_attributesSortColumn, _attributesSortOrder);
}

void SQSQueueDetailsDialog::SetupLambdaTriggersTab() const {

    // Refresh button
    _ui->lambdaRefreshButton->setText(nullptr);
    _ui->lambdaRefreshButton->setIcon(IconUtils::GetIcon("dark", "refresh"));
    _ui->lambdaRefreshButton->setToolTip(tr("Refresh lambda trigger list"));
    connect(_ui->lambdaRefreshButton, &QPushButton::clicked, [this]() {
        _sqsService->ListQueueLambdaTriggers(_queueArn, "");
    });

    // Table
    const QStringList headers = QStringList() = {tr("UUID"), tr("Arn"), tr("Enabled")};

    _ui->lambdaTable->setColumnCount(static_cast<int>(headers.count()));
    _ui->lambdaTable->setShowGrid(true);
    _ui->lambdaTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->lambdaTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->lambdaTable->setHorizontalHeaderLabels(headers);
    _ui->lambdaTable->setSortingEnabled(true);
    _ui->lambdaTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->lambdaTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    _ui->lambdaTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    _ui->lambdaTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    _sqsService->ListQueueLambdaTriggers(_queueArn, "");
}

void SQSQueueDetailsDialog::UpdateQueueLambdaTriggers(const SQSListQueueLambdaTriggersResponse &response) const {
    _ui->lambdaTable->setRowCount(0);
    _ui->lambdaTable->setSortingEnabled(false); // stop sorting
    for (auto r = 0, c = 0; r < response.queueLambdaTriggerCounters.count(); r++, c = 0) {
        _ui->lambdaTable->insertRow(r);
        SetColumn(_ui->lambdaTable, r, c++, response.queueLambdaTriggerCounters.at(r).uuid);
        SetColumn(_ui->lambdaTable, r, c++, response.queueLambdaTriggerCounters.at(r).arn);
        SetColumn(_ui->lambdaTable, r, c, response.queueLambdaTriggerCounters.at(r).enabled);
    }
    _ui->lambdaTable->setRowCount(static_cast<int>(response.queueLambdaTriggerCounters.count()));
    _ui->lambdaTable->setSortingEnabled(true);
    _ui->lambdaTable->sortItems(_lambdaTriggerSortColumn, _lambdaTriggerSortOrder);
}

void SQSQueueDetailsDialog::SetupDefaultAttributesTab() const {

    // Add button
    _ui->addDefaultAttributeButton->setText(nullptr);
    _ui->addDefaultAttributeButton->setIcon(IconUtils::GetIcon("dark", "add"));
    _ui->addDefaultAttributeButton->setToolTip(tr("Add default attributes"));
    connect(_ui->addDefaultAttributeButton, &QPushButton::clicked, [this]() {
        _sqsService->ListQueueDefaultAttributes(_queueArn, "");
    });

    // Table
    const QStringList headers = QStringList() = {tr("Key"), tr("Data Type"), tr("Value")};

    _ui->defaultAttributeTable->setColumnCount(static_cast<int>(headers.count()));
    _ui->defaultAttributeTable->setShowGrid(true);
    _ui->defaultAttributeTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->defaultAttributeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->defaultAttributeTable->setHorizontalHeaderLabels(headers);
    _ui->defaultAttributeTable->setSortingEnabled(true);
    _ui->defaultAttributeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->defaultAttributeTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    _ui->defaultAttributeTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    _ui->defaultAttributeTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    _sqsService->ListQueueDefaultAttributes(_queueArn, "");
}

void SQSQueueDetailsDialog::UpdateDefaultAttributes(const SQSListQueueDefaultAttributesResponse &response) const {
    _ui->defaultAttributeTable->setRowCount(0);
    _ui->defaultAttributeTable->setSortingEnabled(false); // stop sorting
    int r = 0;
    for (const auto &key: response.defaultAttributesCounters.keys()) {
        _ui->defaultAttributeTable->insertRow(r);
        MessageAttribute defaultAttribute = response.defaultAttributesCounters[key];
        SetColumn(_ui->defaultAttributeTable, r, 0, key);
        SetColumn(_ui->defaultAttributeTable, r, 1, MessageAttributeDataTypeToString(response.defaultAttributesCounters[key].dataType).data());
        SetColumn(_ui->defaultAttributeTable, r, 2, response.defaultAttributesCounters[key].stringValue);
        r++;
    }
    _ui->defaultAttributeTable->setRowCount(r);
    _ui->defaultAttributeTable->setSortingEnabled(true);
    _ui->defaultAttributeTable->sortItems(_defaultAttributeSortColumn, _defaultAttributeSortOrder);
}
