//
// Created by vogje01 on 11/25/25.
//

#include <modules/lambda/LambdaDetailsDialog.h>
#include "ui_LambdaDetailsDialog.h"

LambdaDetailsDialog::LambdaDetailsDialog(QString lambdaArn, QWidget *parent) : BaseDialog(parent), _ui(new Ui::LambdaDetailsDialog), _lambdaArn(std::move(lambdaArn)) {

    // Initialize service
    _lambdaService = new LambdaService();

    connect(_lambdaService, &LambdaService::GetLambdaDetailsSignal, this, &LambdaDetailsDialog::UpdateLambda);
    connect(_lambdaService, &LambdaService::LoadLambdaEnvironment, this, &LambdaDetailsDialog::LoadContent);
    connect(_lambdaService, &LambdaService::ReloadLambdaDetails, this, &LambdaDetailsDialog::LoadContent);
    connect(_lambdaService, &LambdaService::ReloadLambdaInstances, this, &LambdaDetailsDialog::LoadContent);
    connect(_lambdaService, &LambdaService::ReloadLambdas, this, &LambdaDetailsDialog::LoadContent);

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &LambdaDetailsDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &LambdaDetailsDialog::HandleReject);

    // Connect refresh button
    _ui->refreshButton->setText(nullptr);
    _ui->refreshButton->setIcon(IconUtils::GetIcon("refresh"));
    connect(_ui->refreshButton, &QPushButton::clicked, this, [this]() {
        _lambdaService->GetLambda(_lambdaArn);
    });

    // Logs button
    _ui->logsButton->setText(nullptr);
    _ui->logsButton->setIcon(IconUtils::GetIcon("logs"));
    connect(_ui->logsButton, &QPushButton::clicked, [this]() {
        auto *dialog = new LambdaResultListDialog(_lambdaArn, this);
        dialog->setModal(false);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->show();
    });

    // Timer
    QStringList apis = {"GetLambda", "GetLambdaInstances", "GetLambdaEnvironment", "UpdateLambdaEnvironment", "AddLambdaEnvironment", "RemoveLambdaEnvironment", "ListLambdaEventSources"};
    connect(&EventBus::instance(), &EventBus::TimerSignal, this, [this, apis](const QString &timerName, const qint64 elapsed) {
        if (apis.contains(timerName)) {
            _ui->lastUpdateLabel->setText("Last update: " + QDateTime::currentDateTime().toString("hh:mm:ss") + " [" + QString::number(elapsed) + "ms]");
        }
    });

    // Enabled
    connect(_ui->enabledCheckBox, &QCheckBox::checkStateChanged, this, [this](int) {
        _lambdaGetResponse.enabled = _ui->enabledCheckBox->isChecked();
        _changed = true;
    });

    // Setup instances tab
    SetupInstancesTab();

    // Setup environment tab
    SetupEnvironmentTab();

    // Setup event sources tab
    SetupEventSourcesTab();

    // Set default tab
    _ui->tabWidget->setCurrentIndex(0);

    // Load content
    LambdaDetailsDialog::LoadContent();

    // Status
    _ui->lastUpdateLabel->setText("Initialized");
}

LambdaDetailsDialog::~LambdaDetailsDialog() {
    delete _ui;
}

void LambdaDetailsDialog::LoadContent() {
    _lambdaService->GetLambda(_lambdaArn);
    _ui->lastUpdateLabel->setText("Last update: " + DateTimeUtils::GetLogTimeFormat(QDateTime::currentDateTime()));
}

void LambdaDetailsDialog::UpdateLambda(const LambdaGetResponse &lambdaGetResponse) {
    _lambdaGetResponse = lambdaGetResponse;
    _ui->regionEdit->setText(lambdaGetResponse.region);
    _ui->nameEdit->setText(lambdaGetResponse.lambdaName);
    _ui->arnEdit->setText(lambdaGetResponse.lambdaArn);
    _ui->handlerEdit->setText(lambdaGetResponse.handler);
    _ui->roleEdit->setText(lambdaGetResponse.role);
    _ui->userEdit->setText(lambdaGetResponse.user);
    _ui->runtimeEdit->setText(lambdaGetResponse.runtime);
    _ui->versionEdit->setText(lambdaGetResponse.version);
    _ui->sizeEdit->setText(StringUtils::FormatSizeColumn(lambdaGetResponse.size, 0));
    _ui->concurrencyEdit->setText(QString::number(lambdaGetResponse.concurrency));
    _ui->instancesEdit->setText(QString::number(lambdaGetResponse.instances));
    _ui->invocationsEdit->setText(QString::number(lambdaGetResponse.invocations));
    _ui->avgExecutionEdit->setText(QString::number(lambdaGetResponse.avgDuration));
    _ui->zipFileEdit->setText(lambdaGetResponse.zipFile);
    _ui->statusEdit->setText(lambdaGetResponse.state);
    _ui->lifetimeEdit->setText(QString::number(lambdaGetResponse.lifetime));
    _ui->enabledCheckBox->setCheckState(lambdaGetResponse.enabled ? Qt::CheckState::Checked : Qt::Unchecked);
}

