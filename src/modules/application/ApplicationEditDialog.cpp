//
// Created by vogje01 on 11/7/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ApplicationEditDIalog.h" resolved

#include <modules/application/ApplicationEditDialog.h>
#include "ui_ApplicationEditDialog.h"

ApplicationEditDialog::ApplicationEditDialog(const QString &name, QWidget *parent) : BaseDialog(parent), _ui(new Ui::ApplicationEditDialog) {

    _applicationService = new ApplicationService();

    _applicationService->GetApplication(name);
    connect(_applicationService, &ApplicationService::GetApplicationDetailsSignal, this, &ApplicationEditDialog::UpdateApplication);

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &ApplicationEditDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &ApplicationEditDialog::HandleReject);

    // Connect start button
    _ui->startButton->setText(nullptr);
    _ui->startButton->setIcon(IconUtils::GetIcon("dark", "start"));
    _ui->startButton->setEnabled(false);
    connect(_ui->startButton, &QPushButton::clicked, this, [this,name]() {
        _applicationService->StartApplication(name);
        _applicationService->GetApplication(name);
    });

    // Connect stop button
    _ui->stopButton->setText(nullptr);
    _ui->stopButton->setIcon(IconUtils::GetIcon("dark", "stop"));
    _ui->stopButton->setEnabled(false);
    connect(_ui->stopButton, &QPushButton::clicked, this, [this,name]() {
        _applicationService->StopApplication(name);
        _applicationService->GetApplication(name);
    });

    // Connect refresh button
    _ui->refreshButton->setText(nullptr);
    _ui->refreshButton->setIcon(IconUtils::GetIcon("dark", "refresh"));
    connect(_ui->refreshButton, &QPushButton::clicked, this, [this,name]() {
        _applicationService->GetApplication(name);
    });

    // Connect enable check box
    connect(_ui->enabledCheckBox, &QCheckBox::stateChanged, this, [this]() {
        _application.enabled = _ui->enabledCheckBox->isChecked();
        _changed = true;
        if (_application.enabled) {
            _applicationService->EnableApplication(_application.name);
        } else {
            _applicationService->DisableApplication(_application.name);
        }
    });

    // Connect description test area
    connect(_ui->descriptionEdit, &QTextEdit::textChanged, this, [this]() {
        _application.description = _ui->descriptionEdit->toPlainText();
        _changed = true;
    });

    // Setup environment tab
    SetupEnvironmentTab();

    // Setup tags tab
    SetupTagsTab();

    // Setup dependency tab
    SetupDependenciesTab();

    _ui->tabWidget->setCurrentIndex(0);
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
    _ui->statusEdit->setText(_application.status);
    _ui->enabledCheckBox->setChecked(_application.enabled);
    _ui->descriptionEdit->setText(_application.description);
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
    _ui->envTable->sortItems(_sortColumnEnv, _sortOrderEnv);

    // Update tag table
    r = 0;
    _ui->tagTable->setRowCount(0);
    _ui->tagTable->setSortingEnabled(false);
    for (auto [key, value]: _application.tags.asKeyValueRange()) {
        _ui->tagTable->insertRow(r);
        SetColumn(_ui->tagTable, r, 0, key);
        SetColumn(_ui->tagTable, r, 1, value);
        r++;
    }
    _ui->tagTable->setRowCount(static_cast<int>(_application.tags.count()));
    _ui->tagTable->setSortingEnabled(true);
    _ui->tagTable->sortItems(_sortColumnTag, _sortOrderTag);

    // Update dependencies tab
    r = 0;
    _ui->dependencyList->clear();
    for (const auto &name: _application.dependencies) {
        _ui->dependencyList->insertItem(r, name);
        r++;
    }

    // Set start button
    _ui->startButton->setDisabled(_application.status == "RUNNING");
    _ui->stopButton->setDisabled(_application.status != "RUNNING");
}

void ApplicationEditDialog::SetupEnvironmentTab() {

    // Add button
    _ui->envAddButton->setText("");
    _ui->envAddButton->setIcon(IconUtils::GetIcon("dark", "add"));

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

void ApplicationEditDialog::SetupTagsTab() {

    // Add button
    _ui->tagAddButton->setText("");
    _ui->tagAddButton->setIcon(IconUtils::GetIcon("dark", "add"));

    // Table
    const QStringList headers = QStringList() = {tr("Key"), tr("Value")};

    _ui->tagTable->setColumnCount(static_cast<int>(headers.count()));
    _ui->tagTable->setShowGrid(true);
    _ui->tagTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->tagTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->tagTable->setHorizontalHeaderLabels(headers);
    _ui->tagTable->setSortingEnabled(true);
    _ui->tagTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->tagTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    _ui->tagTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    // Add tag context menu
    _ui->tagTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_ui->tagTable, &QTableWidget::customContextMenuRequested, this, &ApplicationEditDialog::ShowTagsContextMenu);

    // Connect double-click
    connect(_ui->tagTable, &QTableView::doubleClicked, this, [=](const QModelIndex &index) {

        // Get the position
        const int row = index.row();

        // Extract ARN and URL
        const QString key = _ui->tagTable->item(row, 0)->text();
        const QString value = _ui->tagTable->item(row, 1)->text();

        if (ApplicationTagDialog dialog(key, value, false, nullptr); dialog.exec() == Accepted) {
            SetColumn(_ui->tagTable, row, 1, dialog.GetValue());
            _application.tags[key] = dialog.GetValue();
            _changed = true;
        }
    });

    // Connect add button
    connect(_ui->tagAddButton, &QPushButton::clicked, this, [=]() {

        // Extract ARN and URL
        const QString key;
        const QString value;

        if (ApplicationTagDialog dialog(key, value, true, nullptr); dialog.exec() == Accepted) {
            const int newRowIndex = _ui->tagTable->rowCount();
            _ui->tagTable->insertRow(newRowIndex);
            SetColumn(_ui->tagTable, newRowIndex, 0, dialog.GetKey());
            SetColumn(_ui->tagTable, newRowIndex, 1, dialog.GetValue());
            _application.tags[dialog.GetKey()] = dialog.GetValue();
            _changed = true;
        }
    });
}

