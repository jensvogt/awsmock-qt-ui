//
// Created by vogje01 on 2/11/26.
//

#ifndef AWSMOCK_QT_UI_FTP_FILE_FILTER_MODEL_H
#define AWSMOCK_QT_UI_FTP_FILE_FILTER_MODEL_H

// Qt includes
#include <iostream>
#include <QSortFilterProxyModel>

#include "FolderFilterModel.h"

#define FTP_FILE_TYPE_FILE "file"

/**
 * @brief Special filter model which filters all rows with fileType = 'folder';
 */
class FileFilterModel : public QSortFilterProxyModel {

protected:
    [[nodiscard]] bool filterAcceptsRow(const int sourceRow, const QModelIndex &sourceParent) const override {
        const QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
        const QString type = index.data(Qt::UserRole + 1).toString();
        if (type == FTP_FILE_TYPE_FOLDER) {
            return true;
        }
        return type == FTP_FILE_TYPE_FILE;
    }
};

#endif // AWSMOCK_QT_UI_FTP_FILE_FILTER_MODEL_H
