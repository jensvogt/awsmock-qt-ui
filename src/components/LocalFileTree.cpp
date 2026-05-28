//
// Created by vogje01 on 2/10/26.
//

#include <components/LocalFileTree.h>

// ─────────────────────────────────────────────────────────────────────────────
// Construction / destruction
// ─────────────────────────────────────────────────────────────────────────────

LocalFileTree::LocalFileTree(const QString &rootFolder, QWidget *parent)
    : QWidget(parent), _rootFolder(rootFolder), _currentFolder(rootFolder) {

    _layout = new QVBoxLayout(this);
    _layout->setContentsMargins(0, 0, 0, 0);
    _layout->setSpacing(2);
    setLayout(_layout);

    // ── Folder model ──────────────────────────────────────────────────────
    // QStandardItemModel lets us prepend the ".." row and lazy-load children.
    _folderModel = new QStandardItemModel(this);

    // ── Folder tree (top panel) ───────────────────────────────────────────
    _folderTreeView = new QTreeView(this);
    _folderTreeView->setModel(_folderModel);
    _folderTreeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _folderTreeView->setHeaderHidden(true);
    _folderTreeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _folderTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _folderTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
    _folderTreeView->setUniformRowHeights(true);
    // Sorting disabled: ".." must always stay at row 0;
    // PopulateFolderNode sorts entries itself via QDir.
    _folderTreeView->setSortingEnabled(false);
    _folderTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(_folderTreeView, &QTreeView::clicked,
            this, &LocalFileTree::OnFolderClicked);
    connect(_folderTreeView, &QTreeView::expanded,
            this, &LocalFileTree::OnFolderExpanded);
    connect(_folderTreeView, &QTreeView::customContextMenuRequested,
            this, &LocalFileTree::ShowFolderContextMenu);

    // ── File model – files only ───────────────────────────────────────────
    _fileModel = new QFileSystemModel(this);
    _fileModel->setFilter(QDir::Files | QDir::NoDotAndDotDot);
    _fileModel->setRootPath(rootFolder);

    // ── File view (bottom panel) ──────────────────────────────────────────
    _fileView = new DroppableTreeView(this);
    _fileView->setModel(_fileModel);
    _fileView->setRootIndex(_fileModel->index(rootFolder));
    _fileView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    _fileView->header()->setStretchLastSection(false);
    _fileView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    _fileView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    _fileView->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    _fileView->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    _fileView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _fileView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _fileView->setSelectionMode(QAbstractItemView::SingleSelection);
    _fileView->setUniformRowHeights(true);
    _fileView->setRootIsDecorated(false); // flat list – no expand arrows
    _fileView->setSortingEnabled(true);
    _fileView->sortByColumn(0, Qt::AscendingOrder);

    // Drag & Drop
    _fileView->setAcceptDrops(true);
    _fileView->setDropIndicatorShown(true);
    _fileView->setDragEnabled(true);
    _fileView->setDragDropMode(QAbstractItemView::DragDrop);

    _fileView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_fileView, &QTreeView::customContextMenuRequested,
            this, &LocalFileTree::ShowFileContextMenu);

    // Forward drag-and-drop upload
    connect(_fileView, &DroppableTreeView::FileDropped, this, [this](const QString &filePath) {
        if (_connected && _ftpClientThread) {
            _ftpClientThread->task = TUp;
            _ftpClientThread->arglist[0] = filePath.toStdString();
            _ftpClientThread->start();
        }
    });

    // Keep public alias pointing at the file panel
    _fileTreeView = _fileView;

    // ── Toolbar ───────────────────────────────────────────────────────────
    _menuBarLayout = new QHBoxLayout();
    _menuBarLayout->setContentsMargins(0, 0, 0, 0);

    auto *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    _menuBarLayout->addWidget(spacer);

    auto *refreshButton = new QPushButton(IconUtils::GetIcon("refresh"), nullptr);
    refreshButton->setToolTip("Refresh");
    connect(refreshButton, &QPushButton::clicked, this, &LocalFileTree::RefreshView);
    _menuBarLayout->addWidget(refreshButton);

    // ── Assemble ──────────────────────────────────────────────────────────
    _layout->addWidget(_folderTreeView);
    _layout->addLayout(_menuBarLayout);
    _layout->addWidget(_fileView);

    // Initial population
    PopulateFolderNode(rootFolder, _folderModel->invisibleRootItem(), /*includeParent=*/true);
}

LocalFileTree::~LocalFileTree() = default;

// ─────────────────────────────────────────────────────────────────────────────
// Public API
// ─────────────────────────────────────────────────────────────────────────────

void LocalFileTree::SetBaseDir(const QString &baseDir) {
    _rootFolder = baseDir;
    _currentFolder = baseDir;

    // Rebuild the folder tree from the new root
    PopulateFolderNode(baseDir, _folderModel->invisibleRootItem(), /*includeParent=*/true);

    // Point the file panel at the new root as well
    _fileModel->setRootPath(baseDir);
    _fileView->setRootIndex(_fileModel->index(baseDir));
}

void LocalFileTree::Clear() const {
    _folderTreeView->collapseAll();
    PopulateFolderNode(_rootFolder, _folderModel->invisibleRootItem(), /*includeParent=*/true);
    _fileView->setRootIndex(_fileModel->index(_rootFolder));
}

// ─────────────────────────────────────────────────────────────────────────────
// Private helpers
// ─────────────────────────────────────────────────────────────────────────────