void ApplicationEditDialog::SetupDependenciesTab() {

    // Add button
    _ui->dependencyAddButton->setText("");
    _ui->dependencyAddButton->setIcon(IconUtils::GetIcon("dark", "add"));

    QStringList dependenciesList;
    for (const auto &dependency: _application.dependencies) {
        dependenciesList.append(dependency);
    }
    _ui->dependencyList->addItems(dependenciesList);

    // Add dependency context menu
    _ui->dependencyList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_ui->dependencyList, &QListWidget::customContextMenuRequested, this, &ApplicationEditDialog::ShowDependenciesContextMenu);

    // Connect double-click
    connect(_ui->dependencyList, &QListWidget::doubleClicked, this, [=](const QModelIndex &index) {

        // Extract ARN and URL
        const QString name = _ui->dependencyList->currentItem()->text();

        if (ApplicationDependencyDialog dialog(name, false, nullptr); dialog.exec() == Accepted) {
            _application.dependencies[index.row()] = dialog.GetName();
            _changed = true;
        }
    });

    // Connect add button
    connect(_ui->dependencyAddButton, &QPushButton::clicked, this, [=]() {

        if (ApplicationDependencyDialog dialog(nullptr, true, nullptr); dialog.exec() == Accepted) {
            if (!_application.dependencies.contains(dialog.GetName())) {
                _ui->dependencyList->addItems(QStringList(dialog.GetName()));
                _application.dependencies.append(dialog.GetName());
                _changed = true;
            }
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

void ApplicationEditDialog::ShowTagsContextMenu(const QPoint &pos) {

    // Cell index
    const QModelIndex index = _ui->tagTable->indexAt(pos);
    if (!index.isValid()) return;

    const int row = index.row();

    QMenu menu;
    QAction *editAction = menu.addAction(IconUtils::GetIcon("dark", "edit"), "Edit Tag");
    editAction->setToolTip("Edit the tag");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("dark", "delete"), "Delete Tag");
    deleteAction->setToolTip("Delete the tag");

    const QString key = _ui->tagTable->item(row, 0)->text();
    const QString value = _ui->tagTable->item(row, 1)->text();
    if (const QAction *selectedAction = menu.exec(_ui->tagTable->viewport()->mapToGlobal(pos)); selectedAction == editAction) {
        if (ApplicationTagDialog dialog(key, value, false); dialog.exec() == QDialog::Accepted) {
            SetColumn(_ui->tagTable, row, 1, dialog.GetValue());
            _application.tags[key] = dialog.GetValue();
            _changed = true;
        }
    } else if (selectedAction == deleteAction) {
        _application.tags.remove(key);
        _ui->tagTable->removeRow(row);
    }
}

void ApplicationEditDialog::ShowDependenciesContextMenu(const QPoint &pos) {

    // Cell index
    const QModelIndex index = _ui->dependencyList->indexAt(pos);
    if (!index.isValid()) return;

    const int row = index.row();

    QMenu menu;
    QAction *editAction = menu.addAction(IconUtils::GetIcon("dark", "edit"), "Edit Dependency");
    editAction->setToolTip("Edit the dependency");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("dark", "delete"), "Delete Dependency");
    deleteAction->setToolTip("Delete the dependency");

    const QString name = _ui->dependencyList->item(row)->text();
    if (const QAction *selectedAction = menu.exec(_ui->dependencyList->viewport()->mapToGlobal(pos)); selectedAction == editAction) {
        if (ApplicationDependencyDialog dialog(name, false); dialog.exec() == Accepted) {
            if (!_application.dependencies.contains(dialog.GetName())) {
                _ui->dependencyList->addItems(QStringList(dialog.GetName()));
                _application.dependencies.append(dialog.GetName());
                _changed = true;
            }
        }
    } else if (selectedAction == deleteAction) {
        _application.dependencies.remove(row);
        delete _ui->dependencyList->takeItem(row);
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
