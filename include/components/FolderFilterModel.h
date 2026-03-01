//
// Created by vogje01 on 2/11/26.
//

#ifndef AWSMOCK_QT_UI_FTP_FOLDER_FILTER_MODEL_H
#define AWSMOCK_QT_UI_FTP_FOLDER_FILTER_MODEL_H

// Qt includes
#include <iostream>
#include <QSortFilterProxyModel>

#define FTP_FILE_TYPE_FOLDER "folder"

/**
 * @brief Special filter model which filters all rows with fileType = 'folder';
 */
class FolderFilterModel : public QSortFilterProxyModel {

protected:
    [[nodiscard]] bool filterAcceptsRow(const int sourceRow, const QModelIndex &sourceParent) const override {
        const QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
        const QString type = index.data(Qt::UserRole + 1).toString();
        return type == FTP_FILE_TYPE_FOLDER;
    }
};

#endif // AWSMOCK_QT_UI_FTP_FOLDER_FILTER_MODEL_H
