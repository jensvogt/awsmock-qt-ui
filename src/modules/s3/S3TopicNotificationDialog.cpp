//
// Created by vogje01 on 18/07/26.
//

#include <modules/s3/S3TopicNotificationDialog.h>
#include "ui_S3TopicNotificationDialog.h"

S3TopicNotificationDialog::S3TopicNotificationDialog(QWidget *parent) : BaseDialog(parent), _ui(new Ui::S3TopicNotificationDialog) {
    Initialize();
}

S3TopicNotificationDialog::S3TopicNotificationDialog(const TopicNotification &notification, QWidget *parent) : BaseDialog(parent), _ui(new Ui::S3TopicNotificationDialog) {

    Initialize();

    _ui->idEdit->setText(notification.id);

    QStringList selectedEvents;
    for (const auto &event: notification.events) {
        selectedEvents.append(S3NotificationEventToString(event));
    }
    _ui->eventSelectLists->SetSelected(selectedEvents);

    for (auto it = notification.filterRules.begin(); it != notification.filterRules.cend(); ++it) {
        const int row = _filterDataModel->rowCount();
        SetColumn(_filterDataModel, row, 0, S3FilterNameToString(it->name));
        SetColumn(_filterDataModel, row, 1, it->filterValue);
    }
}

void S3TopicNotificationDialog::Initialize() {

    _snsService = new SNSService();

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &S3TopicNotificationDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &S3TopicNotificationDialog::HandleReject);

    _ui->topicArnCombo->setEditable(true);
    _ui->topicArnCombo->setInsertPolicy(QComboBox::NoInsert);
    auto *completer = new QCompleter(_ui->topicArnCombo->model(), this);
    completer->setFilterMode(Qt::MatchContains);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    _ui->topicArnCombo->setCompleter(completer);
    _snsService->ListTopicArns();
    connect(_snsService, &SNSService::ListTopicArnsSignal, this, [this](const QList<QString> &response) {
        _ui->topicArnCombo->addItems(response);
        if (!response.isEmpty()) {
            _selectedTopicArn = response[0];
        }
    });
    connect(_ui->topicArnCombo, &QComboBox::currentTextChanged, this, [this](const QString &topicArn) {
        _selectedTopicArn = topicArn;
    });

    _ui->eventSelectLists->SetAvailable(_s3Events);
    connect(_ui->eventSelectLists, &Awsmock::Components::SelectLists::SelectChangedSignal, this, [this](const QStringList &s3Events) {
        _selectedS3Event = s3Events;
    });

    _ui->filterAddButton->setText(nullptr);
    _ui->filterAddButton->setIcon(IconUtils::GetIcon("add"));

    const QStringList headers = {"Type", "Value"};
    _filterDataModel = new QStandardItemModel(this);
    _filterDataModel->setHorizontalHeaderLabels(headers);
    _filterDataModel->setColumnCount(static_cast<int>(headers.count()));
    _ui->filterTableView->setModel(_filterDataModel);
    _ui->filterTableView->setShowGrid(true);
    _ui->filterTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->filterTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->filterTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->filterTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    _ui->filterTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
}

S3TopicNotificationDialog::~S3TopicNotificationDialog() {
    delete _snsService;
    delete _ui;
}

void S3TopicNotificationDialog::HandleAccept() {
    if (_selectedTopicArn.isEmpty()) {
        QMessageBox::warning(this, tr("S3 Topic Notification Dialog"), tr("Topic ARN cannot be empty!"));
        return;
    }
    if (_selectedS3Event.isEmpty()) {
        QMessageBox::warning(this, tr("S3 Topic Notification Dialog"), tr("S3 event cannot be empty!"));
        return;
    }
    accept();
}

void S3TopicNotificationDialog::HandleReject() {
    reject();
}
