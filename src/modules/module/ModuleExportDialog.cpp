//
// Created by jensv on 18/02/2026.
//

#include <modules/module/ModuleExportDialog.h>
#include "ui_ModuleExportDialog.h"

QStringList ModuleExportDialog::_excluded = {"gateway", "monitoring"};

ModuleExportDialog::ModuleExportDialog(QWidget *parent) : BaseDialog(parent), _ui(new Ui::ModuleExportDialog) {

    // Modules service
    _moduleService = new ModuleService();

    // Setup IU components
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &ModuleExportDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &ModuleExportDialog::HandleReject);

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

    // Setup filename
    _ui->filenameEdit->setText(tr(nullptr));
    _ui->browseButton->setText(nullptr);
    _ui->browseButton->setIcon(IconUtils::GetIcon("browse"));
    connect(_ui->browseButton, &QPushButton::clicked, this, &ModuleExportDialog::HandleBrowseButton);

    // List connection
    connect(_moduleService, &ModuleService::ListModuleNamesSignal, this, &ModuleExportDialog::LoadAvailableModules);

    // Check boxes
    _ui->prettyPrintCheck->setChecked(_prettyPrint);

    // Export type radio group
    _exportTypeGroup = new QButtonGroup(this);
    _exportTypeGroup->addButton(_ui->infrastructureRadio, 0);
    _exportTypeGroup->addButton(_ui->objectsRadio, 1);
    _exportTypeGroup->addButton(_ui->bothRadio, 2);
    _ui->infrastructureRadio->setChecked(true);

    ModuleExportDialog::LoadContent();
}

ModuleExportDialog::~ModuleExportDialog() {
    delete _moduleService;
    delete _ui;
}

void ModuleExportDialog::LoadContent() {
    _moduleService->ListModuleNames();
}

void ModuleExportDialog::LoadAvailableModules(const ListModuleNamesResponse &response) const {
    _leftDataModel->removeRows(0, _leftDataModel->rowCount());
    for (auto r = 0; r < response.moduleNames.count(); r++) {
        if (!_excluded.contains(response.moduleNames.at(r))) {
            _leftDataModel->appendRow(new QStandardItem(response.moduleNames.at(r)));
        }
    }
}

void ModuleExportDialog::SetupListButtons() {
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
                }
            }
        }
        _ui->availableView->clearSelection();
        _leftDataModel->sort(1);
        _rightDataModel->sort(1);
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
                }
            }
        }
        _ui->exportView->clearSelection();
        _leftDataModel->sort(1);
        _rightDataModel->sort(1);
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
                }
            }
        }
        _leftDataModel->clear();
        _rightDataModel->sort(1);
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
            }
        }
        _rightDataModel->clear();
        _leftDataModel->sort(1);
    });
}

void ModuleExportDialog::HandleBrowseButton() {

    // Create a QFileDialog set to select existing files
    const auto filter = "JSON Files (*.json);;All Files (*.*)";
    const auto defaultDir = Configuration::instance().GetValue<QString>("ui.default-directory.ExportInfrastructure", "/usr/local/awsmock-qt-_ui");

    if (const QString filePath = QFileDialog::getSaveFileName(nullptr, "Open JSON Configuration File", defaultDir, filter); !filePath.isEmpty()) {
        _exportFilePath = filePath.toUtf8();
        _ui->filenameEdit->setText(filePath);
    }
}

void ModuleExportDialog::WriteInfrastructureFile(const QString &infrastructure) const {

    QString output = infrastructure;
    // Pretty print
    if (_prettyPrint) {
        output = JsonUtils::PrettyPrint(infrastructure);
    }

    // Save JSON data
    QFile file(_exportFilePath);
    file.write(output.toUtf8());
    file.close();
}

void ModuleExportDialog::HandleAccept() {

    // Get all export modules
    const int rowCount = _rightDataModel->rowCount();
    for (int i = 0; i < rowCount; ++i) {

        // Get the index for the current row
        QModelIndex index = _rightDataModel->index(i, 0);

        // Pull the data (text)
        QString text = _rightDataModel->data(index, Qt::DisplayRole).toString();

        // Pull custom data (if you stored IDs or pointers earlier)
        QVariant customData = _rightDataModel->data(index, Qt::UserRole);

        _selectedModules << text;
    }
    if (_selectedModules.isEmpty()) {
        QMessageBox::warning(nullptr, "Error", "Selected module list cannot be empty!");
        return;
    }
    if (_exportFilePath.isEmpty()) {
        QMessageBox::warning(nullptr, "Error", "Output file cannot be empty!");
        return;
    }
    accept();
}

QString ModuleExportDialog::GetFilePath() {
    return _exportFilePath;
}

QStringList ModuleExportDialog::GetModules() {
    return _selectedModules;
}

ExportType ModuleExportDialog::GetExportType() const {
    if (_exportTypeGroup->checkedId() == 0) {
        return INFRA_STRUCTURE;
    }
    if (_exportTypeGroup->checkedId() == 1) {
        return OBJECTS;
    }
    return BOTH;
}

void ModuleExportDialog::HandleReject() {
    reject();
}
