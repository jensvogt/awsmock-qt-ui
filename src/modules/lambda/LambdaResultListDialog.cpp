//
// Created by vogje01 on 11/28/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LambdaResultListDialog.h" resolved

#include <modules/lambda/LambdaResultListDialog.h>
#include "ui_LambdaResultListDialog.h"

LambdaResultListDialog::LambdaResultListDialog(const QString &lambdaArn, QWidget *parent) : BaseDialog(parent), _ui(new Ui::LambdaResultListDialog), _lambdaArn(lambdaArn) {

    // Lambda REST service
    _lambdaService = new LambdaService();

    // Setup UI components
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &LambdaResultListDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &LambdaResultListDialog::HandleReject);

    // Buttons
    _ui->refreshButton->setIcon(IconUtils::GetIcon("refresh"));
    _ui->deleteButton->setIcon(IconUtils::GetIcon("delete"));

    // Result table
    const QStringList headers = QStringList() = {tr("Name"), tr("ContainerId"), tr("Status"), tr("Timestamp"), tr("Duration")};

    _ui->lambdaResultTable->setColumnCount(static_cast<int>(headers.count()));
    _ui->lambdaResultTable->setShowGrid(true);
    _ui->lambdaResultTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->lambdaResultTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->lambdaResultTable->setHorizontalHeaderLabels(headers);
    _ui->lambdaResultTable->setSortingEnabled(true);
    _ui->lambdaResultTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->lambdaResultTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    _ui->lambdaResultTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    _ui->lambdaResultTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    _ui->lambdaResultTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    _ui->lambdaResultTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Interactive);
    _ui->lambdaResultTable->setColumnHidden(5, true);

    _lambdaService->ListLambdaLogs(_lambdaArn);
    connect(_lambdaService, &LambdaService::ListLambdaResultsSignal, this, &LambdaResultListDialog::UpdateResultTable);
}

LambdaResultListDialog::~LambdaResultListDialog() {
    delete _ui;
}

void LambdaResultListDialog::UpdateResultTable(const LambdaListResultsResponse &listResultsResponse) const {
    const int selectedRow = _ui->lambdaResultTable->selectionModel()->currentIndex().row();
    _ui->lambdaResultTable->setRowCount(0);
    _ui->lambdaResultTable->setSortingEnabled(false);
    for (auto r = 0; r < listResultsResponse.lambdaLogCounters.count(); r++) {
        _ui->lambdaResultTable->insertRow(r);
        SetColumn(_ui->lambdaResultTable, r, 0, listResultsResponse.lambdaLogCounters.at(r).lambdaName);
        SetColumn(_ui->lambdaResultTable, r, 1, listResultsResponse.lambdaLogCounters.at(r).containerId);
        SetColumn(_ui->lambdaResultTable, r, 2, listResultsResponse.lambdaLogCounters.at(r).lambdaStatus);
        SetColumn(_ui->lambdaResultTable, r, 3, listResultsResponse.lambdaLogCounters.at(r).timestamp);
        SetColumn(_ui->lambdaResultTable, r, 4, listResultsResponse.lambdaLogCounters.at(r).duration);
        SetColumn(_ui->lambdaResultTable, r, 5, listResultsResponse.lambdaLogCounters.at(r).lambdaArn);
    }
    _ui->lambdaResultTable->setRowCount(static_cast<int>(listResultsResponse.lambdaLogCounters.count()));
    _ui->lambdaResultTable->setSortingEnabled(true);
    //_ui->lambdaResultTable->sortItems(_sortColumn, _sortOrder);
    _ui->lambdaResultTable->selectRow(selectedRow);
}

void LambdaResultListDialog::HandleAccept() {
    accept();
}

void LambdaResultListDialog::HandleReject() {
    accept();
}
