//
// Created by vogje01 on 2/10/26.
//

#ifndef AWSMOCK_QT_UI_COMPONENTS_LOCAL_FILE_TREE_H
#define AWSMOCK_QT_UI_COMPONENTS_LOCAL_FILE_TREE_H

// Qt includes
#include <QMenu>
#include <QTreeView>
#include <QFileSystemModel>
#include <QStandardItemModel>
#include <QMimeDatabase>
#include <QVBoxLayout>
#include <QHeaderView>

// Awsmock includes
#include <utils/DroppableTreeView.h>
#include <utils/IconUtils.h>

#include <components/FileFilterModel.h>
#include <components/FolderFilterModel.h>
#include <modules/ftpclient/FTPLowLevelClient.h>
#include <components/FileFilterModel.h>
#include <modules/ftpclient/FTPLowLevelClient.h>

class LocalFileTree : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param parent parent widget
     */
    explicit LocalFileTree(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~LocalFileTree() override;

    void AddItem(const FileInfo &fileInfo, QStandardItem *parent) const;

    /**
     * @brief Returns the root item
     *
     * @return root item
     */
    [[nodiscard]] QStandardItem *GetRootItem() const {
        return _rootItem;
    };

    static QIcon GetIcon(const QString &mimeType, const QString &fileType);

    void Clear() const;

    void HideColumns(const QVector<int> &columns) const;

    void HideAllColumns() const;

    static bool HasChild(const QModelIndex &parent, int column, const QString &value, const QAbstractItemModel *model);

    void ShowFileContextMenu(const QPoint &pos);

    void ShowFolderContextMenu(const QPoint &pos);

    /**
     * TODO: Check inheritance
     * @brief Droppable file tree view
     */
    DroppableTreeView *_fileTreeView;

signals:
    void FolderSelectedSignal(const QString &filePath, QStandardItem *parent);

    void TargetTreeFileRenameSignal(const QString &filePath);

    void TargetTreeFileDeleteSignal(const QString &filePath);

    void TargetTreeDirectoryRename(const QString &filePath);

    void TargetTreeDirectoryDelete(const QString &filePath);

private:
    /**
     * @brief Item model
     */
    QStandardItemModel *_model;

    /**
     * @brief Root item (invisible)
     */
    QStandardItem *_rootItem;

    /**
     * @brief Mime type
     */
    QMimeDatabase _mimeDb;

    /**
     * @brief Droppable file tree view
     */
    QTreeView *_folderTreeView;

    /**
     * @brief Filter file type proxy model
     */
    FolderFilterModel *_folderProxyModel;

    /**
     * @brief Filter file type proxy model
     */
    FileFilterModel *_fileProxyModel;

    /**
     * @brief Base layout
     */
    QLayout *_layout;
};

#endif // AWSMOCK_QT_UI_COMPONENTS_LOCAL_FILE_TREE_H#1#
