#include <modules/sns/SNSTopicDetailsDialog.h>
#include "ui_SNSTopicDetailsDialog.h"
#include "modules/sns/SNSTopicDefaultAttributeDialog.h"

SNSTopicDetailsDialog::SNSTopicDetailsDialog(QString topicArn, QWidget *parent) : ::BaseDialog(parent), _ui(new Ui::SNSTopicDetailsDialog), _topicArn(std::move(topicArn)) {

    // Setup UI
    _ui->setupUi(this);
    connect(_ui->snsTopicDetailsButtonBox, &QDialogButtonBox::accepted, this, &SNSTopicDetailsDialog::HandleAccept);
    connect(_ui->snsTopicDetailsButtonBox, &QDialogButtonBox::rejected, this, &SNSTopicDetailsDialog::HandleReject);

    // SNS REST service
    _snsService = new SNSService();
    connect(_snsService, &SNSService::GetTopicDetailsSignal, this, &SNSTopicDetailsDialog::UpdateTopicDetails);
    connect(_snsService, &SNSService::ListTopicAttributesSignal, this, &SNSTopicDetailsDialog::UpdateTopicAttributes);
    connect(_snsService, &SNSService::ListTopicTagsSignal, this, &SNSTopicDetailsDialog::UpdateTopicTags);
    connect(_snsService, &SNSService::ListTopicSubscriptionsSignal, this, &SNSTopicDetailsDialog::UpdateTopicSubscriptions);
    connect(_snsService, &SNSService::ListTopicDefaultAttributesSignal, this, &SNSTopicDetailsDialog::UpdateDefaultAttributes);

    // Tab widget
    connect(_ui->tabWidget, &QTabWidget::currentChanged, this, &SNSTopicDetailsDialog::CurrentTabChanged);
    _ui->tabWidget->setCurrentIndex(0);

    // Tables
    SetupAttributesTable();
    SetupTagsTable();
    SetupSubscriptionsTable();
    SetupDefaultAttributesTab();

    // Load initial content
    SNSTopicDetailsDialog::LoadContent();
}

SNSTopicDetailsDialog::~SNSTopicDetailsDialog() {
    delete _ui;
}

void SNSTopicDetailsDialog::LoadContent() {
    _snsService->GetTopicDetails(_topicArn);
    _snsService->ListTopicAttributes(_topicArn);
}

void SNSTopicDetailsDialog::HandleAccept() {
    accept();
}

void SNSTopicDetailsDialog::HandleReject() {
    accept();
}

void SNSTopicDetailsDialog::UpdateTopicDetails(const SNSGetTopicDetailsResponse &response) const {

    _ui->regionEdit->setText(response.region);
    _ui->topicNameEdit->setText(response.topicName);
    _ui->topicArnEdit->setText(response.topicArn);
    _ui->messageCountEdit->setText(QString::number(response.messageCount));
    _ui->messageSizeEdit->setText(StringUtils::FormatSizeColumn(response.size, 1));
    _ui->createdEdit->setText(DateTimeUtils::GetDateTimeFormat(response.created));
    _ui->modifiedEdit->setText(DateTimeUtils::GetDateTimeFormat(response.modified));
}

void SNSTopicDetailsDialog::UpdateTopicAttributes(const ListTopicAttributesResponse &response) const {
    const int selectedRow = _ui->attributeTable->selectionModel()->currentIndex().row();
    _ui->attributeTable->setSortingEnabled(false);

    for (int r = 0, c = 0; r < response.topicAttributes.count(); r++, c = 0) {
        SetColumn(_attributesDataModel, r, c++, response.topicAttributes.at(r).key);
        SetColumn(_attributesDataModel, r, c++, response.topicAttributes.at(r).value);
    }
    // Reset selection
    _ui->attributeTable->setSortingEnabled(true);
    _ui->attributeTable->sortByColumn(_attributesSortColumn, _attributesSortOrder);
    _ui->attributeTable->selectRow(selectedRow);
}

void SNSTopicDetailsDialog::UpdateTopicTags(const ListTopicTagsResponse &response) const {
    const int selectedRow = _ui->tagsTable->selectionModel()->currentIndex().row();
    _ui->tagsTable->setSortingEnabled(false);

    for (int r = 0, c = 0; r < response.topicTags.count(); r++, c = 0) {
        SetColumn(_tagsDataModel, r, c++, response.topicTags.at(r).name);
        SetColumn(_tagsDataModel, r, c++, response.topicTags.at(r).value);
    }
    // Reset selection
    _ui->tagsTable->setSortingEnabled(true);
    _ui->tagsTable->sortByColumn(_tagsSortColumn, _tagsSortOrder);
    _ui->tagsTable->selectRow(selectedRow);
}

void SNSTopicDetailsDialog::CurrentTabChanged(const int index) const {
    switch (index) {
        case 0:
            _snsService->ListTopicAttributes(_topicArn);
            break;
        case 1:
            _snsService->ListTopicTags(_topicArn);
            break;
        case 2:
            _snsService->ListTopicSubscriptions(_topicArn);
            break;
        default:
            qCritical() << "Unknown index: " << index;
            break;
    }
}

