//
// Created by vogje01 on 11/9/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SQSMessageAddDialog.h" resolved

#include <modules/sqs/SQSMessageAddDialog.h>

#include <utility>

#include "ui_SQSMessageAddDialog.h"

SQSMessageAddDialog::SQSMessageAddDialog(QString queueUrl, QWidget *parent) : BaseDialog(parent), _ui(new Ui::SQSMessageAddDialog), _queueUrl(std::move(queueUrl)) {

    // Connect service events
    _sqsService = new SQSService();
    connect(_sqsService, &SQSService::SendMessagesSignal, this, &SQSMessageAddDialog::HandleSendMessageSignal);

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &SQSMessageAddDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &SQSMessageAddDialog::HandleReject);

    // Pretty print button
    _ui->prettyButton->setCheckable(true);
    _ui->prettyButton->setText(nullptr);
    _ui->prettyButton->setIcon(IconUtils::GetIcon("pretty"));
    connect(_ui->prettyButton, &QPushButton::toggled, this, &SQSMessageAddDialog::HandlePrettyButton);

    // Browse button
    _ui->browseButton->setText(nullptr);
    _ui->browseButton->setIcon(IconUtils::GetIcon("browse"));
    connect(_ui->browseButton, &QPushButton::clicked, this, &SQSMessageAddDialog::HandleBrowseButton);

    // Add attribute button
    _ui->addAttributeButton->setText(nullptr);
    _ui->addAttributeButton->setIcon(IconUtils::GetIcon("add"));
    connect(_ui->addAttributeButton, &QPushButton::clicked, this, &SQSMessageAddDialog::HandleAddAttributeButton);

    // Attribute table
    const QStringList headers = QStringList() = {tr("Key"), tr("Value")};

    _ui->tableWidget->setColumnCount(static_cast<int>(headers.count()));
    _ui->tableWidget->setShowGrid(true);
    _ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->tableWidget->setHorizontalHeaderLabels(headers);
    _ui->tableWidget->setSortingEnabled(true);
    _ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    _ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);

    // Set default tab
    _ui->tabWidget->setCurrentIndex(0);
}

SQSMessageAddDialog::~SQSMessageAddDialog() {
    delete _ui;
}

void SQSMessageAddDialog::HandleAccept() const {
    SQSSendMessageRequest request;
    request.region = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");
    request.queueUrl = _queueUrl;
    request.body = _ui->bodyEdit->toPlainText().toUtf8();

    const int rows = _ui->tableWidget->rowCount();

    for (int r = 0; r < rows; ++r) {
        MessageAttribute messageAttribute;
        messageAttribute.dataType = STRING;

        const QTableWidgetItem *key = _ui->tableWidget->item(r, 0);
        const QTableWidgetItem *value = _ui->tableWidget->item(r, 1);
        messageAttribute.stringValue = value->text().toUtf8();
        request.messageAttributes[key->text().toStdString()] = messageAttribute;
    }
    _sqsService->SendMessage(request);
}

void SQSMessageAddDialog::HandleSendMessageSignal(const SQSSendMessageResponse &response) {

    QMessageBox::information(nullptr, "Info", "Message send with messageId: " + response.messageId);
    accept();
}

void SQSMessageAddDialog::HandleReject() {
    accept();
}

void SQSMessageAddDialog::HandleBrowseButton() const {

    // Create a QFileDialog set to select existing files
    const auto filter = "All Files (*.*)";
    const auto defaultDir = Configuration::instance().GetValue<QString>("ui.default-directory", "/usr/local/awsmock-qt-ui");

    if (const QString filePath = QFileDialog::getOpenFileName(nullptr, "Open JSON Input File", defaultDir, filter); !filePath.isEmpty()) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(nullptr, "Error", "Could not open file:" + filePath);
            return;
        }

        const QByteArray jsonData = file.readAll();
        file.close();

        // Set the body
        _ui->bodyEdit->setText(QString::fromUtf8(jsonData));
        Configuration::instance().SetValue<QString>("ui.default-directory", QFileInfo(filePath).absolutePath());
    }
}

void SQSMessageAddDialog::HandlePrettyButton(const bool checked) const {
    if (checked) {
        const QByteArray body = _ui->bodyEdit->toPlainText().toUtf8();
        const QJsonDocument jDoc = QJsonDocument::fromJson(body);
        _ui->bodyEdit->clear();
        _ui->bodyEdit->setPlainText(jDoc.toJson(QJsonDocument::Indented));
    } else {
        const QByteArray body = _ui->bodyEdit->toPlainText().toUtf8();
        const QJsonDocument jDoc = QJsonDocument::fromJson(body);
        _ui->bodyEdit->clear();
        _ui->bodyEdit->setPlainText(jDoc.toJson(QJsonDocument::Compact));
    }
}

void SQSMessageAddDialog::HandleAddAttributeButton() const {

    QDialog dialog;
    dialog.setWindowTitle("Add attribute");

    QFormLayout form(&dialog);

    // Field 1
    const auto keyEdit = new QLineEdit(&dialog);
    form.addRow("Key:", keyEdit);

    // Field 2
    const auto valueEdit = new QLineEdit(&dialog);
    form.addRow("Value:", valueEdit);

    // Buttons
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == Accepted) {
        const int row = _ui->tableWidget->rowCount();
        _ui->tableWidget->insertRow(row);
        SetColumn(_ui->tableWidget, row, 0, keyEdit->text());
        SetColumn(_ui->tableWidget, row, 1, valueEdit->text());
    }
}
