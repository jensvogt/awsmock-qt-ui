//
// Created by vogje01 on 11/7/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ApplicationEditDIalog.h" resolved

#include <QMenu>
#include <modules/application/ApplicationEditDialog.h>
#include "ui_ApplicationEditDialog.h"

ApplicationEditDialog::ApplicationEditDialog(const QString &name, QWidget *parent) : BaseDialog(parent), _ui(new Ui::ApplicationEditDialog) {

    _applicationService = new ApplicationService();

    _applicationService->GetApplication(name);
    connect(_applicationService, &ApplicationService::GetApplicationDetailsSignal, this, &ApplicationEditDialog::UpdateApplication);

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &ApplicationEditDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &ApplicationEditDialog::HandleReject);

    connect(_ui->enabledCheckBox, &QCheckBox::stateChanged, this, [this]() {
        if (_ui->enabledCheckBox->isChecked()) {
            _applicationService->EnableApplication(_ui->nameEdit->text());
        } else {
            _applicationService->DisableApplication(_ui->nameEdit->text());
        }
        _application.enabled = _ui->enabledCheckBox->isChecked();
    });

    // Setup environment tab
    SetupEnvironmentTab();
}

ApplicationEditDialog::~ApplicationEditDialog() {
    delete _ui;
}

void ApplicationEditDialog::UpdateApplication(const ApplicationGetResponse &applicationGetResponse) {

    _application = applicationGetResponse.application;

    _ui->nameEdit->setText(_application.name);
    _ui->runtimeEdit->setText(_application.runtime);
    _ui->runTypeEdit->setText(_application.runType);
    _ui->privatePortEdit->setText(QString::number(_application.privatePort));
    _ui->publicPortEdit->setText(QString::number(_application.publicPort));
    _ui->archiveEdit->setText(_application.archive);
    _ui->versionEdit->setText(_application.version);
    _ui->imageIdEdit->setText(_application.imageId);
    _ui->containerIdEdit->setText(_application.containerId);
    _ui->containerNameEdit->setText(_application.containerName);
    //_ui->statusEdit->setText(_application.status);
    _ui->enabledCheckBox->setChecked(_application.enabled);
    _ui->lastStartedEdit->setText(_application.lastStarted.toString("yyyy-MM-dd hh:mm:ss"));
    _ui->createdEdit->setText(_application.created.toString("yyyy-MM-dd hh:mm:ss"));
    _ui->modifiedEdit->setText(_application.modified.toString("yyyy-MM-dd hh:mm:ss"));

    connect(_ui->enabledCheckBox, &QCheckBox::stateChanged, this, [&]() { this->_changed = true; });

    // Update environment table
    int r = 0;
    _ui->envTable->setRowCount(0);
    _ui->envTable->setSortingEnabled(false);
    for (auto [key, value]: _application.environment.asKeyValueRange()) {
        _ui->envTable->insertRow(r);
        SetColumn(_ui->envTable, r, 0, key);
        SetColumn(_ui->envTable, r, 1, value);
        r++;
    }
    _ui->envTable->setRowCount(static_cast<int>(_application.environment.count()));
    _ui->envTable->setSortingEnabled(true);
    _ui->envTable->sortItems(_sortColumn, _sortOrder);
}

void ApplicationEditDialog::SetupEnvironmentTab() {

    // Add button
    _ui->envAddButton->setText("");
    _ui->envAddButton->setIcon(IconUtils::GetIcon("dark", "add"));

    // Refresh button
    _ui->envRefreshButton->setText("");
    _ui->envRefreshButton->setIcon(IconUtils::GetIcon("dark", "refresh"));

    // Table
    const QStringList headers = QStringList() = {tr("Key"), tr("Value")};

    _ui->envTable->setColumnCount(static_cast<int>(headers.count()));
    _ui->envTable->setShowGrid(true);
    _ui->envTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->envTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->envTable->setHorizontalHeaderLabels(headers);
    _ui->envTable->setSortingEnabled(true);
    _ui->envTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->envTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    _ui->envTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    // Add environment
    _ui->envTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_ui->envTable, &QTableWidget::customContextMenuRequested, this, &ApplicationEditDialog::ShowEnvironmentContextMenu);

    // Connect double-click
    connect(_ui->envTable, &QTableView::doubleClicked, this, [=](const QModelIndex &index) {
        // Get the position
        const int row = index.row();

        // Extract ARN and URL
        const QString key = _ui->envTable->item(row, 0)->text();
        const QString value = _ui->envTable->item(row, 1)->text();

        if (ApplicationEnvironmentEditDialog dialog(key, value, false, nullptr); dialog.exec() == Accepted) {
            SetColumn(_ui->envTable, row, 1, dialog.GetValue());
            _application.environment[key] = dialog.GetValue();
            _changed = true;
        }
    });

    // Connect add button
    connect(_ui->envAddButton, &QPushButton::clicked, this, [=]() {

        // Extract ARN and URL
        const QString key;
        const QString value;

        if (ApplicationEnvironmentEditDialog dialog(key, value, true, nullptr); dialog.exec() == Accepted) {
            const int newRowIndex = _ui->envTable->rowCount();
            _ui->envTable->insertRow(newRowIndex);
            SetColumn(_ui->envTable, newRowIndex, 0, dialog.GetKey());
            SetColumn(_ui->envTable, newRowIndex, 1, dialog.GetValue());
            _application.environment[dialog.GetKey()] = dialog.GetValue();
            _changed = true;
        }
    });
}

void ApplicationEditDialog::ShowEnvironmentContextMenu(const QPoint &pos) {

    // Cell index
    const QModelIndex index = _ui->envTable->indexAt(pos);
    if (!index.isValid()) return;

    const int row = index.row();

    QMenu menu;
    QAction *editAction = menu.addAction(IconUtils::GetIcon("dark", "edit"), "Edit Environment Variable");
    editAction->setToolTip("Edit the environment variable");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("dark", "delete"), "Delete Environment Variable");
    deleteAction->setToolTip("Delete the environment variable");

    const QString key = _ui->envTable->item(row, 0)->text();
    const QString value = _ui->envTable->item(row, 1)->text();
    if (const QAction *selectedAction = menu.exec(_ui->envTable->viewport()->mapToGlobal(pos)); selectedAction == editAction) {
        if (ApplicationEnvironmentEditDialog dialog(key, value, false); dialog.exec() == QDialog::Accepted) {
            SetColumn(_ui->envTable, row, 1, dialog.GetValue());
            _application.environment[key] = dialog.GetValue();
            _changed = true;
        }
    } else if (selectedAction == deleteAction) {
        _application.environment.remove(key);
        _ui->envTable->removeRow(row);
    }
}

void ApplicationEditDialog::HandleAccept() {
    if (this->_changed) {
        _applicationService->UpdateApplication(_application);
    }
    accept();
}


void ApplicationEditDialog::HandleReject() {
    accept();
}
