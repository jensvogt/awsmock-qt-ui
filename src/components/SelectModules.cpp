//
// Created by vogje01 on 4/3/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SelectModules.h" resolved

#include <QGroupBox>
#include <components/SelectModules.h>
#include "ui_SelectModules.h"

//namespace Awsmock::Components {

QStringList SelectModules::_excluded = {"gateway", "monitoring"};

SelectModules::SelectModules(QWidget *parent) : QWidget(parent), _ui(new Ui::SelectModules) {

    // Modules service
    _moduleService = new ModuleService();

    // Setup IU components
    _ui->setupUi(this);

    // Left view
    _leftDataModel = new QStandardItemModel(_ui->availableView);
    _leftDataModel->sort(1);
    _ui->availableView->setModel(_leftDataModel);
    _ui->availableView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    // Right view
    _rightDataModel = new QStandardItemModel(_ui->exportView);
    _rightDataModel->sort(1);
    _ui->exportView->setModel(_rightDataModel);
    _ui->exportView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    // Buttons
    _ui->left2RightButton->setText(tr(nullptr));
    _ui->left2RightButton->setIcon(IconUtils::GetIcon("left-2-right"));
    _ui->right2LeftButton->setText(tr(nullptr));
    _ui->right2LeftButton->setIcon(IconUtils::GetIcon("right-2-left"));
    _ui->allLeft2RightButton->setText(tr(nullptr));
    _ui->allLeft2RightButton->setIcon(IconUtils::GetIcon("all-left-2-right"));
    _ui->allRight2LeftButton->setText(tr(nullptr));
    _ui->allRight2LeftButton->setIcon(IconUtils::GetIcon("all-right-2-left"));

    // Button connection
    SetupListButtons();

    // List connection
    connect(_moduleService, &ModuleService::ListModuleNamesSignal, this, &SelectModules::LoadAvailableModules);

    // Check boxes
    _ui->prettyPrintCheck->setChecked(_prettyPrint);
    connect(_ui->prettyPrintCheck, &QCheckBox::toggled, this, [this](const bool checked) {
        _prettyPrint = checked;
        emit PrettyPrintChangedSignal(_prettyPrint);
    });

    // Export type radio group
    _exportTypeGroup = new QButtonGroup(this);
    _exportTypeGroup->addButton(_ui->infrastructureRadio, 0);
    _exportTypeGroup->addButton(_ui->objectsRadio, 1);
    _exportTypeGroup->addButton(_ui->bothRadio, 2);
    _ui->infrastructureRadio->setChecked(true);
    connect(_exportTypeGroup, &QButtonGroup::idToggled, this, [this](const int id, const bool checked) {
        if (checked) {
            switch (id) {
                case 0: _exportType = INFRA_STRUCTURE;
                    break;
                case 1: _exportType = OBJECTS;
                    break;
                case 2: _exportType = BOTH;
                    break;
                default: _exportType = INFRA_STRUCTURE;
            }
            emit ExportTypeChangedSignal(_exportType);
        }
    });

    // Load the modules from server
    _moduleService->ListModuleNames();
}

SelectModules::~SelectModules() {
    delete _ui;
}

