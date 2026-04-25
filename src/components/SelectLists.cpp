//
// Created by vogje01 on 4/3/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SelectModules.h" resolved

#include <components/SelectLists.h>
#include "ui_SelectLists.h"

namespace Awsmock::Components {

    SelectLists::SelectLists(QWidget *parent) : QWidget(parent), _ui(new Ui::SelectLists) {

        // Modules service
        // _moduleService = new ModuleService();

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
    }

    SelectLists::~SelectLists() {
        delete _ui;
    }

    void SelectLists::SetAvailable(const QStringList &available) {
        _available = available;
        for (const auto &item: available) {
            _leftDataModel->appendRow(new QStandardItem(item));
        }
    }

    void SelectLists::SetSelected(const QStringList &selected) {
        _selected = selected;
        for (const auto &item: selected) {
            _rightDataModel->appendRow(new QStandardItem(item));
        }
        for (int i = _leftDataModel->rowCount() - 1; i >= 0; --i) {
            if (selected.contains(_leftDataModel->item(i)->text())) {
                _leftDataModel->removeRow(i);
            }
        }
    }

    void SelectLists::SetupListButtons() {
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
                        if (!_selected.contains(text)) {
                            _selected.append(text);
                        }
                    }
                }
            }
            _ui->availableView->clearSelection();
            _leftDataModel->sort(1);
            _rightDataModel->sort(1);
            emit SelectChangedSignal(_selected);
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
                        if (_selected.contains(text)) {
                            _selected.removeOne(text);
                        }
                    }
                }
            }
            _ui->exportView->clearSelection();
            _leftDataModel->sort(1);
            _rightDataModel->sort(1);
            emit SelectChangedSignal(_selected);
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
                        if (!_selected.contains(text)) {
                            _selected.append(text);
                        }
                    }
                }
            }
            _leftDataModel->clear();
            _rightDataModel->sort(1);
            emit SelectChangedSignal(_selected);
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
                    _selected.removeAll(text);
                }
            }
            _rightDataModel->clear();
            _leftDataModel->sort(1);
            emit SelectChangedSignal(_selected);
        });
    }

    QStringList SelectLists::GetSelected() {
        return _selected;
    }

} // Awsmock::Components
