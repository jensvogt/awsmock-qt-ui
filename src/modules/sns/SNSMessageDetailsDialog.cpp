
#include <modules/sns/SNSMessageDetailsDialog.h>
#include "ui_SNSMessageDetailsDialog.h"

SNSMessageDetailsDialog::SNSMessageDetailsDialog(const QString &messageId, QWidget *parent): QDialog(parent), _ui(new Ui::SNSMessageDetailsDialog), _messageId(messageId) {

    _ui->setupUi(this);

    _snsService = new SNSService();

    _snsService->GetSnsMessageDetails(messageId);
    connect(_snsService, &SNSService::GetMessageDetailsSignal, this, &SNSMessageDetailsDialog::UpdateMessageDetails);

    const QStringList messageAttributeHeaders = QStringList() << tr("Key")
                                                        << tr("Value");

    QStringList systemAttributeHeaders = QStringList() << tr("Key")
                                                       << tr("Value");

    // Message attribute table
    _ui->attributeTable->setColumnCount(messageAttributeHeaders.count());
    _ui->attributeTable->setShowGrid(true);
    _ui->attributeTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->attributeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->attributeTable->setHorizontalHeaderLabels(messageAttributeHeaders);
    _ui->attributeTable->setSortingEnabled(true);
    _ui->attributeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->attributeTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    _ui->attributeTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    // Set body tab
    _ui->tabWidget->setCurrentIndex(0);
}

SNSMessageDetailsDialog::~SNSMessageDetailsDialog() {
    delete _ui;
}

void SNSMessageDetailsDialog::UpdateMessageDetails(const SNSGetMessageDetailsResponse &response) const {
    _ui->regionEdit->setText(response.region);
    _ui->messageIdEdit->setText(response.messageId);
    _ui->topicArnEdit->setText(response.topicArn);
    _ui->contentTypeEdit->setText(response.contentType);
    _ui->createdEdit->setText(response.created.toString());
    _ui->modifiedEdit->setText(response.modified.toString());

    // Body
    _ui->bodyPlainTextEdit->setPlainText(response.message);

    // Attributes
    _ui->attributeTable->setRowCount(0);
    _ui->attributeTable->setSortingEnabled(false); // stop sorting
    _ui->attributeTable->sortItems(-1);
    for(int r = 0; r < response.messageAttributes.count(); r++) {
        _ui->attributeTable->insertRow(r);
        _ui->attributeTable->setItem(r, 0, new QTableWidgetItem(response.messageAttributes.at(r).name));
        _ui->attributeTable->setItem(r, 1, new QTableWidgetItem(response.messageAttributes.at(r).stringValue));
    }
}

void SNSMessageDetailsDialog::on_prettyPushButton_toggled(bool checked) const {
    if(checked) {
        const QByteArray body = _ui->bodyPlainTextEdit->toPlainText().toUtf8();
        const QJsonDocument jDoc = QJsonDocument::fromJson(body);
        _ui->bodyPlainTextEdit->clear();
        _ui->bodyPlainTextEdit->setPlainText(jDoc.toJson(QJsonDocument::Indented));
    } else {
        const QByteArray body = _ui->bodyPlainTextEdit->toPlainText().toUtf8();
        const QJsonDocument jDoc = QJsonDocument::fromJson(body);
        _ui->bodyPlainTextEdit->clear();
        _ui->bodyPlainTextEdit->setPlainText(jDoc.toJson(QJsonDocument::Compact));
    }
}
