//
// Created by vogje01 on 2/10/26.
//

#include <components/FTPFileTree.h>

// ─────────────────────────────────────────────────────────────────────────────
// Construction / destruction
// ─────────────────────────────────────────────────────────────────────────────

FTPFileTree::FTPFileTree(QWidget *parent) : QWidget(parent) {

    _layout = new QVBoxLayout;
    _layout->setContentsMargins(0, 0, 0, 0);
    _layout->setSpacing(2);
    setLayout(_layout);

    // Setup FTP connection
    SetupFtpConnection();

    // Setup shared model + folder proxy
    SetupModel();

    // ── Folder tree (top panel) ───────────────────────────────────────────
    _folderTreeView = new QTreeView(this);
    _folderTreeView->setModel(_folderProxyModel);
    _folderTreeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _folderTreeView->setHeaderHidden(true);
    // Hide all extra columns – only the name column is shown in the folder tree
    HideColumns({1, 2, 3, 4, 5, 6, 7, 8});

    _folderTreeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _folderTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _folderTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
    _folderTreeView->setAcceptDrops(false);
    _folderTreeView->setUniformRowHeights(true);
    // Keep sorting enabled: ".." starts with '.' (ASCII 46) so it always sorts
    // before any letter, staying naturally at the top of each directory.
    _folderTreeView->setSortingEnabled(true);
    _folderTreeView->sortByColumn(0, Qt::AscendingOrder);
    _folderTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(_folderTreeView, &FTPFileTree::customContextMenuRequested,
            this, &FTPFileTree::ShowFolderContextMenu);

    // Folder tree click: navigate file view and, for "..", go up the hierarchy
    connect(_folderTreeView, &QTreeView::clicked, this, [this](const QModelIndex &proxyIndex) {
        const QModelIndex sourceIndex = _folderProxyModel->mapToSource(proxyIndex);
        QStandardItem *clickedItem = _model->itemFromIndex(sourceIndex);
        if (!clickedItem)
            return;

        if (const QString type = clickedItem->data(Qt::UserRole + 1).toString(); type == QLatin1String(FTP_FILE_TYPE_PARENT)) {
            // ".." clicked ── navigate up one level in the model.
            // clickedItem          = the ".." item
            // clickedItem->parent()  = the current directory item
            // clickedItem->parent()->parent() = target directory (or invisible root)
            const QStandardItem *currentDir = clickedItem->parent();
            const QStandardItem *targetDir = currentDir ? currentDir->parent() : nullptr;

            const QModelIndex targetSourceIndex = targetDir ? targetDir->index() : QModelIndex();
            const QModelIndex targetFileProxyIndex = _fileProxyModel->mapFromSource(targetSourceIndex);
            _fileTreeView->setRootIndex(targetFileProxyIndex);

            // Also move the folder-tree selection to the target directory
            const QModelIndex targetFolderProxyIndex = _folderProxyModel->mapFromSource(targetSourceIndex);
            _folderTreeView->setCurrentIndex(targetFolderProxyIndex);
            return;
        }

        // Regular folder – show its files and trigger an FTP listing
        const QModelIndex fileProxyIndex = _fileProxyModel->mapFromSource(sourceIndex);
        _fileTreeView->setRootIndex(fileProxyIndex);

        const QString absPath = clickedItem->data(Qt::UserRole).toString();
        TargetFolderSelectionChanged(absPath, clickedItem);
    });

    // ── File view (bottom panel) ──────────────────────────────────────────
    _fileProxyModel = new FileFilterModel();
    _fileProxyModel->setSourceModel(_model);

    _fileTreeView = new DroppableTreeView(this);
    _fileTreeView->setModel(_fileProxyModel);
    _fileTreeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    _fileTreeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _fileTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _fileTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
    _fileTreeView->setUniformRowHeights(true);
    _fileTreeView->setRootIsDecorated(false); // flat list – no expand arrows
    _fileTreeView->setAcceptDrops(true);
    _fileTreeView->setDropIndicatorShown(true);
    _fileTreeView->setDragEnabled(true);
    _fileTreeView->setDragDropMode(QAbstractItemView::DragDrop);
    _fileTreeView->setSortingEnabled(true);
    _fileTreeView->sortByColumn(0, Qt::AscendingOrder);
    _fileTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    SetFileHeaders(_fileTreeView);

    connect(_fileTreeView, &FTPFileTree::customContextMenuRequested,
            this, &FTPFileTree::ShowFileContextMenu);
    connect(_fileTreeView, &DroppableTreeView::FileDropped,
            this, &FTPFileTree::TargetTreeFileDropped);

    // Delete key shortcut for the file view
    const auto *deleteShortcut = new QShortcut(QKeySequence::Delete, _fileTreeView);
    connect(deleteShortcut, &QShortcut::activated, this, [this]() {
        const QModelIndex proxyIndex = _fileTreeView->currentIndex();
        if (!proxyIndex.isValid())
            return;
        const QModelIndex sourceIndex = _fileProxyModel->mapToSource(proxyIndex);
        const QString fullPath = sourceIndex.data(Qt::UserRole).toString();
        _model->removeRow(sourceIndex.row(), sourceIndex.parent());
        TargetTreeFileDelete(fullPath);
    });

    connect(&EventBus::instance(), &EventBus::FtpUploadSignal, this, [this](const QString &path) {
        TargetTreeFileDropped(path);
    });

    // ── Toolbar ───────────────────────────────────────────────────────────
    _menuBarLayout = new QHBoxLayout(this);
    _menuBarLayout->setContentsMargins(0, 0, 0, 0);

    const auto spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    _menuBarLayout->addWidget(spacer);

    auto *fileRefreshPushButton = new QPushButton(IconUtils::GetIcon("refresh"), nullptr);
    fileRefreshPushButton->setToolTip("Refresh");
    connect(fileRefreshPushButton, &QPushButton::clicked, this, &FTPFileTree::RefreshFileView);
    _menuBarLayout->addWidget(fileRefreshPushButton);

    // ── Assemble ──────────────────────────────────────────────────────────
    _layout->addWidget(_folderTreeView);
    _layout->addLayout(_menuBarLayout);
    _layout->addWidget(_fileTreeView);
}

