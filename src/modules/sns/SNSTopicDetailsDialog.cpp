#include <modules/sns/SNSTopicDetailsDialog.h>
#include "ui_SNSTopicDetailsDialog.h"

SNSTopicDetailsDialog::SNSTopicDetailsDialog(const QString &topicArn, QWidget *parent) : ::BaseDialog(parent), _ui(new Ui::SNSTopicDetailsDialog), topicArn(topicArn) {

    // Setup UI
    _ui->setupUi(this);
    connect(_ui->snsTopicDetailsButtonBox, &QDialogButtonBox::accepted, this, &SNSTopicDetailsDialog::HandleAccept);
    connect(_ui->snsTopicDetailsButtonBox, &QDialogButtonBox::rejected, this, &SNSTopicDetailsDialog::HandleReject);

    // SNS REST service
    _snsService = new SNSService();
    connect(_snsService, &SNSService::GetTopicDetailsSignal, this, &SNSTopicDetailsDialog::UpdateTopicDetails);
    connect(_snsService, &SNSService::ListTopicAttributesSignal, this, &SNSTopicDetailsDialog::UpdateTopicAttributes);

    // Tab widget
    connect(_ui->tabWidget, &QTabWidget::currentChanged, this, &SNSTopicDetailsDialog::CurrentTabChanged);
    _ui->tabWidget->setCurrentIndex(0);

    // Attributes table
    const QStringList attributeHeaders = QStringList() = {tr("Key"), tr("Value")};

    // Table
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

    // Load initial content
    SNSTopicDetailsDialog::LoadContent();
}

SNSTopicDetailsDialog::~SNSTopicDetailsDialog() {
    delete _ui;
}

void SNSTopicDetailsDialog::LoadContent() {
    _snsService->GetTopicDetails(topicArn);
    _snsService->ListTopicAttributes(topicArn);
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
    _ui->messageSizeEdit->setText(QString::number(response.size / 1024));
    _ui->createdEdit->setText(response.created.toString("yyyy-MM-dd hh:mm:ss"));
    _ui->modifiedEdit->setText(response.modified.toString("yyyy-MM-dd hh:mm:ss"));
}

void SNSTopicDetailsDialog::UpdateTopicAttributes(const ListTopicAttributesCountersResponse &response) const {
    const int selectedRow = _ui->attributeTable->selectionModel()->currentIndex().row();
    _ui->attributeTable->setSortingEnabled(false);

    for (int r = 0, c = 0; r < response.topicAttributes.count(); r++, c = 0) {
        SetColumn(_attributesDataModel, r, c++, response.topicAttributes.at(r).key);
        SetColumn(_attributesDataModel, r, c++, response.topicAttributes.at(r).value);
    }
    // Reset selection
    _ui->attributeTable->setSortingEnabled(true);
    //_ui->attributeTable->sortByColumn(_sortColumn, _sortOrder);
    _ui->attributeTable->selectRow(selectedRow);
}

void SNSTopicDetailsDialog::CurrentTabChanged(const int index) const {
    switch (index) {
        case 0:
            _snsService->ListTopicAttributes(topicArn);
            break;
        case 1:
            _snsService->ListTopicTags(topicArn);
            break;
        case 2:
            _snsService->ListTopicSubscriptions(topicArn);
            break;
        default:
            qCritical() << "Unknown index: " << index;
            break;
    }
}
