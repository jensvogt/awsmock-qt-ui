//
// Created by vogje01 on 2/10/26.
//

#pragma once

// Qt includes
#include <QDir>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMenu>
#include <QPushButton>
#include <QPushButton>
#include <QStandardItemModel>
#include <QTreeView>
#include <QVBoxLayout>

// Awsmock includes
#include <modules/ftpclient/FTPClientThread.h>
#include <utils/DroppableTreeView.h>
#include <utils/IconUtils.h>

/**
 * @brief Split local-filesystem browser.
 *
 * Top panel  – expandable directory tree (folders only).
 * Bottom panel – flat file list showing only the files that live directly
 *                inside the currently-selected directory.
 *
 * Top folder tree uses QStandardItemModel so a ".." entry can be prepended.
 * Sub-directories are lazy-loaded when expanded.
 * The bottom file panel uses QFileSystemModel (files only).
 */
class LocalFileTree : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param rootFolder root directory shown at the top of the folder tree
     * @param parent     parent widget
     */
    explicit LocalFileTree(const QString &rootFolder, QWidget *parent);

    /**
     * @brief Destructor
     */
    ~LocalFileTree() override;

    /**
     * @brief Change the root directory displayed by both panels.
     */
    void SetBaseDir(const QString &baseDir);

    /**
     * @brief Collapse the folder tree and reset both panels to the root.
     */
    void Clear() const;

    /**
     * @brief Public alias for the bottom file-panel widget.
     *        Kept for drag-and-drop API compatibility with FTPFileTree.
     */
    DroppableTreeView *_fileTreeView{};

signals:
    void FolderSelectedSignal(const QString &folderPath);

    void TargetTreeFileRenameSignal(const QString &filePath);

    void TargetTreeFileDeleteSignal(const QString &filePath);

    void TargetTreeDirectoryRename(const QString &dirPath);

    void TargetTreeDirectoryDelete(const QString &dirPath);

    void TargetTreeDirectoryReload();

private slots:
    void OnFolderClicked(const QModelIndex &index);

    void OnFolderExpanded(const QModelIndex &index) const;

    void ShowFolderContextMenu(const QPoint &pos);
    
    void ShowFileContextMenu(const QPoint &pos);

    void RefreshView() const;

private:
    /**
     * @brief Fills @p parent with the immediate sub-directories of @p path.
     *
     * When @p includeParent is true a ".." row is prepended so the user can
     * navigate to the parent directory.  Sub-directories that contain children
     * receive a placeholder child so the expand arrow is rendered.
     */
    static void PopulateFolderNode(const QString &path, QStandardItem *parent, bool includeParent);

    /**
     * @brief QStandardItemModel for the top panel – ".." + directories, lazy-loaded.
     */
    QStandardItemModel *_folderModel{};

    /**
     * @brief Model for the bottom panel – files only.
     */
    QFileSystemModel *_fileModel{};

    /**
     * @brief Top panel: collapsible/expandable directory tree.
     */
    QTreeView *_folderTreeView{};

    /**
     * @brief Bottom panel: flat file list (also the public _fileTreeView alias).
     */
    DroppableTreeView *_fileView{};

    /**
     * @brief Root path set at construction (or via SetBaseDir).
     */
    QString _rootFolder;

    /**
     * @brief Path of the directory currently selected in the folder tree.
     */
    QString _currentFolder;

    QVBoxLayout *_layout{};
    QHBoxLayout *_menuBarLayout{};

    FTPClientThread *_ftpClientThread{};
    bool _connected = false;

    // Data roles used by the folder-tree items
    static constexpr int PATH_ROLE = Qt::UserRole; ///< absolute path
    static constexpr int TYPE_ROLE = Qt::UserRole + 1; ///< "parent" | "dir"

    static constexpr auto TYPE_PARENT = "parent"; ///< ".." item marker
    static constexpr auto TYPE_DIR = "dir"; ///< real directory marker
    static constexpr auto PLACEHOLDER = "__ph__"; ///< dummy child triggering the expand arrow
};

