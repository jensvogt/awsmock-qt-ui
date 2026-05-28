//
// Created by vogje01 on 2/11/26.
//

#pragma once

// Qt includes
#include <QSortFilterProxyModel>

#define FTP_FILE_TYPE_FOLDER "folder"
#define FTP_FILE_TYPE_PARENT "parent"  ///< virtual ".." navigation item

/**
 * @brief Proxy model that passes only directory rows and the virtual ".." row.
 *        Used by the folder tree panel in FTPFileTree.
 */
class FolderFilterModel : public QSortFilterProxyModel {

protected:
    [[nodiscard]] bool filterAcceptsRow(const int sourceRow, const QModelIndex &sourceParent) const override {
        const QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
        const QString type = index.data(Qt::UserRole + 1).toString();
        return type == FTP_FILE_TYPE_FOLDER || type == FTP_FILE_TYPE_PARENT;
    }
};
