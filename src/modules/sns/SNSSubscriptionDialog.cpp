//
// Created by jensv on 28/04/2026.
//

#include <modules/sns/SNSSubscriptionDialog.h>
#include "ui_SNSSubscriptionDialog.h"

SNSSubscriptionDialog::SNSSubscriptionDialog(const QString &topicArn, QWidget *parent) : QDialog(parent), _ui(new Ui::SNSSubscriptionDialog), _topicArn(topicArn) {

    // Initialize
    Initialize();
}

SNSSubscriptionDialog::SNSSubscriptionDialog(const QString &topicArn, const QString &subscriptionArn, QWidget *parent) : QDialog(parent), _ui(new Ui::SNSSubscriptionDialog), _topicArn(topicArn) {

    // Initialize
    Initialize();

    // Get the subscription
    _snsService->GetSubscription(topicArn, subscriptionArn);
    connect(_snsService, &SNSService::GetSubscriptionSignal, this, [this](const SNSSubscriptionResponse &response) {
        _snsTopicSubscription = response.snsTopicSubscription;
        _ui->targetCombo->setCurrentText(_snsTopicSubscription.endpoint);
        _ui->protocolCombo->setCurrentText(_snsTopicSubscription.protocol);
    });
}

SNSSubscriptionDialog::~SNSSubscriptionDialog() {
    delete _snsService;
    delete _sqsService;
    delete _ui;
}

void SNSSubscriptionDialog::Initialize() {

    // Setup services
    _snsService = new SNSService;
    _sqsService = new SQSService;

    // Setup UI
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &SNSSubscriptionDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &SNSSubscriptionDialog::HandleReject);

    // Setup target combo
    _ui->targetCombo->setEditable(true);
    _ui->targetCombo->setInsertPolicy(QComboBox::NoInsert);
    auto *completer = new QCompleter(_ui->targetCombo->model(), this);
    completer->setFilterMode(Qt::MatchContains);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    _ui->targetCombo->setCompleter(completer);

    // Setup protocol combo
    const QStringList protocols = {"SQS", "SNS", "Lambda", "HTML"};
    _ui->protocolCombo->addItems(protocols);
    connect(_ui->protocolCombo, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        if (!text.isEmpty() && text == "SQS") {
            LoadSQSQueues();
        }
    });

    // Default is SQS queues
    LoadSQSQueues();

    // Set owner
    _ui->ownerEdit->setText(Configuration::instance().GetValue<QString>("aws.user"));
}

void SNSSubscriptionDialog::LoadSQSQueues() {
    _sqsService->ListQueueArns();
    connect(_sqsService, &SQSService::ListQueueArnsSignal, this, [this](const QStringList &targets) {
        _ui->targetCombo->addItems(targets);
        if (_snsTopicSubscription.protocol == "sqs") {
            _ui->targetCombo->setCurrentText(_snsTopicSubscription.endpoint);
        }
    });
}

void SNSSubscriptionDialog::HandleAccept() {
    if (_ui->protocolCombo->currentText().isEmpty()) {
        QMessageBox::warning(this, "Add subscription", "Protocol can't be empty.");
        return;
    }
    if (_ui->targetCombo->currentText().isEmpty()) {
        QMessageBox::warning(this, "Add subscription", "Endpoint can't be empty.");
        return;
    }
    SNSTopicSubscription subscription;
    subscription.protocol = _ui->protocolCombo->currentText();
    subscription.endpoint = _ui->targetCombo->currentText();
    subscription.owner = Configuration::instance().GetValue<QString>("aws.user");
    subscription.region = Configuration::instance().GetValue<QString>("aws.region");

    // Add to topic
    _snsService->AddSubscription(_topicArn, subscription);
    connect(_snsService, &SNSService::AddTopicSubscriptionSignal, this, [this]() {
        accept();
    });
}

void SNSSubscriptionDialog::HandleReject() {
    reject();
}
