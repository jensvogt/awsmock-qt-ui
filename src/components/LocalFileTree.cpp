//
// Created by vogje01 on 2/10/26.
//

#include <components/LocalFileTree.h>

LocalFileTree::LocalFileTree(const QString &rootFolder, QWidget *parent) : QWidget(parent) {

    _layout = new QVBoxLayout;
    setLayout(_layout);

    // 1. Create the model and the view
    _model = new QStandardItemModel(this);
    _model->setHorizontalHeaderLabels({"Name", "Size", "Type", "Last Modified", "Permission", "Owner", "Group"});

    // Root item
    _rootItem = _model->invisibleRootItem();
    _rootItem->setData("/", Qt::UserRole);

    // Setup model
    _folderProxyModel = new FolderFilterModel();
    _folderProxyModel->setSourceModel(_model);

    _folderTreeView = new QTreeView(this);
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
    _folderTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    // Enable sorting
    _folderTreeView->setSortingEnabled(true);
    _folderTreeView->sortByColumn(0, Qt::AscendingOrder);
    connect(_folderTreeView, &LocalFileTree::customContextMenuRequested, this, &LocalFileTree::ShowFolderContextMenu);
    connect(_folderTreeView, &QTreeView::doubleClicked, this, [this](const QModelIndex &index) {
        // Get the source index from the folder view click
        const QModelIndex sourceIndex = _folderProxyModel->mapToSource(index);

        // Map that source index into the file proxy's coordinate system
        const QModelIndex fileProxyIndex = _fileProxyModel->mapFromSource(sourceIndex);

        // Point the file view to look INSIDE that folder
        _fileTreeView->setRootIndex(fileProxyIndex);
        QStandardItem *parentItem = _model->itemFromIndex(sourceIndex);
        const QString absPath = parentItem->data(Qt::UserRole).toString();

        parentItem->removeRows(0, parentItem->rowCount());
        ScanFolder(parentItem->data(Qt::UserRole).toString(), parentItem);
    });

    // Setup model
    _fileProxyModel = new FileFilterModel();
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
    _fileTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    // Enable sorting
    _fileTreeView->setSortingEnabled(true);
    _fileTreeView->sortByColumn(0, Qt::AscendingOrder);
    connect(_fileTreeView, &LocalFileTree::customContextMenuRequested, this, &LocalFileTree::ShowFileContextMenu);

    // Synchronization
    connect(_folderTreeView, &QTreeView::clicked, this, [this](const QModelIndex &index) {
        // Get the source index from the folder view click
        const QModelIndex sourceIndex = _folderProxyModel->mapToSource(index);

        // Map that source index into the file proxy's coordinate system
        const QModelIndex fileProxyIndex = _fileProxyModel->mapFromSource(sourceIndex);

        // Point the file view to look INSIDE that folder
        _fileTreeView->setRootIndex(fileProxyIndex);
        QStandardItem *parentItem = _model->itemFromIndex(sourceIndex);
        const QString absPath = parentItem->data(Qt::UserRole).toString();

        parentItem->removeRows(0, parentItem->rowCount());
        emit FolderSelectedSignal(absPath, parentItem);
    });

    _layout->addWidget(_folderTreeView);
    _layout->addWidget(_fileTreeView);

    ScanFolder(rootFolder, _rootItem);
}

LocalFileTree::~LocalFileTree() = default;

void LocalFileTree::ScanFolder(const QString &rootFolder, QStandardItem *parent) const {
    const auto root = QDir(rootFolder);

    for (QFileInfoList files = root.entryInfoList(); const auto &file: files) {
        if (file.fileName() != ".." && file.fileName() != ".") {
            FileInfo fileInfo;
            fileInfo.permissions = ToUnitPermString(file);
            fileInfo.size = file.size();
            fileInfo.username = file.owner();
            fileInfo.groupname = file.group();
            fileInfo.name = file.fileName();
            fileInfo.path = file.absoluteFilePath();
            AddItem(fileInfo, parent);
        }
    }
}

void LocalFileTree::AddItem(const FileInfo &fileInfo, QStandardItem *parent) const {

    if (HasChild(parent->index(), 0, fileInfo.name, _model)) {
        return;
    }
    const QString parentPath = parent->data(Qt::UserRole).toString();
    const QString absPath = fileInfo.path;
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

    // Setup columns
    SetFileHeaders(_fileTreeView);
}

QIcon LocalFileTree::GetIcon(const QString &mimeType, const QString &fileType) {
    if (fileType == FTP_FILE_TYPE_FOLDER) {
        return IconUtils::GetIcon(fileType.toLower());
    }
    return QIcon::fromTheme("text-x-generic");
}

void LocalFileTree::Clear() const {
    _model->removeRows(0, _model->rowCount());
}

void LocalFileTree::HideColumns(const QVector<int> &columns) const {
    _folderTreeView->setHeaderHidden(true);
    for (const auto &column: columns) {
        _folderTreeView->setColumnHidden(column, true);
    }
}