void LocalFileTree::PopulateFolderNode(const QString &path, QStandardItem *parent, bool includeParent) {
    parent->removeRows(0, parent->rowCount());

    // ".." row – navigate to the parent directory
    if (includeParent) {
        const QString parentPath = QFileInfo(path).dir().absolutePath();
        if (parentPath != path) {
            // skip when already at the filesystem root
            auto *dotDot = new QStandardItem(IconUtils::GetIcon("folder"), "..");
            dotDot->setData(parentPath, PATH_ROLE);
            dotDot->setData(QString(TYPE_PARENT), TYPE_ROLE);
            parent->appendRow(dotDot);
        }
    }

    // Sub-directories, sorted by name (case-insensitive)
    QDir dir(path);
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden);
    dir.setSorting(QDir::Name | QDir::IgnoreCase);

    for (const QFileInfo &fi: dir.entryInfoList()) {
        auto *item = new QStandardItem(IconUtils::GetIcon("folder"), fi.fileName());
        item->setData(fi.absoluteFilePath(), PATH_ROLE);
        item->setData(QString(TYPE_DIR), TYPE_ROLE);

        // Add a placeholder child so the expand arrow is shown for non-empty dirs
        QDir sub(fi.absoluteFilePath());
        sub.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden);
        if (!sub.entryInfoList().isEmpty()) {
            item->appendRow(new QStandardItem(QString(PLACEHOLDER)));
        }

        parent->appendRow(item);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Private slots
// ─────────────────────────────────────────────────────────────────────────────

void LocalFileTree::OnFolderClicked(const QModelIndex &index) {
    if (!index.isValid())
        return;

    const QStandardItem *item = _folderModel->itemFromIndex(index);
    if (!item)
        return;

    const QString path = item->data(PATH_ROLE).toString();

    if (const QString type = item->data(TYPE_ROLE).toString(); type == QLatin1String(TYPE_PARENT)) {
        // ".." clicked – re-root the entire tree at the parent directory
        SetBaseDir(path);
        return;
    }

    // Regular directory: update the file panel
    _currentFolder = path;
    _fileModel->setRootPath(path);
    _fileView->setRootIndex(_fileModel->index(path));
    emit FolderSelectedSignal(path);
}

void LocalFileTree::OnFolderExpanded(const QModelIndex &index) const {
    QStandardItem *item = _folderModel->itemFromIndex(index);
    if (!item)
        return;

    // Only populate real directory nodes that still have the placeholder child
    if (item->data(TYPE_ROLE).toString() != QLatin1String(TYPE_DIR))
        return;

    if (item->rowCount() == 1 && item->child(0)->text() == QLatin1String(PLACEHOLDER)) {
        const QString path = item->data(PATH_ROLE).toString();
        PopulateFolderNode(path, item, /*includeParent=*/false);
    }
}

void LocalFileTree::RefreshView() const {
    // Rebuild the folder tree in-place
    PopulateFolderNode(_rootFolder, _folderModel->invisibleRootItem(), /*includeParent=*/true);

    // Force the file model to re-read the current directory
    _fileModel->setRootPath(QString());
    _fileModel->setRootPath(_currentFolder);
    _fileView->setRootIndex(_fileModel->index(_currentFolder));
}

void LocalFileTree::ShowFolderContextMenu(const QPoint &pos) {
    const QModelIndex index = _folderTreeView->indexAt(pos);
    if (!index.isValid())
        return;

    const QStandardItem *item = _folderModel->itemFromIndex(index);
    if (!item)
        return;

    // No context menu on the ".." entry
    if (item->data(TYPE_ROLE).toString() == QLatin1String(TYPE_PARENT))
        return;

    const QString path = item->data(PATH_ROLE).toString();

    QMenu menu;
    QAction *renameAction = menu.addAction(IconUtils::GetIcon("rename"), "Rename Directory");
    renameAction->setToolTip("Rename this directory");
    menu.addSeparator();
    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Directory");
    deleteAction->setToolTip("Delete this directory and all its contents");

    if (const QAction *selected = menu.exec(_folderTreeView->viewport()->mapToGlobal(pos));
        selected == renameAction) {
        emit TargetTreeDirectoryRename(path);
    } else if (selected == deleteAction) {
        emit TargetTreeDirectoryDelete(path);
    }
}

void LocalFileTree::ShowFileContextMenu(const QPoint &pos) {
    const QModelIndex index = _fileView->indexAt(pos);
    if (!index.isValid())
        return;

    const QString path = _fileModel->filePath(index.siblingAtColumn(0));

    QMenu menu;
    QAction *uploadAction = menu.addAction(IconUtils::GetIcon("upload"), "Upload file");
    uploadAction->setToolTip("Upload this file");
    QAction *renameAction = menu.addAction(IconUtils::GetIcon("rename"), "Rename File");
    renameAction->setToolTip("Rename this file");
    menu.addSeparator();
    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete File");
    deleteAction->setToolTip("Delete this file");

    if (const QAction *selected = menu.exec(_fileView->viewport()->mapToGlobal(pos));
        selected == renameAction) {
        emit TargetTreeFileRenameSignal(path);
    } else if (selected == uploadAction) {
        emit EventBus::instance().FtpUploadSignal(path);
    } else if (selected == deleteAction) {
        emit TargetTreeFileDeleteSignal(path);
    }
}
