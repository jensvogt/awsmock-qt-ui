//
// Created by vogje01 on 11/25/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LambdaDetailsDialog.h" resolved

#include <modules/lambda/LambdaDetailsDialog.h>
#include "ui_LambdaDetailsDialog.h"

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
    const QStringList headers = QStringList() << tr("Instance ID")
                                << tr("Container ID")
                                << tr("Host")
                                << tr("Port")
                                << tr("Status")
                                << tr("Last Invocation");

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
    //_ui->instanceTable->sortItems(_sortColumn, _sortOrder);
    _ui->instanceTable->selectRow(selectedRow);
}

void LambdaDetailsDialog::HandleAccept() {
    accept();
}

void LambdaDetailsDialog::HandleReject() {
    accept();
}