void LocalFileTree::HideAllColumns() const {
    _folderTreeView->setHeaderHidden(true);
    for (int i = 0; i < _model->columnCount(); i++) {
        _folderTreeView->setColumnHidden(i, true);
    }
}

bool LocalFileTree::HasChild(const QModelIndex &parent, const int column, const QString &value, const QAbstractItemModel *model) {
    for (int row = 0; row < model->rowCount(parent); ++row) {
        QModelIndex index = model->index(row, column, parent);
        if (index.data().toString() == value)
            return true;
    }
    return false;
}

void LocalFileTree::ShowFileContextMenu(const QPoint &pos) {

    const auto proxy = qobject_cast<QSortFilterProxyModel *>(_fileTreeView->model());

    const QModelIndex proxyIndex = _fileTreeView->currentIndex();
    const QModelIndex sourceIndex = proxy->mapToSource(proxyIndex);

    if (!sourceIndex.isValid()) {
        return;
    }

    // If not valid, allow only create directory
    QMenu menu;
    if (!sourceIndex.isValid()) {
        QAction *addDirAction = menu.addAction(IconUtils::GetIcon("add-directory"), "Create directory");
        addDirAction->setToolTip("Create directory");
        if (const auto selectedAction = menu.exec(_fileTreeView->viewport()->mapToGlobal(pos)); selectedAction == addDirAction) {
            //TargetTreeAddDirectory();
        }
        return;
    }

    const QString name = sourceIndex.siblingAtColumn(0).data().toString();
    const QString absPath = sourceIndex.siblingAtColumn(0).data(Qt::UserRole).toString();

    if (name == "..") {
        return;
    }

    QAction *renameAction = menu.addAction(IconUtils::GetIcon("rename"), "Rename File");
    renameAction->setToolTip("Rename the file");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete File");
    deleteAction->setToolTip("Delete the file");

    if (const auto selectedAction = menu.exec(_fileTreeView->viewport()->mapToGlobal(pos)); selectedAction == renameAction) {
        emit TargetTreeFileRenameSignal(absPath);
    } else if (selectedAction == deleteAction) {
        _model->removeRow(sourceIndex.row(), sourceIndex.parent());
        _fileTreeView->clearSelection();
        emit TargetTreeFileDeleteSignal(absPath);
    }
}

void LocalFileTree::ShowFolderContextMenu(const QPoint &pos) {

    const auto proxy = qobject_cast<QSortFilterProxyModel *>(_folderTreeView->model());

    const QModelIndex proxyIndex = _folderTreeView->currentIndex();
    const QModelIndex sourceIndex = proxy->mapToSource(proxyIndex);
    if (!sourceIndex.isValid()) {
        return;
    }

    // If not valid, allow only create directory
    QMenu menu;
    if (!sourceIndex.isValid()) {
        QAction *addDirAction = menu.addAction(IconUtils::GetIcon("add-directory"), "Create directory");
        addDirAction->setToolTip("Create directory");
        if (const auto selectedAction = menu.exec(_fileTreeView->viewport()->mapToGlobal(pos)); selectedAction == addDirAction) {
            //TargetTreeAddDirectory();
        }
        return;
    }

    const QString name = sourceIndex.siblingAtColumn(0).data().toString();
    const QString absPath = sourceIndex.siblingAtColumn(0).data(Qt::UserRole).toString();
    const QString type = sourceIndex.siblingAtColumn(0).data(Qt::UserRole + 1).toString();

    QAction *renameAction = menu.addAction(IconUtils::GetIcon("rename"), "Rename Directory");
    renameAction->setToolTip("Rename the directory");

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Directory");
    deleteAction->setToolTip("Delete the directory");

    if (const auto selectedAction = menu.exec(_folderTreeView->viewport()->mapToGlobal(pos)); selectedAction == renameAction) {
        emit TargetTreeDirectoryRename(absPath);
    } else if (selectedAction == deleteAction) {
        emit TargetTreeDirectoryDelete(absPath);
    }
}

QString LocalFileTree::ToUnitPermString(const QFileInfo &info) {
    QString permString;
    permString += (info.isDir() ? "d" : "-");

    const QFile::Permissions p = info.permissions();

    permString += (p & QFileDevice::ReadOwner) ? "r" : "-";
    permString += (p & QFileDevice::WriteOwner) ? "w" : "-";
    permString += (p & QFileDevice::ExeOwner) ? "x" : "-";

    permString += (p & QFileDevice::ReadGroup) ? "r" : "-";
    permString += (p & QFileDevice::WriteGroup) ? "w" : "-";
    permString += (p & QFileDevice::ExeGroup) ? "x" : "-";

    permString += (p & QFileDevice::ReadOther) ? "r" : "-";
    permString += (p & QFileDevice::WriteOther) ? "w" : "-";
    permString += (p & QFileDevice::ExeOther) ? "x" : "-";

    return permString;
}

void LocalFileTree::SetFileHeaders(const QTreeView *treeView) {
    treeView->header()->setStretchLastSection(false);
    treeView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    treeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    treeView->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    treeView->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    treeView->header()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    treeView->header()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    treeView->header()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
}