void SNSTopicDetailsDialog::SetupAttributesTable() {

    // Headers  
    const QStringList attributeHeaders = QStringList() = {tr("Key"), tr("Value")};
    _attributesDataModel = new QStandardItemModel(this);
    _attributesDataModel->setHorizontalHeaderLabels(attributeHeaders);
    _attributesDataModel->setColumnCount(static_cast<int>(attributeHeaders.count()));

    // Proxy model for prefix filtering
    _attributesProxyModel = new PrefixFilterProxyModel(this);
    _attributesProxyModel->setSourceModel(_attributesDataModel);
    _ui->attributeTable->setModel(_attributesProxyModel);

    _ui->attributeTable->setShowGrid(true);
    _ui->attributeTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->attributeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->attributeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->attributeTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents); // name
    _ui->attributeTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch); //status

    // Buttons
    _ui->attributeRefreshButton->setText(nullptr);
    _ui->attributeRefreshButton->setIcon(IconUtils::GetIcon("refresh"));
    connect(_ui->attributeRefreshButton, &QPushButton::clicked, this, [this]() {
        _snsService->ListTopicAttributes(_topicArn);
    });
}

void SNSTopicDetailsDialog::SetupTagsTable() {

    // Headers  
    const QStringList tagsHeaders = QStringList() = {tr("Name"), tr("Value")};
    _tagsDataModel = new QStandardItemModel(this);
    _tagsDataModel->setHorizontalHeaderLabels(tagsHeaders);
    _tagsDataModel->setColumnCount(static_cast<int>(tagsHeaders.count()));

    // Proxy model for prefix filtering
    _tagsProxyModel = new PrefixFilterProxyModel(this);
    _tagsProxyModel->setSourceModel(_tagsDataModel);
    _ui->tagsTable->setModel(_tagsProxyModel);

    _ui->tagsTable->setShowGrid(true);
    _ui->tagsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->tagsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->tagsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->tagsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents); // name
    _ui->tagsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch); //status

    // Buttons
    _ui->tagAddButton->setText(nullptr);
    _ui->tagAddButton->setIcon(IconUtils::GetIcon("add"));

    _ui->tagRefreshButton->setText(nullptr);
    _ui->tagRefreshButton->setIcon(IconUtils::GetIcon("refresh"));
    connect(_ui->tagRefreshButton, &QPushButton::clicked, this, [this]() {
        _snsService->ListTopicTags(_topicArn);
    });
}

void SNSTopicDetailsDialog::SetupSubscriptionsTable() {

    // Headers  
    const QStringList subscriptionsHeaders = QStringList() = {tr("Id"), tr("Endpoint"), tr("Protocol"), tr("Owner")};
    _subscriptionsDataModel = new QStandardItemModel(this);
    _subscriptionsDataModel->setHorizontalHeaderLabels(subscriptionsHeaders);
    _subscriptionsDataModel->setColumnCount(static_cast<int>(subscriptionsHeaders.count()));

    // Proxy model for prefix filtering
    _subscriptionsProxyModel = new PrefixFilterProxyModel(this);
    _subscriptionsProxyModel->setSourceModel(_subscriptionsDataModel);
    _ui->subscriptionTable->setModel(_subscriptionsProxyModel);

    _ui->subscriptionTable->setShowGrid(true);
    _ui->subscriptionTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->subscriptionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->subscriptionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->subscriptionTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents); // name
    _ui->subscriptionTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch); //status
    _ui->subscriptionTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents); // name
    _ui->subscriptionTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents); // name

    // Buttons
    _ui->subscriptionAddButton->setText(nullptr);
    _ui->subscriptionAddButton->setIcon(IconUtils::GetIcon("add"));

    _ui->subscriptionRefreshButton->setText(nullptr);
    _ui->subscriptionRefreshButton->setIcon(IconUtils::GetIcon("refresh"));
    connect(_ui->subscriptionRefreshButton, &QPushButton::clicked, this, [this]() {
        _snsService->ListTopicSubscriptions(_topicArn);
    });
}

void SNSTopicDetailsDialog::UpdateTopicSubscriptions(const ListTopicSubscriptionsResponse &response) const {

    // Save selection
    const int selectedRow = _ui->subscriptionTable->selectionModel()->currentIndex().row();
    _ui->subscriptionTable->setSortingEnabled(false);

    for (int r = 0, c = 0; r < response.topicSubscriptions.count(); r++, c = 0) {
        SetColumn(_subscriptionsDataModel, r, c++, response.topicSubscriptions.at(r).id);
        SetColumn(_subscriptionsDataModel, r, c++, response.topicSubscriptions.at(r).endpoint);
        SetColumn(_subscriptionsDataModel, r, c++, response.topicSubscriptions.at(r).protocol);
        SetColumn(_subscriptionsDataModel, r, c++, response.topicSubscriptions.at(r).owner);
    }

    // Reset selection
    _ui->subscriptionTable->setSortingEnabled(true);
    _ui->subscriptionTable->sortByColumn(_subscriptionsSortColumn, _subscriptionsSortOrder);
    _ui->subscriptionTable->selectRow(selectedRow);
}

