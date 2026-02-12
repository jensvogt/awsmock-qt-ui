//
// Created by vogje01 on 2/10/26.
//

#include <components/FTPFileTree.h>
#include "ui_FTPFileTree.h"

FTPFileTree::FTPFileTree(QStandardItem *root, QWidget *parent) : QWidget(parent), _ui(new Ui::FTPFileTree), _rootItem(root) {

    _layout = new QVBoxLayout;
    setLayout(_layout);

    // 1. Create the model and the view
    _model = new QStandardItemModel(this);
    _model->setHorizontalHeaderLabels({"Name", "Size", "Type", "Last Modified", "Permission", "Owner", "Group"});

    // Root item
    _rootItem = _model->invisibleRootItem();
    _rootItem->setData("/", Qt::UserRole);

    // Setup model
    _folderProxyModel = new FTPFolderFilterModel();
    _folderProxyModel->setSourceModel(_model);

    _folderTreeView = new DroppableTreeView(this);
    _folderTreeView->setModel(_folderProxyModel);
    _folderTreeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Setup columns
    _folderTreeView->header()->setStretchLastSection(false);
    _folderTreeView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    _folderTreeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    _folderTreeView->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    _folderTreeView->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    _folderTreeView->header()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    _folderTreeView->header()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    _folderTreeView->header()->setSectionResizeMode(6, QHeaderView::ResizeToContents);

    // Setup options
    _folderTreeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _folderTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _folderTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
    _folderTreeView->setAcceptDrops(true);
    _folderTreeView->setDropIndicatorShown(true);
    _folderTreeView->setDragEnabled(true);
    _folderTreeView->setDragDropMode(QAbstractItemView::DragDrop);

    // Enable sorting
    _folderTreeView->setSortingEnabled(true);
    _folderTreeView->sortByColumn(0, Qt::AscendingOrder);

    // connect(_folderTreeView, &QTreeView::doubleClicked, this, [this](const QModelIndex &index) {
    //
    //     const QModelIndex sourceIndex = _folderProxyModel->mapToSource(index);
    //
    //     QStandardItem *parentItem = _model->itemFromIndex(sourceIndex);
    //     const QString absPath = parentItem->data(Qt::UserRole).toString();
    //
    //     parentItem->removeRows(0, parentItem->rowCount());
    //
    //     emit FolderSelectedSignal(absPath, parentItem);
    // });

    connect(_folderTreeView, &QTreeView::clicked, this, [this](const QModelIndex &index) {
        QStandardItem *parentItem = _model->itemFromIndex(index);
    });

    // Setup model
    _fileProxyModel = new FTPFileFilterModel();
    _fileProxyModel->setSourceModel(_model);

    // File tree view
    _fileTreeView = new DroppableTreeView(this);
    _fileTreeView->setModel(_fileProxyModel);
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

    // Synchronization
    connect(_model, &QStandardItemModel::rowsInserted, _fileProxyModel, &QSortFilterProxyModel::invalidate);
    connect(_folderTreeView, &QTreeView::clicked, this, [this](const QModelIndex &index) {
        // Get the source index from the folder view click
        const QModelIndex sourceIndex = _folderProxyModel->mapToSource(index);

        // Map that source index into the file proxy's coordinate system
        QModelIndex fileProxyIndex = _fileProxyModel->mapFromSource(sourceIndex);

        // Point the file view to look INSIDE that folder
        _fileTreeView->setRootIndex(fileProxyIndex);
        QStandardItem *parentItem = _model->itemFromIndex(sourceIndex);
        const QString absPath = parentItem->data(Qt::UserRole).toString();

        parentItem->removeRows(0, parentItem->rowCount());
        emit FolderSelectedSignal(absPath, parentItem);
    });

    _layout->addWidget(_folderTreeView);
    _layout->addWidget(_fileTreeView);
}

FTPFileTree::~FTPFileTree() {
    delete _ui;
}

void FTPFileTree::AddItem(const FileInfo &fileInfo, QStandardItem *parent) const {

    const QString parentPath = parent->data(Qt::UserRole).toString();
    const QString absPath = parentPath.endsWith("/") ? parentPath + fileInfo.name : parentPath + "/" + fileInfo.name;
    const QString fileType = fileInfo.permissions.startsWith("d") ? FTP_FILE_TYPE_FOLDER : FTP_FILE_TYPE_FILE;

    const QMimeType mime = _mimeDb.mimeTypeForFile(fileInfo.name);
    auto size = new QStandardItem(QString::number(fileInfo.size));
    auto contentType = new QStandardItem(mime.name());
    auto modified = new QStandardItem(fileInfo.timestamp);
    auto perm = new QStandardItem(fileInfo.permissions);
    auto userName = new QStandardItem(fileInfo.username);
    auto groupName = new QStandardItem(fileInfo.groupname);

    auto *item = new QStandardItem(GetIcon(fileInfo.contentType, fileType), fileInfo.name);
    item->setData(absPath, Qt::UserRole);
    item->setData(fileType, Qt::UserRole + 1);
    parent->appendRow({item, size, contentType, modified, perm, userName, groupName});

    // Expand
    if (fileType == FTP_FILE_TYPE_FOLDER && fileInfo.name != "..") {
        _folderTreeView->expand(parent->index());
    }
}

QIcon FTPFileTree::GetIcon(const QString &mimeType, const QString &fileType) {
    if (fileType == FTP_FILE_TYPE_FOLDER) {
        return IconUtils::GetIcon(fileType.toLower());
    }
    return QIcon::fromTheme("text-x-generic");
}

void FTPFileTree::Clear() const {
    _model->removeRows(0, _model->rowCount());
}

void FTPFileTree::HideColumns(const QVector<int> &columns) const {
    _folderTreeView->setHeaderHidden(true);
    for (const auto &column: columns) {
        _folderTreeView->setColumnHidden(column, true);
    }
}

void FTPFileTree::HideAllColumns() const {
    _folderTreeView->setHeaderHidden(true);
    for (int i = 0; i < _model->columnCount(); i++) {
        _folderTreeView->setColumnHidden(i, true);
    }
}
