//
// Created by jensv on 25/04/2026.
//

#include <modules/s3/S3QueueNotificationDialog.h>
#include "ui_S3QueueNotificationDialog.h"

S3QueueNotificationDialog::S3QueueNotificationDialog(QWidget *parent) : BaseDialog(parent), _ui(new Ui::S3QueueNotificationDialog) {

    // Initialization
    Initialize();
}

S3QueueNotificationDialog::S3QueueNotificationDialog(const QueueNotification &notification, QWidget *parent) : BaseDialog(parent), _ui(new Ui::S3QueueNotificationDialog) {

    // Initialization
    Initialize();

    // Set ID
    _ui->idEdit->setText(notification.id);

    // Events
    QStringList selectedEvents;
    for (const auto &event: notification.events) {
        selectedEvents.append(S3NotificationEventToString(event));
    }
    _ui->eventSelectLists->SetSelected(selectedEvents);

    // Filters
    for (auto it = notification.filterRules.begin(); it != notification.filterRules.cend(); ++it) {
        const int row = _filterDataModel->rowCount();
        SetColumn(_filterDataModel, row, 0, S3FilterNameToString(it->name));
        SetColumn(_filterDataModel, row, 1, it->filterValue);
    }
}

void S3QueueNotificationDialog::Initialize() {

    // SQS service
    _sqsService = new SQSService();

    // Setup UI
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &S3QueueNotificationDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &S3QueueNotificationDialog::HandleReject);

    // Fill SQS queue combo box (all)
    _ui->queueArnCombo->setEditable(true);
    _ui->queueArnCombo->setInsertPolicy(QComboBox::NoInsert);
    auto *completer = new QCompleter(_ui->queueArnCombo->model(), this);
    completer->setFilterMode(Qt::MatchContains);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    _ui->queueArnCombo->setCompleter(completer);
    _sqsService->ListQueueArns();
    connect(_sqsService, &SQSService::ListQueueArnsSignal, this, [this](const QList<QString> &response) {
        _ui->queueArnCombo->addItems(response);
        _selectedQueueArn = response[0];
    });
    connect(_ui->queueArnCombo, &QComboBox::currentTextChanged, this, [this](const QString &queueArn) {
        _selectedQueueArn = queueArn;
    });

    // Fill in S3 events
    _ui->eventSelectLists->SetAvailable(_s3Events);
    connect(_ui->eventSelectLists, &Awsmock::Components::SelectLists::SelectChangedSignal, this, [this](const QStringList &s3Events) {
        _selectedS3Event = s3Events;
    });

    // Events
    _ui->filterAddButton->setText(nullptr);
    _ui->filterAddButton->setIcon(IconUtils::GetIcon("add"));

    // Filter types
    const QStringList headers = {"Type", "Value"};
    _filterDataModel = new QStandardItemModel(this);
    _filterDataModel->setHorizontalHeaderLabels(headers);
    _filterDataModel->setColumnCount(static_cast<int>(headers.count()));
    _ui->filterTableView->setModel(_filterDataModel);
    _ui->filterTableView->setShowGrid(true);
    _ui->filterTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->filterTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->filterTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->filterTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents); // name
    _ui->filterTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch); //status

    const QStringList filterTypeNames = {"Prefix", "Suffix"};
    //_ui->filterTypeCombo->addItems(filterTypeNames);
}

S3QueueNotificationDialog::~S3QueueNotificationDialog() {
    delete _sqsService;
    delete _ui;
}

void S3QueueNotificationDialog::HandleAccept() {
    if (_selectedQueueArn.isEmpty()) {
        QMessageBox::warning(this, tr("S3 Queue Notification Dialog"), tr("Queue ARN cannot be empty!"));
        return;
    }
    if (_selectedS3Event.isEmpty()) {
        QMessageBox::warning(this, tr("S3 Queue Notification Dialog"), tr("S3 event cannot be empty!"));
        return;
    }
    accept();
}

void S3QueueNotificationDialog::HandleReject() {
    reject();
}
