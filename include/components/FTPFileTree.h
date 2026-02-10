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
     * @param parent parent widget
     */
    explicit FTPFileTree(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~FTPFileTree() override;

    /**
     * @brief Returns the root item
     *
     * @return root item
     */
    [[nodiscard]] QStandardItem *GetRootItem() const {
        return _rootItem;
    };

    /**
     * @brief Adds a folder item to the window.
     *
     * @param name folder name
     */
    void AddFolder(const QString &name) const;

    /**
     * @brief Add a file item to the tree
     *
     * @param name file name
     * @param fileSize  file size
     * @param modifiedDate modified timestamp
     * @param permissions file permissions
     * @param user owner
     * @param group group
     */
    void AddFile(const QString &name, long fileSize, const QString &modifiedDate, const QString &permissions, const QString &user, const QString &group) const;

    static QIcon GetIcon(const QString &mimeType);

    void Clear() const;

    void HideColumns(const QVector<int> &columns) const;

    void HideAllColumns() const;

signals:
    void FolderSelectedSignal(const QString &filePath);

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
     * @brief Target (FTP) tree view
     */
    QStandardItem *_currentParent;

    /**
     * @brief Mime type
     */
    QMimeDatabase _mimeDb;

    DroppableTreeView *_fileTreeView;

    QLayout *_layout;
};

#endif // AWSMOCK_QT_UI_COMPONENTS_FTP_FILE_TREE_H#1#