void LambdaDetailsDialog::ReloadLambdaInstances() {
    // Send request
    _lambdaService->GetLambdaInstances(_lambdaArn);
    connect(_lambdaService, &LambdaService::ListLambdaInstancesSignal, this, &LambdaDetailsDialog::UpdateLambdaInstances);
}

void LambdaDetailsDialog::SetupInstancesTab() {

    // Table
    const QStringList headers = QStringList() = {tr("Instance ID"), tr("Runtime"), tr("Runtime Version"), tr("Container ID"), tr("Host"), tr("Private Port"), tr("Public Port"), tr("Status"), tr("Last Invocation")};
    _ui->instanceTable->setColumnCount(static_cast<int>(headers.count()));
    _ui->instanceTable->setShowGrid(true);
    _ui->instanceTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->instanceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->instanceTable->setHorizontalHeaderLabels(headers);
    _ui->instanceTable->setSortingEnabled(true);
    _ui->instanceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->instanceTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    _ui->instanceTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    _ui->instanceTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    _ui->instanceTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    _ui->instanceTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    _ui->instanceTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    _ui->instanceTable->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    _ui->instanceTable->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
    _ui->instanceTable->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents);

    // Start instance button
    _ui->startInstanceButton->setText(nullptr);
    _ui->startInstanceButton->setIcon(IconUtils::GetIcon("add"));
    connect(_ui->startInstanceButton, &QPushButton::clicked, this, &LambdaDetailsDialog::StartInstance);

    // Instance refresh button
    _ui->instanceRefreshButton->setText(nullptr);
    _ui->instanceRefreshButton->setIcon(IconUtils::GetIcon("refresh"));
    connect(_ui->instanceRefreshButton, &QPushButton::clicked, this, &LambdaDetailsDialog::ReloadLambdaInstances);

    // Add tag context menu
    _ui->instanceTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_ui->instanceTable, &QTableWidget::customContextMenuRequested, this, &LambdaDetailsDialog::ShowInstanceContextMenu);

    // Delete key
    auto *instanceDeleteAction = new QAction(_ui->instanceTable);
    instanceDeleteAction->setShortcut(QKeySequence(Qt::Key_Delete));
    instanceDeleteAction->setShortcutContext(Qt::WidgetShortcut);
    _ui->instanceTable->addAction(instanceDeleteAction);
    connect(instanceDeleteAction, &QAction::triggered, this, &LambdaDetailsDialog::KillSelectedInstance);

    // Connect double-click
    connect(_ui->instanceTable, &QTableWidget::doubleClicked, this, [this](const QModelIndex &index) {
        // Get the position
        const int row = index.row();

        const QString instanceId = _ui->instanceTable->item(row, 0)->text();

        // Open details dialog
        if (LambdaInstanceDialog dialog(_lambdaArn, instanceId); dialog.exec() == Accepted) {
            //_changed = true;
        }
    });

    // Send request
    ReloadLambdaInstances();
}