FTPFileTree::~FTPFileTree() = default;

// ─────────────────────────────────────────────────────────────────────────────
// Public API
// ─────────────────────────────────────────────────────────────────────────────
void FTPFileTree::SetupFtpConnection() {
    _ftpClientThread = new FTPClientThread();
    connect(_ftpClientThread, &FTPClientThread::emitFileListItem, this, &FTPFileTree::AddItem);
    connect(_ftpClientThread, &FTPClientThread::emitSuccess, this, &FTPFileTree::ConnectionSucceeded);
    connect(_ftpClientThread, &FTPClientThread::finished, _ftpClientThread, &FTPClientThread::stop);
}

void FTPFileTree::Connect(const QString &server, const QString &port,
                          const QString &user, const QString &password) {
    if (!_ftpClientThread->isRunning() && !_connected) {
        _ftpClientThread->curClient->login(server, stoi(port.toStdString()), user, password);
        _ftpClientThread->task = TConnect;
        _ftpClientThread->parent = _rootItem;
        _ftpClientThread->start();
        _connected = true;
        emit ConnectionSucceeded();
    }
}

void FTPFileTree::Disconnect() {
    if (_connected) {
        _ftpClientThread->task = TDisconnect;
        _ftpClientThread->start();
        _model->removeRows(0, _model->rowCount());
        _connected = false;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Private helpers
// ─────────────────────────────────────────────────────────────────────────────

void FTPFileTree::SetupModel() {
    _model = new QStandardItemModel(this);
    _model->setHorizontalHeaderLabels(
        {"Name", "Size", "Type", "Last Modified", "Permission", "Owner", "Group"});

    _rootItem = _model->invisibleRootItem();
    _rootItem->setData("/", Qt::UserRole);

    _folderProxyModel = new FolderFilterModel();
    _folderProxyModel->setSourceModel(_model);
}

void FTPFileTree::AddItem(const FileInfo &fileInfo, QStandardItem *parent) const {

    if (HasChild(parent->index(), 0, fileInfo.name, _model))
        return;

    // ── Prepend ".." the first time this non-root directory is populated ──
    if (parent != _rootItem && !HasChild(parent->index(), 0, "..", _model)) {
        const QString currentPath = parent->data(Qt::UserRole).toString();
        const QString parentPath = FtpParentPath(currentPath);

        auto *dotDot = new QStandardItem(GetIcon(QString(), QString(FTP_FILE_TYPE_FOLDER)), "..");
        dotDot->setData(parentPath, Qt::UserRole);
        dotDot->setData(QString(FTP_FILE_TYPE_PARENT), Qt::UserRole + 1);

        // Insert as a full-width row so column counts stay consistent
        parent->insertRow(0, {
                              dotDot,
                              new QStandardItem(),
                              new QStandardItem(),
                              new QStandardItem(),
                              new QStandardItem(),
                              new QStandardItem(),
                              new QStandardItem()
                          });
    }

    // ── Regular item ──────────────────────────────────────────────────────
    const QString parentPath = parent->data(Qt::UserRole).toString();
    const QString absPath = parentPath.endsWith('/')
                                ? parentPath + fileInfo.name
                                : parentPath + '/' + fileInfo.name;
    const QString fileType = fileInfo.permissions.startsWith('d')
                                 ? FTP_FILE_TYPE_FOLDER
                                 : FTP_FILE_TYPE_FILE;

    const QMimeType mime = _mimeDb.mimeTypeForFile(fileInfo.name);

    auto *item = new QStandardItem(GetIcon(fileInfo.contentType, fileType), fileInfo.name);
    auto *size = new QStandardItem(QString::number(fileInfo.size));
    auto *ctype = new QStandardItem(mime.name());
    auto *modified = new QStandardItem(fileInfo.timestamp);
    auto *perm = new QStandardItem(fileInfo.permissions);
    auto *userName = new QStandardItem(fileInfo.username);
    auto *groupName = new QStandardItem(fileInfo.groupname);

    item->setData(absPath, Qt::UserRole);
    item->setData(QString(fileType), Qt::UserRole + 1);

    parent->appendRow({item, size, ctype, modified, perm, userName, groupName});

    if (fileType == FTP_FILE_TYPE_FOLDER)
        _folderTreeView->expand(_folderProxyModel->mapFromSource(parent->index()));

    SetFileHeaders(_fileTreeView);
}

void FTPFileTree::TargetFolderSelectionChanged(const QString &absPath, QStandardItem *parent) const {
    if (!_ftpClientThread->isRunning()) {
        _ftpClientThread->arglist[0] = absPath.toStdString();
        _ftpClientThread->parent = parent;
        _ftpClientThread->task = TCd;
        _ftpClientThread->start();
    }
}

void FTPFileTree::TargetTreeFileDropped(const QString &filePath) const {
    if (_connected) {
        _ftpClientThread->task = TUp;
        _ftpClientThread->arglist[0] = filePath.toStdString();
        _ftpClientThread->start();
    }
}

void FTPFileTree::TargetTreeFileDelete(const QString &filePath) const {
    _ftpClientThread->task = TDele;
    _ftpClientThread->arglist[0] = filePath.toStdString();
    _ftpClientThread->start();
}

void FTPFileTree::TargetTreeFileRename(const QString &filePath) {
    bool ok;
    const QString newName = QInputDialog::getText(
        this, "New File Name", "Enter new name:", QLineEdit::Normal, QString(), &ok);
    if (!ok || newName.isEmpty())
        return;

    _ftpClientThread->task = TRename;
    _ftpClientThread->arglist[0] = filePath.toStdString();
    _ftpClientThread->arglist[1] = newName.toStdString();
    _ftpClientThread->start();
}

void FTPFileTree::TargetTreeAddDirectory() {
    bool ok;
    const QString dirName = QInputDialog::getText(
        this, "Create Directory", "Enter directory name:", QLineEdit::Normal, QString(), &ok);
    if (!ok || dirName.isEmpty())
        return;

    _ftpClientThread->task = TMkd;
    _ftpClientThread->arglist[0] = dirName.toStdString();
    _ftpClientThread->start();
}

void FTPFileTree::TargetTreeDirectoryDelete(const QString &filePath) const {
    _ftpClientThread->task = TRmd;
    _ftpClientThread->arglist[0] = filePath.toStdString();
    _ftpClientThread->start();
}

void FTPFileTree::TargetTreeDirectoryRename(const QString &filePath) {
    bool ok;
    const QString newName = QInputDialog::getText(
        this, "New Directory Name", "Enter new name:", QLineEdit::Normal, QString(), &ok);
    if (!ok || newName.isEmpty())
        return;

    _ftpClientThread->task = TRename;
    _ftpClientThread->arglist[0] = filePath.toStdString();
    _ftpClientThread->arglist[1] = newName.toStdString();
    _ftpClientThread->start();
}

void FTPFileTree::RefreshFileView() const {
    QStandardItem *item;
    if (const QModelIndex proxyIndex = _folderTreeView->currentIndex(); proxyIndex.isValid()) {
        const QModelIndex sourceIndex = _folderProxyModel->mapToSource(proxyIndex);
        item = _model->itemFromIndex(sourceIndex);
        // Don't refresh on the ".." virtual item
        if (!item || item->data(Qt::UserRole + 1).toString() == QLatin1String(FTP_FILE_TYPE_PARENT))
            item = _rootItem;
    } else {
        item = _rootItem;
    }

    if (!_ftpClientThread->isRunning()) {
        _ftpClientThread->parent = item;
        _ftpClientThread->task = TPwd;
        _ftpClientThread->start();
        connect(_ftpClientThread, &FTPClientThread::emitPwd, this, [this, item](const QString &cwd) {
            if (!_ftpClientThread->isRunning()) {
                _ftpClientThread->arglist[0] = cwd.toStdString();
                _ftpClientThread->parent = item;
                _ftpClientThread->task = TList;
                _ftpClientThread->start();
            }
        });
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Context menus
// ─────────────────────────────────────────────────────────────────────────────

void FTPFileTree::ShowFolderContextMenu(const QPoint &pos) {
    const auto *proxy = qobject_cast<QSortFilterProxyModel *>(_folderTreeView->model());
    const QModelIndex sourceIndex = proxy->mapToSource(_folderTreeView->currentIndex());
    if (!sourceIndex.isValid())
        return;

    // Skip the ".." virtual item
    if (sourceIndex.siblingAtColumn(0).data(Qt::UserRole + 1).toString()
        == QLatin1String(FTP_FILE_TYPE_PARENT))
        return;

    const QString absPath = sourceIndex.siblingAtColumn(0).data(Qt::UserRole).toString();

    QMenu menu;
    QAction *renameAction = menu.addAction(IconUtils::GetIcon("rename"), "Rename Directory");
    renameAction->setToolTip("Rename the directory");
    menu.addSeparator();
    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Directory");
    deleteAction->setToolTip("Delete the directory");

    const auto selected = menu.exec(_folderTreeView->viewport()->mapToGlobal(pos));
    if (selected == renameAction)
        TargetTreeDirectoryRename(absPath);
    else if (selected == deleteAction)
        TargetTreeDirectoryDelete(absPath);
}

void FTPFileTree::ShowFileContextMenu(const QPoint &pos) {
    const auto *proxy = qobject_cast<QSortFilterProxyModel *>(_fileTreeView->model());
    const QModelIndex sourceIndex = proxy->mapToSource(_fileTreeView->currentIndex());
    if (!sourceIndex.isValid())
        return;

    const QString name = sourceIndex.siblingAtColumn(0).data().toString();
    const QString absPath = sourceIndex.siblingAtColumn(0).data(Qt::UserRole).toString();

    // ".." should never appear in the file view, but guard anyway
    if (name == QLatin1String(".."))
        return;

    QMenu menu;
    QAction *renameAction = menu.addAction(IconUtils::GetIcon("rename"), "Rename File");
    renameAction->setToolTip("Rename the file");
    menu.addSeparator();
    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete File");
    deleteAction->setToolTip("Delete the file");

    if (const auto selected = menu.exec(_fileTreeView->viewport()->mapToGlobal(pos)); selected == renameAction) {
        TargetTreeFileRename(absPath);
    } else if (selected == deleteAction) {
        _model->removeRow(sourceIndex.row(), sourceIndex.parent());
        _fileTreeView->clearSelection();
        TargetTreeFileDelete(absPath);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Static helpers
// ─────────────────────────────────────────────────────────────────────────────

QString FTPFileTree::FtpParentPath(const QString &path) {
    if (path == QLatin1String("/") || path.isEmpty())
        return {};
    const int slash = path.lastIndexOf('/');
    if (slash <= 0)
        return QStringLiteral("/");
    return path.left(slash);
}

QIcon FTPFileTree::GetIcon(const QString & /*mimeType*/, const QString &fileType) {
    if (fileType == QLatin1String(FTP_FILE_TYPE_FOLDER))
        return IconUtils::GetIcon(QString(FTP_FILE_TYPE_FOLDER));
    return QIcon::fromTheme("text-x-generic");
}

void FTPFileTree::HideColumns(const QVector<int> &columns) const {
    _folderTreeView->setHeaderHidden(true);
    for (const int col: columns)
        _folderTreeView->setColumnHidden(col, true);
}

void FTPFileTree::HideAllColumns() const {
    _folderTreeView->setHeaderHidden(true);
    for (int i = 0; i < _model->columnCount(); ++i)
        _folderTreeView->setColumnHidden(i, true);
}

bool FTPFileTree::HasChild(const QModelIndex &parent, const int column,
                           const QString &value, const QAbstractItemModel *model) {
    for (int row = 0; row < model->rowCount(parent); ++row) {
        if (model->index(row, column, parent).data().toString() == value)
            return true;
    }
    return false;
}

void FTPFileTree::SetFileHeaders(const QTreeView *treeView) {
    treeView->header()->setStretchLastSection(false);
    treeView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    treeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    treeView->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    treeView->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    treeView->header()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    treeView->header()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    treeView->header()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
}