void SelectModules::SetupListButtons() {
    connect(_ui->left2RightButton, &QPushButton::clicked, [this] {

        // Get the selection model
        const QItemSelectionModel *select = _ui->availableView->selectionModel();

        // Sort indices in descending order by row
        QModelIndexList selectedItems = select->selectedIndexes();
        std::ranges::sort(selectedItems, [](const QModelIndex &a, const QModelIndex &b) {
            return a.row() > b.row();
        });

        // Iterate and access data
        foreach(const QModelIndex &index, selectedItems) {
            if (QString text = index.data(Qt::DisplayRole).toString(); _rightDataModel->findItems(text).isEmpty()) {
                if (!text.isEmpty()) {
                    _rightDataModel->appendRow(new QStandardItem(text));
                    _leftDataModel->removeRow(index.row());
                    if (!_selectedModules.contains(text)) {
                        _selectedModules.append(text);
                    }
                }
            }
        }
        _ui->availableView->clearSelection();
        _leftDataModel->sort(1);
        _rightDataModel->sort(1);
        ExportType i;
        emit ModulesSelectSignal(_selectedModules, ExportTypeFromIndex(_exportTypeGroup->checkedId()), _ui->prettyPrintCheck->isChecked());
    });
    connect(_ui->right2LeftButton, &QPushButton::clicked, [this] {

        // Get the selection model
        const QItemSelectionModel *select = _ui->exportView->selectionModel();

        // Sort indices in descending order by row
        QModelIndexList selectedItems = select->selectedIndexes();
        std::ranges::sort(selectedItems, [](const QModelIndex &a, const QModelIndex &b) {
            return a.row() > b.row();
        });

        // Iterate and access data
        foreach(const QModelIndex &index, selectedItems) {
            if (QString text = index.data(Qt::DisplayRole).toString(); _leftDataModel->findItems(text).isEmpty()) {
                if (!text.isEmpty()) {
                    _leftDataModel->appendRow(new QStandardItem(text));
                    _rightDataModel->removeRow(index.row());
                    if (_selectedModules.contains(text)) {
                        _selectedModules.removeOne(text);
                    }
                }
            }
        }
        _ui->exportView->clearSelection();
        _leftDataModel->sort(1);
        _rightDataModel->sort(1);
        emit ModulesSelectSignal(_selectedModules, ExportTypeFromIndex(_exportTypeGroup->checkedId()), _ui->prettyPrintCheck->isChecked());
    });
    connect(_ui->allLeft2RightButton, &QPushButton::clicked, [this] {

        _ui->availableView->selectAll();
        // Get the selection model
        const QItemSelectionModel *select = _ui->availableView->selectionModel();

        // Sort indices in descending order by row
        QModelIndexList selectedItems = select->selectedIndexes();
        std::ranges::sort(selectedItems, [](const QModelIndex &a, const QModelIndex &b) {
            return a.row() > b.row();
        });

        // Iterate and access data
        foreach(const QModelIndex &index, selectedItems) {
            if (QString text = index.data(Qt::DisplayRole).toString(); _rightDataModel->findItems(text).isEmpty()) {
                if (!text.isEmpty()) {
                    _rightDataModel->appendRow(new QStandardItem(text));
                    if (!_selectedModules.contains(text)) {
                        _selectedModules.append(text);
                    }
                }
            }
        }
        _leftDataModel->clear();
        _rightDataModel->sort(1);
        emit ModulesSelectSignal(_selectedModules, ExportTypeFromIndex(_exportTypeGroup->checkedId()), _ui->prettyPrintCheck->isChecked());
    });
    connect(_ui->allRight2LeftButton, &QPushButton::clicked, [this] {

        _ui->exportView->selectAll();

        // Get the selection model
        const QItemSelectionModel *select = _ui->exportView->selectionModel();

        // Sort indices in descending order by row
        QModelIndexList selectedItems = select->selectedIndexes();
        std::ranges::sort(selectedItems, [](const QModelIndex &a, const QModelIndex &b) {
            return a.row() > b.row();
        });

        // Iterate and access data
        foreach(const QModelIndex &index, selectedItems) {
            if (QString text = index.data(Qt::DisplayRole).toString(); _leftDataModel->findItems(text).isEmpty()) {
                if (!text.isEmpty()) {
                    _leftDataModel->appendRow(new QStandardItem(text));
                }
                _selectedModules.removeAll(text);
            }
        }
        _rightDataModel->clear();
        _leftDataModel->sort(1);
        emit ModulesSelectSignal(_selectedModules, ExportTypeFromIndex(_exportTypeGroup->checkedId()), _ui->prettyPrintCheck->isChecked());
    });
}

void SelectModules::LoadAvailableModules(const ListModuleNamesResponse &response) const {
    _leftDataModel->removeRows(0, _leftDataModel->rowCount());
    for (auto r = 0; r < response.moduleNames.count(); r++) {
        if (!_excluded.contains(response.moduleNames.at(r))) {
            _leftDataModel->appendRow(new QStandardItem(response.moduleNames.at(r)));
        }
    }
}

QStringList SelectModules::GetModules() {
    return _selectedModules;
}

ExportType SelectModules::GetExportType() const {
    if (_exportTypeGroup->checkedId() == 0) {
        return INFRA_STRUCTURE;
    }
    if (_exportTypeGroup->checkedId() == 1) {
        return OBJECTS;
    }
    return BOTH;
}

bool SelectModules::GetPrettyPrint() const {
    return _prettyPrint;
}

//} // Awsmock::Components