void LambdaDetailsDialog::UpdateLambdaInstances(const LambdaListInstancesResponse &listInstancesResponse) const {
    const int selectedRow = _ui->instanceTable->selectionModel()->currentIndex().row();
    _ui->instanceTable->setRowCount(0);
    _ui->instanceTable->setSortingEnabled(false);
    for (auto r = 0, c = 0; r < listInstancesResponse.lambdaInstanceCounters.count(); r++, c = 0) {
        _ui->instanceTable->insertRow(r);
        SetColumn(_ui->instanceTable, r, c++, listInstancesResponse.lambdaInstanceCounters.at(r).instanceId);
        SetColumn(_ui->instanceTable, r, c++, _lambdaGetResponse.runtime);
        SetColumn(_ui->instanceTable, r, c++, listInstancesResponse.lambdaInstanceCounters.at(r).runtimeVersion);
        SetColumn(_ui->instanceTable, r, c++, listInstancesResponse.lambdaInstanceCounters.at(r).containerId.mid(0, 12));
        SetColumn(_ui->instanceTable, r, c++, listInstancesResponse.lambdaInstanceCounters.at(r).hostname);
        SetColumn(_ui->instanceTable, r, c++, listInstancesResponse.lambdaInstanceCounters.at(r).privatePort);
        SetColumn(_ui->instanceTable, r, c++, listInstancesResponse.lambdaInstanceCounters.at(r).publicPort);
        SetColumn(_ui->instanceTable, r, c++, listInstancesResponse.lambdaInstanceCounters.at(r).status);
        if (listInstancesResponse.lambdaInstanceCounters.at(r).lastInvocation.has_value()) {
            SetColumn(_ui->instanceTable, r, c++, listInstancesResponse.lambdaInstanceCounters.at(r).lastInvocation.value());
        }
    }
    _ui->instanceTable->setRowCount(static_cast<int>(listInstancesResponse.lambdaInstanceCounters.count()));
    _ui->instanceTable->setSortingEnabled(true);
    _ui->instanceTable->sortItems(_instanceSortColumn, _instanceSortOrder);
    _ui->instanceTable->selectRow(selectedRow);
}

void LambdaDetailsDialog::SetupEnvironmentTab() const {
    // Add button
    _ui->environmentAddButton->setText(nullptr);
    _ui->environmentAddButton->setIcon(IconUtils::GetIcon("add"));
    connect(_ui->environmentAddButton, &QPushButton::clicked, [this]() {
        if (LambdaEnvironmentDetailDialog dialog{}; dialog.exec() == Accepted) {
            const int newRowIndex = _ui->environmentTable->rowCount();
            _ui->environmentTable->insertRow(newRowIndex);
            SetColumn(_ui->environmentTable, newRowIndex, 0, dialog.GetKey());
            SetColumn(_ui->environmentTable, newRowIndex, 1, dialog.GetValue());
            _lambdaService->AddLambdaEnvironment(_lambdaArn, dialog.GetKey(), dialog.GetValue());
        }
    });

    // Refresh button
    _ui->environmentRefreshButton->setText(nullptr);
    _ui->environmentRefreshButton->setIcon(IconUtils::GetIcon("refresh"));
    connect(_ui->environmentRefreshButton, &QPushButton::clicked, [this]() {
        _lambdaService->GetLambdaEnvironment(_lambdaArn);
    });

    // Add context menu
    _ui->environmentTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_ui->environmentTable, &QTableWidget::customContextMenuRequested, this, &LambdaDetailsDialog::ShowEnvironmentContextMenu);

    // Delete key
    auto *environmentDeleteAction = new QAction(_ui->environmentTable);
    environmentDeleteAction->setShortcut(QKeySequence(Qt::Key_Delete));
    environmentDeleteAction->setShortcutContext(Qt::WidgetShortcut);
    _ui->environmentTable->addAction(environmentDeleteAction);
    connect(environmentDeleteAction, &QAction::triggered, this, &LambdaDetailsDialog::DeleteSelectedEnvironment);

    // Send request
    _lambdaService->GetLambdaEnvironment(_lambdaArn);
    connect(_lambdaService, &LambdaService::ListLambdaEnvironmentSignal, this, &LambdaDetailsDialog::UpdateLambdaEnvironment);

    const QStringList headers = QStringList() = {tr("Key"), tr("Value")};
    _ui->environmentTable->setColumnCount(static_cast<int>(headers.count()));
    _ui->environmentTable->setShowGrid(true);
    _ui->environmentTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->environmentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->environmentTable->setHorizontalHeaderLabels(headers);
    _ui->environmentTable->setSortingEnabled(true);
    _ui->environmentTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->environmentTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    _ui->environmentTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    // Connect double-click
    connect(_ui->environmentTable, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {
        // Get the position
        const int row = index.row();

        // Extract ARN and URL
        const QString key = _ui->environmentTable->item(row, 0)->text();
        const QString value = _ui->environmentTable->item(row, 1)->text();

        if (LambdaEnvironmentDetailDialog dialog(key, value); dialog.exec() == Accepted) {
            SetColumn(_ui->environmentTable, row, 1, dialog.GetValue());
            _lambdaService->UpdateLambdaEnvironment(_lambdaArn, dialog.GetKey(), dialog.GetValue());
        }
    });
}

