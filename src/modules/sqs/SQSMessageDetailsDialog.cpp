
#include <modules/sqs/SQSMessageDetailsDialog.h>
#include "ui_SQSMessageDetailsDialog.h"

SQSMessageDetailsDialog::SQSMessageDetailsDialog(const QString &messageId, QWidget *parent): QDialog(parent), _ui(new Ui::SQSMessageDetailsDialog), _messageId(messageId) {

    _ui->setupUi(this);

    _sqsService = new SQSService();

    _sqsService->GetSqsMessageDetails(messageId);
    connect(_sqsService, &SQSService::GetSqsMessageDetailsSignal, this, &SQSMessageDetailsDialog::UpdateMessageDetails);

    QStringList messageAttributeHeaders = QStringList() << tr("Key")
                                                        << tr("Value");

    QStringList systemAttributeHeaders = QStringList() << tr("Key")
                                                       << tr("Value");

    // Message attribute table
    _ui->messageAttributeTable->setColumnCount(messageAttributeHeaders.count());
    _ui->messageAttributeTable->setShowGrid(true);
    _ui->messageAttributeTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->messageAttributeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->messageAttributeTable->setHorizontalHeaderLabels(messageAttributeHeaders);
    _ui->messageAttributeTable->setSortingEnabled(true);
    _ui->messageAttributeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->messageAttributeTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    _ui->messageAttributeTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    // System attribute table
    _ui->systemAttributeTable->setColumnCount(systemAttributeHeaders.count());
    _ui->systemAttributeTable->setShowGrid(true);
    _ui->systemAttributeTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->systemAttributeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->systemAttributeTable->setHorizontalHeaderLabels(systemAttributeHeaders);
    _ui->systemAttributeTable->setSortingEnabled(true);
    _ui->systemAttributeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->systemAttributeTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    _ui->systemAttributeTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);

    // Set body tab
    _ui->tabWidget->setCurrentIndex(0);
}

SQSMessageDetailsDialog::~SQSMessageDetailsDialog() {
    delete _ui;
}

void SQSMessageDetailsDialog::UpdateMessageDetails(const SQSGetMessageDetailsResponse &response) {
    _ui->regionEdit->setText(response.region);
    _ui->messageIdEdit->setText(response.messageId);
    _ui->receiptHandleEdit->setText(response.receiptHandle);
    _ui->md5BodyEdit->setText(response.md5Body);
    _ui->md5AttributesEdit->setText(response.md5Attributes);
    _ui->createdEdit->setText(response.created.toString());
    _ui->modifiedEdit->setText(response.modified.toString());

    // Body
    _ui->bodyPlainTextEdit->setPlainText(response.body);

    // Message attributes
    _ui->messageAttributeTable->setRowCount(0);
    _ui->messageAttributeTable->setSortingEnabled(false); // stop sorting
    _ui->messageAttributeTable->sortItems(-1);
    for(int r = 0; r < response.messageAttributes.count(); r++) {
        _ui->messageAttributeTable->insertRow(r);
        _ui->messageAttributeTable->setItem(r, 0, new QTableWidgetItem(response.messageAttributes.at(r).name));
        _ui->messageAttributeTable->setItem(r, 1, new QTableWidgetItem(response.messageAttributes.at(r).stringValue));
    }

    // System attributes
    _ui->systemAttributeTable->setRowCount(0);
    _ui->systemAttributeTable->setSortingEnabled(false); // stop sorting
    _ui->systemAttributeTable->sortItems(-1);
    for(int r = 0; r < response.messageAttributes.count(); r++) {
        _ui->systemAttributeTable->insertRow(r);
        _ui->systemAttributeTable->setItem(r, 0, new QTableWidgetItem(response.attributes.at(r).key));
        _ui->systemAttributeTable->setItem(r, 1, new QTableWidgetItem(response.attributes.at(r).value));
    }
}

void SQSMessageDetailsDialog::on_prettyPushButton_toggled(bool checked)
{
    if(checked) {
        QByteArray body = _ui->bodyPlainTextEdit->toPlainText().toUtf8();
        QJsonDocument jDoc = QJsonDocument::fromJson(body);
        _ui->bodyPlainTextEdit->clear();
        _ui->bodyPlainTextEdit->setPlainText(jDoc.toJson(QJsonDocument::Indented));
    } else {
        QByteArray body = _ui->bodyPlainTextEdit->toPlainText().toUtf8();
        QJsonDocument jDoc = QJsonDocument::fromJson(body);
        _ui->bodyPlainTextEdit->clear();
        _ui->bodyPlainTextEdit->setPlainText(jDoc.toJson(QJsonDocument::Compact));
    }
}

