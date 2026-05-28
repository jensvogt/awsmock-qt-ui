//
// Created by vogje01 on 2/11/26.
//

#pragma once

// Qt includes
#include <QSortFilterProxyModel>

#include "FolderFilterModel.h"

#define FTP_FILE_TYPE_FILE "file"

/**
 * @brief Proxy model for the FTP file panel (bottom panel in FTPFileTree).
 *
 * Accepts file and folder rows; rejects only the virtual ".." parent items.
 * This keeps folder items in the proxy so that setRootIndex() receives a valid
 * proxy index when the user selects a folder in the folder tree.
 */
class FileFilterModel : public QSortFilterProxyModel {
protected:
    [[nodiscard]] bool filterAcceptsRow(const int sourceRow, const QModelIndex &sourceParent) const override {
        const QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
        const QString type = index.data(Qt::UserRole + 1).toString();
        return type == FTP_FILE_TYPE_FILE || type == FTP_FILE_TYPE_FOLDER;
    }
};