void LambdaDetailsDialog::UpdateLambdaEnvironment(const LambdaListEnvironmentResponse &listEnvironmentResponse) const {
    const int selectedRow = _ui->environmentTable->selectionModel()->currentIndex().row();
    _ui->environmentTable->setRowCount(0);
    _ui->environmentTable->setSortingEnabled(false);
    int r = 0, c = 0;
    for (const auto &key: listEnvironmentResponse.environmentCounters.keys()) {
        _ui->environmentTable->insertRow(r);
        SetColumn(_ui->environmentTable, r, c++, key);
        SetColumn(_ui->environmentTable, r, c, listEnvironmentResponse.environmentCounters[key]);
        r++;
        c = 0;
    }
    _ui->environmentTable->setRowCount(r);
    _ui->environmentTable->setSortingEnabled(true);
    _ui->instanceTable->sortItems(_environmentSortColumn, _environmentSortOrder);
    _ui->environmentTable->selectRow(selectedRow);
}

void LambdaDetailsDialog::ShowEnvironmentContextMenu(const QPoint &pos) const {
    // Cell index
    const QModelIndex index = _ui->environmentTable->indexAt(pos);
    if (!index.isValid()) return;

    const int row = index.row();

    QMenu menu;
    QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit Environment Variable");
    editAction->setToolTip("Edit the environment variable");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Environment Variable");
    deleteAction->setToolTip("Delete the environment variable");

    const QString key = _ui->environmentTable->item(row, 0)->text();
    const QString value = _ui->environmentTable->item(row, 1)->text();
    if (const QAction *selectedAction = menu.exec(_ui->environmentTable->viewport()->mapToGlobal(pos));
        selectedAction == editAction) {
        if (LambdaEnvironmentDetailDialog dialog(key, value); dialog.exec() == Accepted) {
            SetColumn(_ui->environmentTable, row, 1, dialog.GetValue());
            SetColumn(_ui->environmentTable, row, 1, dialog.GetValue());
            _lambdaService->UpdateLambdaEnvironment(_lambdaArn, dialog.GetKey(), dialog.GetValue());
        }
    } else if (selectedAction == deleteAction) {
        DeleteSelectedEnvironment();
    }
}

void LambdaDetailsDialog::DeleteSelectedEnvironment() const {
    const int row = _ui->environmentTable->currentRow();
    if (row < 0) return;

    const QTableWidgetItem *keyItem = _ui->environmentTable->item(row, 0);
    if (!keyItem) return;

    _lambdaService->RemoveLambdaEnvironment(_lambdaArn, keyItem->text());
    _ui->environmentTable->removeRow(row);
}

void LambdaDetailsDialog::SetupEventSourcesTab() const {

    // Table
    const QStringList headers = QStringList() = {tr("Type"), tr("Event Source ARN"), tr("Batch Size"), tr("Max Batching Window (s)"), tr("UUID")};
    _ui->eventSourceTable->setColumnCount(static_cast<int>(headers.count()));
    _ui->eventSourceTable->setShowGrid(true);
    _ui->eventSourceTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->eventSourceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->eventSourceTable->setHorizontalHeaderLabels(headers);
    _ui->eventSourceTable->setSortingEnabled(true);
    _ui->eventSourceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->eventSourceTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    _ui->eventSourceTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    _ui->eventSourceTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    _ui->eventSourceTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    _ui->eventSourceTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);

    // Add button
    _ui->eventSourceAddButton->setText(nullptr);
    _ui->eventSourceAddButton->setIcon(IconUtils::GetIcon("add"));
    connect(_ui->eventSourceAddButton, &QPushButton::clicked, [this]() {
        if (LambdaEventSourceDetailDialog dialog("", "", 10, 5, ""); dialog.exec() == Accepted) {
            const int newRowIndex = _ui->eventSourceTable->rowCount();
            _ui->eventSourceTable->insertRow(newRowIndex);
            SetColumn(_ui->eventSourceTable, newRowIndex, 0, dialog.GetType());
            SetColumn(_ui->eventSourceTable, newRowIndex, 1, dialog.GetEventSourceArn());
            SetColumn(_ui->eventSourceTable, newRowIndex, 2, dialog.GetBatchSize());
            SetColumn(_ui->eventSourceTable, newRowIndex, 3, dialog.GetMaximumBatchingWindowInSeconds());
            SetColumn(_ui->eventSourceTable, newRowIndex, 4, QString());
            _lambdaService->AddEventSource(dialog.GetType(), _lambdaArn, dialog.GetEventSourceArn(), true, dialog.GetBatchSize(), dialog.GetMaximumBatchingWindowInSeconds());
        }
    });

    // Refresh button
    _ui->eventSourceRefreshButton->setText(nullptr);
    _ui->eventSourceRefreshButton->setIcon(IconUtils::GetIcon("refresh"));
    connect(_ui->eventSourceRefreshButton, &QPushButton::clicked, [this]() {
        _lambdaService->ListLambdaEventSources(_lambdaArn);
    });

    // Add context menu
    _ui->eventSourceTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_ui->eventSourceTable, &QTableWidget::customContextMenuRequested, this, &LambdaDetailsDialog::ShowEventSourceContextMenu);

    // Delete key
    auto *eventSourceDeleteAction = new QAction(_ui->eventSourceTable);
    eventSourceDeleteAction->setShortcut(QKeySequence(Qt::Key_Delete));
    eventSourceDeleteAction->setShortcutContext(Qt::WidgetShortcut);
    _ui->eventSourceTable->addAction(eventSourceDeleteAction);
    connect(eventSourceDeleteAction, &QAction::triggered, this, &LambdaDetailsDialog::DeleteSelectedEventSource);

    // Send request
    _lambdaService->ListLambdaEventSources(_lambdaArn);
    connect(_lambdaService, &LambdaService::ListLambdaEventSourcesSignal, this, &LambdaDetailsDialog::UpdateLambdaEventSources);
}

