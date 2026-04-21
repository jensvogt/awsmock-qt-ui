
#include <modules/sqs/SQSMessageDetailsDialog.h>
#include "ui_SQSMessageDetailsDialog.h"

SQSMessageDetailsDialog::SQSMessageDetailsDialog(const QString &messageId, QWidget *parent) : QDialog(parent),
                                                                                              _ui(new Ui::SQSMessageDetailsDialog), _messageId(messageId) {
    _ui->setupUi(this);

    _sqsService = new SQSService();

    _sqsService->GetSqsMessageDetails(messageId);
    connect(_sqsService, &SQSService::GetSqsMessageDetailsSignal, this, &SQSMessageDetailsDialog::UpdateMessageDetails);

    const QStringList messageAttributeHeaders = {tr("Key"), tr("Value")};
    const QStringList systemAttributeHeaders = {tr("Key"), tr("Value")};

    // Message attributes table
    _ui->messageAttributeTable->setColumnCount(static_cast<int>(messageAttributeHeaders.count()));
    _ui->messageAttributeTable->setShowGrid(true);
    _ui->messageAttributeTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->messageAttributeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->messageAttributeTable->setHorizontalHeaderLabels(messageAttributeHeaders);
    _ui->messageAttributeTable->setSortingEnabled(true);
    _ui->messageAttributeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->messageAttributeTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    _ui->messageAttributeTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    // System attributes table
    _ui->systemAttributeTable->setColumnCount(static_cast<int>(systemAttributeHeaders.count()));
    _ui->systemAttributeTable->setShowGrid(true);
    _ui->systemAttributeTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->systemAttributeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->systemAttributeTable->setHorizontalHeaderLabels(systemAttributeHeaders);
    _ui->systemAttributeTable->setSortingEnabled(true);
    _ui->systemAttributeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->systemAttributeTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    _ui->systemAttributeTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    // Set body tab
    _ui->tabWidget->setCurrentIndex(0);

    // Pretty print button
    _ui->prettyButton->setCheckable(true);
    _ui->prettyButton->setChecked(_ui->bodyTextWidget->GetPrettyPrint());
    _ui->prettyButton->setText(nullptr);
    _ui->prettyButton->setIcon(IconUtils::GetIcon("pretty"));
    connect(_ui->prettyButton, &QPushButton::toggled, this, [this](const bool checked) {
        _ui->bodyTextWidget->SetPrettyPrint(checked);
    });

    // Refresh button
    _ui->refreshButton->setText(nullptr);
    _ui->refreshButton->setIcon(IconUtils::GetIcon("refresh"));
    connect(_ui->prettyButton, &QPushButton::toggled, this, [this](const bool checked) {
        _sqsService->GetSqsMessageDetails(_messageId);
    });
}

SQSMessageDetailsDialog::~SQSMessageDetailsDialog() {
    delete _ui;
}

void SQSMessageDetailsDialog::UpdateMessageDetails(const SQSGetMessageDetailsResponse &response) const {
    _ui->regionEdit->setText(response.region);
    _ui->queueEdit->setText(response.queueName);
    _ui->messageIdEdit->setText(response.messageId);
    _ui->receiptHandleEdit->setText(response.receiptHandle);
    _ui->md5BodyEdit->setText(response.md5OfBody);
    _ui->md5AttributesEdit->setText(response.md5OfMessageAttributes);
    _ui->md5SystemAttributesEdit->setText(response.md5OfSystemAttributes);
    _ui->contentTypeEdit->setText(response.contentType);
    _ui->createdEdit->setText(response.created.toString());
    _ui->modifiedEdit->setText(response.modified.toString());

    // Body
    _ui->bodyTextWidget->SetText(response.body);

    // Message attributes
    _ui->messageAttributeTable->setRowCount(0);
    _ui->messageAttributeTable->setSortingEnabled(false); // stop sorting
    _ui->messageAttributeTable->sortItems(-1);
    for (int r = 0; r < response.messageAttributes.count(); r++) {
        _ui->messageAttributeTable->insertRow(r);
        _ui->messageAttributeTable->setItem(r, 0, new QTableWidgetItem(response.messageAttributes.at(r).name));
        _ui->messageAttributeTable->setItem(r, 1, new QTableWidgetItem(response.messageAttributes.at(r).stringValue));
    }

    // System attributes
    _ui->systemAttributeTable->setRowCount(0);
    _ui->systemAttributeTable->setSortingEnabled(false); // stop sorting
    _ui->systemAttributeTable->sortItems(-1);
    for (int r = 0; r < response.attributes.count(); r++) {
        _ui->systemAttributeTable->insertRow(r);
        _ui->systemAttributeTable->setItem(r, 0, new QTableWidgetItem(response.attributes.at(r).key));
        _ui->systemAttributeTable->setItem(r, 1, new QTableWidgetItem(response.attributes.at(r).value));
    }
}
