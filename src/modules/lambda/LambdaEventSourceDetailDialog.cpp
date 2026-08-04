//
// Created by vogje01 on 11/25/25.
//

#include <modules/lambda/LambdaEventSourceDetailDialog.h>
#include "ui_LambdaEventSourceDetailDialog.h"

LambdaEventSourceDetailDialog::LambdaEventSourceDetailDialog(const QString &type, const QString &eventSourceArn, const long batchSize, const long maximumBatchingWindowInSeconds, QString uuid, QWidget *parent)
    : QDialog(parent), _ui(new Ui::LambdaEventSourceDetailDialog), _type(type.isEmpty() ? "S3" : type), _eventSourceArn(eventSourceArn), _batchSize(batchSize), _maximumBatchingWindowInSeconds(maximumBatchingWindowInSeconds), _uuid(std::move(uuid)) {

    // REST services, used to fetch the event source ARNs matching the selected type
    _s3Service = new S3Service();
    _sqsService = new SQSService();
    _snsService = new SNSService();

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &LambdaEventSourceDetailDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &LambdaEventSourceDetailDialog::HandleReject);

    // Type field
    _ui->typeComboBox->addItems({"S3", "SQS", "SNS"});
    _ui->typeComboBox->setCurrentText(_type);

    // Batch size field
    _ui->batchSizeSpinBox->setRange(1, 10000);
    _ui->batchSizeSpinBox->setValue(static_cast<int>(batchSize));

    // Maximum batching window field
    _ui->maxBatchingWindowSpinBox->setRange(0, 300);
    _ui->maxBatchingWindowSpinBox->setValue(static_cast<int>(maximumBatchingWindowInSeconds));

    // Fill in the event source ARN combo box
    connect(_s3Service, &S3Service::ListBucketArnsSignal, this, [this](const QList<QString> &arns) {
        _ui->eventSourceArnComboBox->addItems(arns);
        _ui->eventSourceArnComboBox->setCurrentText(_eventSourceArn);
    });
    connect(_sqsService, &SQSService::ListQueueArnsSignal, this, [this](const QList<QString> &arns) {
        _ui->eventSourceArnComboBox->addItems(arns);
        _ui->eventSourceArnComboBox->setCurrentText(_eventSourceArn);
    });
    connect(_snsService, &SNSService::ListTopicArnsSignal, this, [this](const QList<QString> &arns) {
        _ui->eventSourceArnComboBox->addItems(arns);
        _ui->eventSourceArnComboBox->setCurrentText(_eventSourceArn);
    });
    PopulateEventSourceArns(_type);

    // Connect fields
    connect(_ui->typeComboBox, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        _type = text;
        PopulateEventSourceArns(text);
    });
    connect(_ui->eventSourceArnComboBox, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        _eventSourceArn = text;
    });
    connect(_ui->batchSizeSpinBox, &QSpinBox::valueChanged, this, [this](const int value) {
        _batchSize = value;
    });
    connect(_ui->maxBatchingWindowSpinBox, &QSpinBox::valueChanged, this, [this](const int value) {
        _maximumBatchingWindowInSeconds = value;
    });
}

LambdaEventSourceDetailDialog::~LambdaEventSourceDetailDialog() {
    delete _s3Service;
    delete _sqsService;
    delete _snsService;
    delete _ui;
}

void LambdaEventSourceDetailDialog::PopulateEventSourceArns(const QString &type) {
    _ui->eventSourceArnComboBox->clear();
    if (type == "S3") {
        _s3Service->ListBucketArns();
    } else if (type == "SQS") {
        _sqsService->ListQueueArns();
    } else if (type == "SNS") {
        _snsService->ListTopicArns();
    }
}

void LambdaEventSourceDetailDialog::HandleAccept() {
    accept();
}

void LambdaEventSourceDetailDialog::HandleReject() {
    reject();
}

QString LambdaEventSourceDetailDialog::GetType() {
    return _type;
}

QString LambdaEventSourceDetailDialog::GetEventSourceArn() {
    return _eventSourceArn;
}

long LambdaEventSourceDetailDialog::GetBatchSize() {
    return _batchSize;
}

long LambdaEventSourceDetailDialog::GetMaximumBatchingWindowInSeconds() {
    return _maximumBatchingWindowInSeconds;
}

QString LambdaEventSourceDetailDialog::GetUuid() {
    return _uuid;
}