void LambdaDetailsDialog::UpdateLambdaEventSources(const LambdaListEventSourcesResponse &listEventSourcesResponse) const {
    const int selectedRow = _ui->eventSourceTable->selectionModel()->currentIndex().row();
    _ui->eventSourceTable->setRowCount(0);
    _ui->eventSourceTable->setSortingEnabled(false);
    for (auto r = 0, c = 0; r < listEventSourcesResponse.eventSourceCounters.count(); r++, c = 0) {
        _ui->eventSourceTable->insertRow(r);
        SetColumn(_ui->eventSourceTable, r, c++, listEventSourcesResponse.eventSourceCounters.at(r).type);
        SetColumn(_ui->eventSourceTable, r, c++, listEventSourcesResponse.eventSourceCounters.at(r).eventSourceArn);
        SetColumn(_ui->eventSourceTable, r, c++, listEventSourcesResponse.eventSourceCounters.at(r).batchSize);
        SetColumn(_ui->eventSourceTable, r, c++, listEventSourcesResponse.eventSourceCounters.at(r).maximumBatchingWindowInSeconds);
        SetColumn(_ui->eventSourceTable, r, c++, listEventSourcesResponse.eventSourceCounters.at(r).uuid);
    }
    _ui->eventSourceTable->setRowCount(static_cast<int>(listEventSourcesResponse.eventSourceCounters.count()));
    _ui->eventSourceTable->setSortingEnabled(true);
    _ui->eventSourceTable->selectRow(selectedRow);
}

void LambdaDetailsDialog::ShowEventSourceContextMenu(const QPoint &pos) const {
    // Cell index
    const QModelIndex index = _ui->eventSourceTable->indexAt(pos);
    if (!index.isValid()) return;

    const int row = index.row();

    QMenu menu;
    QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit Event Source");
    editAction->setToolTip("Edit the lambda event source mapping");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Event Source");
    deleteAction->setToolTip("Delete the lambda event source mapping");

    const QTableWidgetItem *typeItem = _ui->eventSourceTable->item(row, 0);
    const QTableWidgetItem *arnItem = _ui->eventSourceTable->item(row, 1);
    const QTableWidgetItem *batchSizeItem = _ui->eventSourceTable->item(row, 2);
    const QTableWidgetItem *maxBatchingWindowItem = _ui->eventSourceTable->item(row, 3);
    const QTableWidgetItem *uuidItem = _ui->eventSourceTable->item(row, 4);
    if (!typeItem || !arnItem || !batchSizeItem || !maxBatchingWindowItem || !uuidItem) return;

    const QString type = typeItem->text();
    const QString eventSourceArn = arnItem->text();
    const long batchSize = batchSizeItem->text().toLong();
    const long maxBatchingWindow = maxBatchingWindowItem->text().toLong();
    const QString uuid = uuidItem->text();

    if (const QAction *selectedAction = menu.exec(_ui->eventSourceTable->viewport()->mapToGlobal(pos));
        selectedAction == editAction) {
        if (LambdaEventSourceDetailDialog dialog(type, eventSourceArn, batchSize, maxBatchingWindow, uuid); dialog.exec() == Accepted) {
            SetColumn(_ui->eventSourceTable, row, 0, dialog.GetType());
            SetColumn(_ui->eventSourceTable, row, 1, dialog.GetEventSourceArn());
            SetColumn(_ui->eventSourceTable, row, 2, dialog.GetBatchSize());
            SetColumn(_ui->eventSourceTable, row, 3, dialog.GetMaximumBatchingWindowInSeconds());
            _lambdaService->AddEventSource(dialog.GetType(), _lambdaArn, dialog.GetEventSourceArn(), true, dialog.GetBatchSize(), dialog.GetMaximumBatchingWindowInSeconds(), dialog.GetUuid());
        }
    } else if (selectedAction == deleteAction) {
        DeleteSelectedEventSource();
    }
}