void SNSTopicDetailsDialog::SetupDefaultAttributesTab() {

    connect(_snsService, &SNSService::ReloadTopicDefaultAttributesSignal, this, [this]() {
        _snsService->ListTopicDefaultAttributes(_topicArn, "");
    });

    // Add button
    _ui->addDefaultAttributeButton->setText(nullptr);
    _ui->addDefaultAttributeButton->setIcon(IconUtils::GetIcon("add"));
    _ui->addDefaultAttributeButton->setToolTip(tr("Add default message attributes"));
    connect(_ui->addDefaultAttributeButton, &QPushButton::clicked, [this]() {
        if (SNSTopicDefaultAttributeDialog dialog; dialog.exec() == Accepted) {
            const QString key = dialog.GetKey();
            MessageAttribute attribute;
            attribute.name = key;
            attribute.dataType = "String";
            attribute.stringValue = dialog.GetValue();
            _snsService->AddTopicDefaultAttributes(_topicArn, key, attribute);
        }
    });

    _defaultAttributesModel = new QStandardItemModel();
    _ui->defaultAttributeTable->setModel(_defaultAttributesModel);

    // Table
    const QStringList headers = QStringList() = {tr("Key"), tr("Data Type"), tr("Value")};
    _defaultAttributesModel->setHorizontalHeaderLabels(headers);

    _ui->defaultAttributeTable->setShowGrid(true);
    _ui->defaultAttributeTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->defaultAttributeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->defaultAttributeTable->setSortingEnabled(true);
    _ui->defaultAttributeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->defaultAttributeTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    _ui->defaultAttributeTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    _ui->defaultAttributeTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    // Context menu
    _ui->defaultAttributeTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_ui->defaultAttributeTable, &QTableWidget::customContextMenuRequested, this, &SNSTopicDetailsDialog::ShowDefaultAttributeContextMenu);

    // Context menu
    _ui->defaultAttributeTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_ui->defaultAttributeTable, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {
        const QString key = _defaultAttributesModel->item(index.row(), 0)->text();
        const QString dataType = _defaultAttributesModel->item(index.row(), 1)->text();
        QString value = _defaultAttributesModel->item(index.row(), 2)->text();
        if (SNSTopicDefaultAttributeDialog dialog(key, value); dialog.exec() == Accepted) {
            value = dialog.GetValue();
            _snsService->UpdateTopicDefaultAttributes(_topicArn, key, value, dataType);
        }
    });

    // Get the default attribute list
    _snsService->ListTopicDefaultAttributes(_topicArn, "");
}

void SNSTopicDetailsDialog::UpdateDefaultAttributes(const SNSListQueueDefaultAttributesResponse &response) const {

    _defaultAttributesModel->removeRows(0, _defaultAttributesModel->rowCount());
    _ui->defaultAttributeTable->setSortingEnabled(false); // stop sorting
    int r = 0, c = 0;
    for (const auto &key: response.defaultAttributesCounters.keys()) {
        SNSMessageAttribute defaultAttribute = response.defaultAttributesCounters[key];
        SetColumn(_defaultAttributesModel, r, c++, key);
        SetColumn(_defaultAttributesModel, r, c++, MessageAttributeDataTypeToString(response.defaultAttributesCounters[key].dataType));
        SetColumn(_defaultAttributesModel, r, c, response.defaultAttributesCounters[key].stringValue);
        r++;
        c = 0;
    }
    _ui->defaultAttributeTable->setSortingEnabled(true);
    _defaultAttributesModel->sort(_defaultAttributeSortColumn, _defaultAttributeSortOrder);
}

void SNSTopicDetailsDialog::ShowDefaultAttributeContextMenu(const QPoint &pos) const {

    const QModelIndex index = _ui->defaultAttributeTable->indexAt(pos);
    if (!index.isValid()) {
        return;
    }

    const int row = index.row();

    QMenu menu;
    menu.setToolTipsVisible(true);
    QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit Default Attribute");
    editAction->setToolTip("Edit the queue default attribute");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Default Attribute");
    deleteAction->setToolTip("Delete the queue default attribute");

    if (const QAction *selectedAction = menu.exec(_ui->defaultAttributeTable->viewport()->mapToGlobal(pos)); selectedAction == editAction) {
        const QString key = _defaultAttributesModel->item(row, 0)->text();
        const QString dataType = _defaultAttributesModel->item(row, 1)->text();
        QString value = _defaultAttributesModel->item(row, 2)->text();
        if (SNSTopicDefaultAttributeDialog dialog(key, value); dialog.exec() == Accepted) {
            value = dialog.GetValue();
            _snsService->UpdateTopicDefaultAttributes(_topicArn, key, value, dataType);
        }
    } else if (selectedAction == deleteAction) {
        const QString key = _defaultAttributesModel->item(row, 0)->text();
        _snsService->DeleteTopicDefaultAttributes(_topicArn, key);
        _defaultAttributesModel->removeRow(row);
    }
}
