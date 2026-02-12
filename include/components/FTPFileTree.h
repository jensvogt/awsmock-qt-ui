//
// Created by vogje01 on 2/10/26.
//

#ifndef AWSMOCK_QT_UI_COMPONENTS_FTP_FILE_TREE_H
#define AWSMOCK_QT_UI_COMPONENTS_FTP_FILE_TREE_H

// Qt includes
#include <QTreeView>
#include <QFileSystemModel>
#include <QStandardItemModel>
#include <QMimeDatabase>

// Awsmock includes
#include <utils/DroppableTreeView.h>
#include <utils/IconUtils.h>

#include <components/FTPFileFilterModel.h>
#include <components/FTPFolderFilterModel.h>
#include <modules/ftpclient/FTPLowLevelClient.h>
#include <components/FTPFileFilterModel.h>
#include <modules/ftpclient/FTPLowLevelClient.h>

#define FTP_FILE_TYPE_FOLDER "folder"
#define FTP_FILE_TYPE_FILE "file"

QT_BEGIN_NAMESPACE

namespace Ui {
    class FTPFileTree;
}

QT_END_NAMESPACE

class FTPFileTree : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param root
     * @param parent parent widget
     */
    explicit FTPFileTree(QStandardItem *root, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~FTPFileTree() override;

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

signals:
    void FolderSelectedSignal(const QString &filePath, QStandardItem *parent);

private:
    /**
     *  @brief UI components
     */
    Ui::FTPFileTree *_ui;

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
    DroppableTreeView *_folderTreeView;

    /**
     * @brief Droppable file tree view
     */
    DroppableTreeView *_fileTreeView;

    /**
     * @brief Filter file type proxy model
     */
    FTPFolderFilterModel *_folderProxyModel;

    /**
     * @brief Filter file type proxy model
     */
    FTPFileFilterModel *_fileProxyModel;

    /**
     * @brief Base layout
     */
    QLayout *_layout;
};

#endif // AWSMOCK_QT_UI_COMPONENTS_FTP_FILE_TREE_H#1#