void LambdaDetailsDialog::DeleteSelectedEventSource() const {
    const int row = _ui->eventSourceTable->currentRow();
    if (row < 0) return;

    const QTableWidgetItem *arnItem = _ui->eventSourceTable->item(row, 1);
    if (!arnItem) return;

    _lambdaService->RemoveEventSource(_lambdaArn, arnItem->text());
    _ui->eventSourceTable->removeRow(row);
}

void LambdaDetailsDialog::ShowInstanceContextMenu(const QPoint &pos) const {

    int row = 0;
    QMenu menu;
    QAction *startAction = nullptr, *killAction = nullptr, *editAction = nullptr, *stopAction = nullptr;

    // Cell index
    const QModelIndex index = _ui->instanceTable->indexAt(pos);

    // Edit action
    editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit the lambda instance");
    editAction->setToolTip("Start a new Lambda Instance");

    if (!index.isValid()) {

        startAction = menu.addAction(IconUtils::GetIcon("start"), "Start a new Lambda Instance");
        startAction->setToolTip("Start a new Lambda Instance");

    } else {

        stopAction = menu.addAction(IconUtils::GetIcon("stop"), "Stop the lambda instance");
        stopAction->setToolTip("Stop lambda instance");

        killAction = menu.addAction(IconUtils::GetIcon("delete"), "Kill Lambda Instance");
        killAction->setToolTip("Kill lambda instance");
        row = index.row();
    }

    if (const QAction *selectedAction = menu.exec(_ui->instanceTable->viewport()->mapToGlobal(pos)); selectedAction == killAction) {
        KillSelectedInstance();
    } else if (selectedAction == startAction) {
        _lambdaService->StartInstance(_lambdaArn);
        logInfo << "Lambda instance started, lambdaArn: " << _lambdaArn;
        new Awsmock::Components::ToastOverlay("Lambda instance started.\nLambdaArn: " + _lambdaArn);
    } else if (selectedAction == stopAction) {
        const QString instanceId = _ui->instanceTable->item(row, 0)->text();
        _lambdaService->StopInstance(_lambdaArn, instanceId);
        logInfo << "Lambda instance stopped, lambdaArn: " << _lambdaArn << ", instanceId: " << instanceId;
        new Awsmock::Components::ToastOverlay("Lambda instance stopped.\nLambdaArn: " + _lambdaArn + ", instanceId: " + instanceId);
    } else if (selectedAction == editAction) {
        const QString instanceId = _ui->instanceTable->item(row, 0)->text();
        if (LambdaInstanceDialog dialog(_lambdaArn, instanceId); dialog.exec() == Accepted) {
            logInfo << "Lambda instance dialog started, lambdaArn: " << _lambdaArn;
        }
    }
}

void LambdaDetailsDialog::KillSelectedInstance() const {
    const int row = _ui->instanceTable->currentRow();
    if (row < 0) return;

    const QTableWidgetItem *idItem = _ui->instanceTable->item(row, 0);
    if (!idItem) return;

    const QString instanceId = idItem->text();
    _lambdaService->StopInstance(_lambdaArn, instanceId);
    logInfo << "Lambda instance stopped, instanceId: " << instanceId;
    new Awsmock::Components::ToastOverlay("Lambda instance killed.\nInstanceId: " + instanceId);
}

void LambdaDetailsDialog::StartInstance() const {
    _lambdaService->StartInstance(_lambdaArn);
}

void LambdaDetailsDialog::HandleAccept() {
    if (_changed) {
        _lambdaService->UpdateLambda(_lambdaArn, _lambdaGetResponse.enabled);
    }
    accept();
}

void LambdaDetailsDialog::HandleReject() {
    reject();
}
