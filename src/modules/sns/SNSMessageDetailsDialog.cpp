
#include <QFileDialog>
#include <modules/sns/SNSMessageDetailsDialog.h>
#include "ui_SNSMessageDetailsDialog.h"
#include "components/Toast.h"

SNSMessageDetailsDialog::SNSMessageDetailsDialog(const QString &messageId, QWidget *parent) : QDialog(parent), _ui(new Ui::SNSMessageDetailsDialog), _messageId(messageId) {
    // Setup UI
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &SNSMessageDetailsDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &SNSMessageDetailsDialog::HandleReject);

    _snsService = new SNSService();

    _snsService->GetSnsMessageDetails(messageId);
    connect(_snsService, &SNSService::GetMessageDetailsSignal, this, &SNSMessageDetailsDialog::UpdateMessageDetails);

    const QStringList messageAttributeHeaders = QStringList() = {tr("Key"), tr("Value")};

    // Table
    _dataModel = new QStandardItemModel(this);
    _dataModel->setHorizontalHeaderLabels(messageAttributeHeaders);
    _dataModel->setColumnCount(static_cast<int>(messageAttributeHeaders.count()));

    // Message attribute table
    _ui->attributeTable->setModel(_dataModel);
    _ui->attributeTable->setShowGrid(true);
    _ui->attributeTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->attributeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    //    _ui->attributeTable->setHorizontalHeaderLabels(messageAttributeHeaders);
    _ui->attributeTable->setSortingEnabled(true);
    _ui->attributeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->attributeTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    _ui->attributeTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    // Set body tab
    _ui->tabWidget->setCurrentIndex(0);

    // Pretty print
    _ui->prettyPushButton->setText(nullptr);
    _ui->prettyPushButton->setIcon(IconUtils::GetIcon("dark", "pretty"));
    //connect(_ui->prettyPushButton, &QPushButton::toggled)

    // Save button
    _ui->saveButton->setText(nullptr);
    _ui->saveButton->setIcon(IconUtils::GetIcon("save"));
    _ui->saveButton->setToolTip("Save the message to a local file");
    connect(_ui->saveButton, &QPushButton::clicked, this, &SNSMessageDetailsDialog::SaveToFile);

    // Window button
    _ui->windowButton->setText(nullptr);
    _ui->windowButton->setIcon(IconUtils::GetIcon("extern-window"));
    connect(_ui->windowButton, &QPushButton::clicked, this, [messageId]() {
        if (SNSMessageWindow dialog(messageId); dialog.exec() == Accepted) {
            logInfo << "Message window closed";
        }
    });
}

SNSMessageDetailsDialog::~SNSMessageDetailsDialog() {
    delete _ui;
}

void SNSMessageDetailsDialog::UpdateMessageDetails(const SNSGetMessageDetailsResponse &response) const {
    _ui->regionEdit->setText(response.region);
    _ui->messageIdEdit->setText(response.messageId);
    _ui->topicArnEdit->setText(response.topicArn);
    _ui->contentTypeEdit->setText(response.contentType);
    _ui->createdEdit->setText(DateTimeUtils::GetDateTimeFormat(response.created));
    _ui->modifiedEdit->setText(DateTimeUtils::GetDateTimeFormat(response.modified));

    // Body
    _ui->textEditor->SetText(response.message);

    // Attributes
    //_ui->attributeTable->setRowCount(0);
    _ui->attributeTable->setSortingEnabled(false); // stop sorting
    //    _ui->attributeTable->sortItems(-1);
    for (int r = 0; r < response.messageAttributes.count(); r++) {
        // _ui->attributeTable->insertRow(r);
        // _ui->attributeTable->setItem(r, 0, new QTableWidgetItem(response.messageAttributes.at(r).name));
        // _ui->attributeTable->setItem(r, 1, new QTableWidgetItem(response.messageAttributes.at(r).stringValue));
    }
}

void SNSMessageDetailsDialog::on_prettyPushButton_toggled(const bool checked) const {
    _ui->textEditor->SetPrettyPrint(checked);
}

void SNSMessageDetailsDialog::SaveToFile() const {

    if (const QString fileName = SelectFilename(); !fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::critical(nullptr, "Error", "Could not open file:" + fileName);
        }
        const QString message = _ui->textEditor->GetText();
        const long count = file.write(message.toUtf8());
        file.close();

        // Message
        new Awsmock::Components::ToastOverlay("Message saved.\nFilename: " + fileName);
        logInfo << "SNS message saved to file: " << fileName << ", fileSize: " << count;
    }
}

QString SNSMessageDetailsDialog::SelectFilename() {

    // Create a QFileDialog set to select existing files
    const auto filter = "JSON Files (*.json);All Files (*.*)";
    const auto defaultDir = Configuration::instance().GetValue<QString>("ui.default-directory.SNSSaveBodyToFile", "/usr/local/file.json");

    if (const QString filePath = QFileDialog::getSaveFileName(nullptr, "Open JSON Configuration File", defaultDir, filter); !filePath.isEmpty()) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadWrite)) {
            QMessageBox::critical(nullptr, "Error", "Could not open file:" + filePath);
            return {};
        }
        file.close();
        Configuration::instance().SetValue<QString>("ui.default-directory.SNSSaveBodyToFile", QFileInfo(filePath).absolutePath());
        logDebug << "SNS message file path: " << filePath;
        return filePath;
    }
    return {};
}

void SNSMessageDetailsDialog::HandleAccept() {
    accept();
}

void SNSMessageDetailsDialog::HandleReject() {
    accept();
}
