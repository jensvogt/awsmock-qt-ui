//
// Created by vogje01 on 2/10/26.
//

#include <components/FTPFileTree.h>
#include "ui_FTPFileTree.h"
#include "utils/IconUtils.h"

FTPFileTree::FTPFileTree(QWidget *parent) : QWidget(parent), _ui(new Ui::FTPFileTree) {

    _layout = new QVBoxLayout;
    setLayout(_layout);

    // 1. Create the model and the view
    _model = new QStandardItemModel(this);
    _model->setHorizontalHeaderLabels({"Name", "Size", "Type", "Last Modified", "Permission", "Owner", "Group", "FileType", "AbsPath"});

    // Root item
    _rootItem = _model->invisibleRootItem();
    _currentParent = _rootItem;

    // Setup model
    _fileTreeView = new DroppableTreeView(this);
    _fileTreeView->setModel(_model);
    _fileTreeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Setup columns
    _fileTreeView->header()->setStretchLastSection(false);
    _fileTreeView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    _fileTreeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    _fileTreeView->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    _fileTreeView->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    _fileTreeView->header()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    _fileTreeView->header()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    _fileTreeView->header()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    _fileTreeView->setColumnHidden(7, true);
    _fileTreeView->setColumnHidden(8, true);


    // Setup options
    _fileTreeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _fileTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _fileTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
    _fileTreeView->setAcceptDrops(true);
    _fileTreeView->setDropIndicatorShown(true);
    _fileTreeView->setDragEnabled(true);
    _fileTreeView->setDragDropMode(QAbstractItemView::DragDrop);

    // Enable sorting
    _fileTreeView->setSortingEnabled(true);
    _fileTreeView->sortByColumn(0, Qt::AscendingOrder);

    connect(_fileTreeView, &QTreeView::doubleClicked, this, [this](const QModelIndex &index) {
        _currentParent = _model->itemFromIndex(index);
        if (const QString type = _model->item(_currentParent->row(), 7)->text(); type != "folder")
            return;

        const QString absPath = _model->item(_currentParent->row(), 8)->text();

        _currentParent->removeRows(0, _currentParent->rowCount());
        emit FolderSelectedSignal(absPath);
    });

    _layout->addWidget(_fileTreeView);
}

FTPFileTree::~FTPFileTree() {
    delete _ui;
}

void FTPFileTree::AddFolder(const QString &name) const {

    if (name == "..") {
        return;
    }

    auto absPath = new QStandardItem("/" + name);
    if (!_currentParent->text().isEmpty()) {
        absPath = new QStandardItem(_currentParent->text() + "/" + name);
    }
    auto *folder = new QStandardItem(IconUtils::GetIcon("folder"), name);
    auto *fileType = new QStandardItem("folder");
    _currentParent->appendRow({folder, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, fileType, absPath});
}

void FTPFileTree::AddFile(const QString &name, const long fileSize, const QString &modifiedDate, const QString &permissions, const QString &user, const QString &group) const {

    const QMimeType mime = _mimeDb.mimeTypeForFile(name);
    const QIcon icon = GetIcon(mime.name());

    auto file = new QStandardItem(icon, name);
    auto *size = new QStandardItem(QString::number(fileSize));
    auto *type = new QStandardItem(mime.name());
    auto *modified = new QStandardItem(modifiedDate);
    auto *perm = new QStandardItem(permissions);
    auto *userName = new QStandardItem(user);
    auto *groupName = new QStandardItem(group);
    auto *fileType = new QStandardItem("file");
    qInfo() << "Add file: " << name << " to folder: " << _model->item(_currentParent->row(), 0);
    _rootItem->appendRow({file, size, type, modified, perm, userName, groupName, fileType, nullptr});
}

QIcon FTPFileTree::GetIcon(const QString &mimeType) {
    return QIcon::fromTheme("text-x-generic");
    // if (mimeType.startsWith("image")){
    //
    // }
}

void FTPFileTree::Clear() const {
    _model->removeRows(0, _model->rowCount());
}

void FTPFileTree::HideColumns(const QVector<int> &columns) const {
    _fileTreeView->setHeaderHidden(true);
    for (const auto &column: columns) {
        _fileTreeView->setColumnHidden(column, true);
    }
}

void FTPFileTree::HideAllColumns() const {
    _fileTreeView->setHeaderHidden(true);
    for (int i = 0; i < _model->columnCount(); i++) {
        _fileTreeView->setColumnHidden(i, true);
    }
}
