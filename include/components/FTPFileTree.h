//
// Created by vogje01 on 2/10/26.
//

#pragma once

// Qt includes
#include <QHeaderView>
#include <QInputDialog>
#include <QMenu>
#include <QMimeDatabase>
#include <QPushButton>
#include <QShortcut>
#include <QStandardItemModel>
#include <QVBoxLayout>

// Awsmock includes
#include <components/FileFilterModel.h>
#include <components/FolderFilterModel.h>
#include <modules/ftpclient/FTPClientThread.h>
#include <utils/DroppableTreeView.h>
#include <utils/IconUtils.h>

/**
 * @brief Split FTP filesystem browser.
 *
 * Top panel  – expandable directory tree (folders + ".." navigation item).
 *              Clicking ".." navigates up one level in the already-loaded
 *              model tree without issuing a new FTP command.
 *              Clicking a real folder issues an FTP TCd + listing.
 *
 * Bottom panel – flat file list (files only) for the currently-selected
 *                directory.
 */
class FTPFileTree : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param parent parent widget
     */
    explicit FTPFileTree(QWidget *parent = nullptr);

    /** @brief Destructor */
    ~FTPFileTree() override;

    void Connect(const QString &server, const QString &port,
                 const QString &user, const QString &password);

    void Disconnect();

signals:
    void ConnectionSucceeded();

private:
    // ── Setup ─────────────────────────────────────────────────────────────

    void SetupModel();

    void SetupFtpConnection();

    // ── Item management ───────────────────────────────────────────────────

    /**
     * @brief Called by the FTP thread for each item in a directory listing.
     *
     * On the first call for any non-root @p parent a ".." row is prepended
     * automatically so the user can navigate back to the parent directory.
     */
    void AddItem(const FileInfo &fileInfo, QStandardItem *parent) const;

    // ── FTP actions ───────────────────────────────────────────────────────

    void TargetFolderSelectionChanged(const QString &absPath, QStandardItem *parent) const;

    void TargetTreeFileDropped(const QString &filePath) const;

    void TargetTreeFileDelete(const QString &filePath) const;

    void TargetTreeFileRename(const QString &filePath);

    void TargetTreeAddDirectory();

    void TargetTreeDirectoryDelete(const QString &filePath) const;

    void TargetTreeDirectoryRename(const QString &filePath);

    void RefreshFileView() const;

    // ── Context menus ─────────────────────────────────────────────────────

    void ShowFileContextMenu(const QPoint &pos);

    void ShowFolderContextMenu(const QPoint &pos);

    // ── Utilities ─────────────────────────────────────────────────────────

    void HideColumns(const QVector<int> &columns) const;

    void HideAllColumns() const;

    static void SetFileHeaders(const QTreeView *treeView);

    static QIcon GetIcon(const QString &mimeType, const QString &fileType);

    static bool HasChild(const QModelIndex &parent, int column,
                         const QString &value, const QAbstractItemModel *model);

    /**
     * @brief Returns the parent of an FTP path.
     *        e.g. "/home/user" → "/home",  "/" → ""
     */
    static QString FtpParentPath(const QString &path);

    // ── Data members ──────────────────────────────────────────────────────

    /** @brief Shared item model: source of truth for both views. */
    QStandardItemModel *_model{};

    /** @brief Invisible root item; its path data is "/". */
    QStandardItem *_rootItem{};

    /** @brief FTP worker thread. */
    FTPClientThread *_ftpClientThread{};

    QMimeDatabase _mimeDb;

    /** @brief Top panel: expandable folder tree (FolderFilterModel proxy). */
    QTreeView *_folderTreeView{};

    /** @brief Bottom panel: flat file list (FileFilterModel proxy). */
    DroppableTreeView *_fileTreeView{};

    /** @brief Proxy model: passes FTP_FILE_TYPE_FOLDER and FTP_FILE_TYPE_PARENT. */
    FolderFilterModel *_folderProxyModel{};

    /** @brief Proxy model: passes FTP_FILE_TYPE_FILE only. */
    FileFilterModel *_fileProxyModel{};

    QVBoxLayout *_layout{};
    QHBoxLayout *_menuBarLayout{};

    bool _connected = false;
};
