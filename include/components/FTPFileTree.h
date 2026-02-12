//
// Created by vogje01 on 2/10/26.
//

#ifndef AWSMOCK_QT_UI_COMPONENTS_FTP_FILE_TREE_H
#define AWSMOCK_QT_UI_COMPONENTS_FTP_FILE_TREE_H

// Qt includes
#include <QMenu>
#include <QStandardItemModel>
#include <QMimeDatabase>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QShortcut>
#include <QInputDialog>
#include <QPushButton>

// Awsmock includes
#include <utils/DroppableTreeView.h>
#include <utils/IconUtils.h>

#include <components/FileFilterModel.h>
#include <components/FolderFilterModel.h>
#include <modules/ftpclient/FTPClientThread.h>

class FTPFileTree : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param parent parent widget
     */
    explicit FTPFileTree(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~FTPFileTree() override;

    void Connect(const QString &server, const QString &port, const QString &user, const QString &password);

    void Disconnect();

signals:
    void ConnectionSucceeded();

private:
    void SetupModel();

    void AddItem(const FileInfo &fileInfo, QStandardItem *parent) const;

    void TargetFolderSelectionChanged(const QString &absPath, QStandardItem *parent) const;

    void TargetTreeFileDropped(const QString &filePath) const;

    void TargetTreeFileDelete(const QString &filePath) const;

    void TargetTreeFileRename(const QString &filePath);

    void TargetTreeAddDirectory();

    void TargetTreeDirectoryDelete(const QString &filePath) const;

    void TargetTreeDirectoryRename(const QString &filePath);

    void RefreshFileView() const;

    /**
     * @brief Set up the FTP connection
     */
    void SetupFtpConnection();

    void HideColumns(const QVector<int> &columns) const;

    void HideAllColumns() const;

    void ShowFileContextMenu(const QPoint &pos);

    void ShowFolderContextMenu(const QPoint &pos);

    static void SetFileHeaders(const QTreeView *treeView);

    static QIcon GetIcon(const QString &mimeType, const QString &fileType);

    static bool HasChild(const QModelIndex &parent, int column, const QString &value, const QAbstractItemModel *model);

    /**
     * @brief Item model
     */
    QStandardItemModel *_model{};

    /**
     * @brief Root item (invisible)
     */
    QStandardItem *_rootItem{};

    /**
     * @brief FTP client thread
     */
    FTPClientThread *_ftpClientThread{};

    /**
     * @brief Mime type
     */
    QMimeDatabase _mimeDb;

    /**
     * @brief Droppable file tree view
     */
    QTreeView *_folderTreeView;

    /**
     * @brief Droppable file tree view
     */
    DroppableTreeView *_fileTreeView;

    /**
     * @brief Filter file type proxy model
     */
    FolderFilterModel *_folderProxyModel{};

    /**
     * @brief Filter file type proxy model
     */
    FileFilterModel *_fileProxyModel;

    /**
     * @brief Base layout
     */
    QVBoxLayout *_layout;

    /**
     * @brief File menu bar layout
     */
    QHBoxLayout *_menuBarLayout;

    /**
     * @brief connection flag
     */
    bool _connected = false;
};

#endif // AWSMOCK_QT_UI_COMPONENTS_FTP_FILE_TREE_H#1#
