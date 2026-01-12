//
// Created by vogje01 on 1/11/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_S3BucketLifecycleDialog.h" resolved

#include <modules/s3/S3BucketLifecycleDialog.h>
#include "ui_S3BucketLifecycleDialog.h"
#include "utils/IconUtils.h"

S3BucketLifecycleDialog::S3BucketLifecycleDialog(QWidget *parent) : BaseDialog(parent), _ui(new Ui::S3BucketLifecycleDialog) {
    Initialize();
}

S3BucketLifecycleDialog::S3BucketLifecycleDialog(const LifecycleRule &lifecycleRule, QWidget *parent) : BaseDialog(parent), _ui(new Ui::S3BucketLifecycleDialog), _lifecycleRule(lifecycleRule) {
    Initialize();
}

S3BucketLifecycleDialog::~S3BucketLifecycleDialog() {
    delete _ui;
}

void S3BucketLifecycleDialog::Initialize() {

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &S3BucketLifecycleDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &S3BucketLifecycleDialog::HandleReject);

    // Setup attributes
    _ui->idEdit->setText(_lifecycleRule.id);
    _ui->prefixEdit->setText(_lifecycleRule.prefix);

    _ui->statusCombo->addItems({"enabled", "disabled"});
    _ui->statusCombo->setCurrentIndex(_lifecycleRule.status.toInt());

    const QStringList headers = QStringList() = {tr("Date"), tr("Days"), tr("Storage Class")};

    // Table
    _transitionsDataModel = new QStandardItemModel(this);
    _transitionsDataModel->setHorizontalHeaderLabels(headers);
    _transitionsDataModel->setColumnCount(static_cast<int>(headers.count()));
    _ui->transitionsTable->setModel(_transitionsDataModel);

    _ui->transitionsTable->setShowGrid(true);
    _ui->transitionsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->transitionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->transitionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->transitionsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch); // name
    _ui->transitionsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents); //status

    // Enable sorting, default on and sorted ascending by name
    _ui->transitionsTable->setSortingEnabled(true);
    _ui->transitionsTable->sortByColumn(0, Qt::AscendingOrder);
    if (!_lifecycleRule.transitions.isEmpty()) {
        for (int row = 0; row < static_cast<int>(_lifecycleRule.transitions.count()); row++) {
            SetColumn(_transitionsDataModel, row, 0, _lifecycleRule.transitions[row].date);
            SetColumn(_transitionsDataModel, row, 1, _lifecycleRule.transitions[row].days);
            SetColumn(_transitionsDataModel, row, 2, StorageClassToString(_lifecycleRule.transitions[row].storageClass));
        }
    }

    // Add button
    _ui->transitionAddButton->setText(nullptr);
    _ui->transitionAddButton->setIcon(IconUtils::GetIcon("add"));

    _ui->tabWidget->removeTab(1);
}

void S3BucketLifecycleDialog::HandleAccept() {
    _lifecycleRule.id = _ui->idEdit->text();
    _lifecycleRule.prefix = _ui->prefixEdit->text();
    _lifecycleRule.status = _ui->statusCombo->currentText();
    accept();
}


void S3BucketLifecycleDialog::HandleReject() {
    reject();
}
