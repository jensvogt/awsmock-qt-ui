//
// Created by vogje01 on 11/25/25.
//

#include <modules/lambda/LambdaDetailsDialog.h>
#include "ui_LambdaDetailsDialog.h"
#include "modules/lambda/LambdaEnvironmentDetailDialog.h"

LambdaDetailsDialog::LambdaDetailsDialog(const QString &lambdaArn, QWidget *parent) : BaseDialog(parent), _ui(new Ui::LambdaDetailsDialog), _lambdaArn(lambdaArn) {
    _lambdaService = new LambdaService();

    _lambdaService->GetLambda(lambdaArn);
    connect(_lambdaService, &LambdaService::GetLambdaDetailsSignal, this, &LambdaDetailsDialog::UpdateLambda);

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &LambdaDetailsDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &LambdaDetailsDialog::HandleReject);

    // Connect refresh button
    _ui->refreshButton->setText(nullptr);
    _ui->refreshButton->setIcon(IconUtils::GetIcon("refresh"));
    connect(_ui->refreshButton, &QPushButton::clicked, this, [this]() {
        _lambdaService->GetLambda(_lambdaArn);
    });

    // Setup instances tab
    SetupInstancesTab();

    // Setup environment tab
    SetupEnvironmentTab();

    // Set default tab
    _ui->tabWidget->setCurrentIndex(0);
}

LambdaDetailsDialog::~LambdaDetailsDialog() {
    delete _ui;
}

void LambdaDetailsDialog::UpdateLambda(const LambdaGetResponse &lambdaGetResponse) const {
    _ui->regionEdit->setText(lambdaGetResponse.region);
    _ui->nameEdit->setText(lambdaGetResponse.lambdaName);
    _ui->arnEdit->setText(lambdaGetResponse.lambdaArn);
    _ui->handlerEdit->setText(lambdaGetResponse.handler);
    _ui->roleEdit->setText(lambdaGetResponse.role);
    _ui->userEdit->setText(lambdaGetResponse.user);
    _ui->runtimeEdit->setText(lambdaGetResponse.runtime);
    _ui->versionEdit->setText(lambdaGetResponse.version);
    _ui->sizeEdit->setText(QString::number(lambdaGetResponse.size));
    _ui->concurrencyEdit->setText(QString::number(lambdaGetResponse.concurrency));
    _ui->instancesEdit->setText(QString::number(lambdaGetResponse.instances));
    _ui->invocationsEdit->setText(QString::number(lambdaGetResponse.invocations));
    _ui->avgExecutionEdit->setText(QString::number(lambdaGetResponse.averageRuntime));
    _ui->zipFileEdit->setText(lambdaGetResponse.zipFile);
    _ui->statusEdit->setText(lambdaGetResponse.state);
}

void LambdaDetailsDialog::SetupInstancesTab() const {
    // Send request
    _lambdaService->GetLambdaInstances(_lambdaArn);
    connect(_lambdaService, &LambdaService::ListLambdaInstancesSignal, this, &LambdaDetailsDialog::UpdateLambdaInstances);

    // Table
    const QStringList headers = QStringList() = {tr("Instance ID"), tr("Container ID"), tr("Host"), tr("Port"), tr("Status"), tr("Last Invocation")};
    _ui->instanceTable->setColumnCount(static_cast<int>(headers.count()));
    _ui->instanceTable->setShowGrid(true);
    _ui->instanceTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->instanceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->instanceTable->setHorizontalHeaderLabels(headers);
    _ui->instanceTable->setSortingEnabled(true);
    _ui->instanceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->instanceTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    _ui->instanceTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    _ui->instanceTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    _ui->instanceTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Interactive);
    _ui->instanceTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    _ui->instanceTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);

    // Instance refresh button
    _ui->instanceRefreshButton->setText(nullptr);
    _ui->instanceRefreshButton->setIcon(IconUtils::GetIcon("refresh"));
}

void LambdaDetailsDialog::UpdateLambdaInstances(const LambdaListInstancesResponse &listInstancesResponse) const {
    const int selectedRow = _ui->instanceTable->selectionModel()->currentIndex().row();
    _ui->instanceTable->setRowCount(0);
    _ui->instanceTable->setSortingEnabled(false);
    for (auto r = 0, c = 0; r < listInstancesResponse.lambdaInstanceCounters.count(); r++, c = 0) {
        _ui->instanceTable->insertRow(r);
        SetColumn(_ui->instanceTable, r, c++, listInstancesResponse.lambdaInstanceCounters.at(r).instanceId);
        SetColumn(_ui->instanceTable, r, c++, listInstancesResponse.lambdaInstanceCounters.at(r).containerId);
        SetColumn(_ui->instanceTable, r, c++, listInstancesResponse.lambdaInstanceCounters.at(r).hostname);
        SetColumn(_ui->instanceTable, r, c++, listInstancesResponse.lambdaInstanceCounters.at(r).port);
        SetColumn(_ui->instanceTable, r, c++, listInstancesResponse.lambdaInstanceCounters.at(r).status);
        SetColumn(_ui->instanceTable, r, c++, listInstancesResponse.lambdaInstanceCounters.at(r).lastInvocation);
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
        if (LambdaEnvironmentDetailDialog dialog(nullptr, nullptr); dialog.exec() == Accepted) {
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
}


void LambdaDetailsDialog::UpdateLambdaEnvironment(const LambdaListEnvironmentResponse &listInstancesResponse) const {

    const int selectedRow = _ui->environmentTable->selectionModel()->currentIndex().row();
    _ui->environmentTable->setRowCount(0);
    _ui->environmentTable->setSortingEnabled(false);
    int r = 0, c = 0;
    for (const auto &key: listInstancesResponse.environmentCounters.keys()) {
        _ui->environmentTable->insertRow(r);
        SetColumn(_ui->environmentTable, r, c++, key);
        SetColumn(_ui->environmentTable, r, c, listInstancesResponse.environmentCounters[key]);
        r++;
        c = 0;
    }
    _ui->environmentTable->setRowCount(r);
    _ui->environmentTable->setSortingEnabled(true);
    _ui->instanceTable->sortItems(_environmentSortColumn, _environmentSortOrder);
    _ui->environmentTable->selectRow(selectedRow);
}

void LambdaDetailsDialog::HandleAccept() {
    accept();
}

void LambdaDetailsDialog::HandleReject() {
    accept();
}
