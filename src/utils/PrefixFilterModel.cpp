//
// Created by vogje01 on 12/10/25.
//

#include <utils/PrefixFilterModel.h>

PrefixFilterProxyModel::PrefixFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent) {
}

bool PrefixFilterProxyModel::filterAcceptsRow(const int sourceRow, const QModelIndex &sourceParent) const {

    // Get the model index for the cell in the specified column and current row.
    const QModelIndex index = sourceModel()->index(sourceRow, m_filterColumn, sourceParent);

    if (const QString cellText = sourceModel()->data(index).toString(); !cellText.isEmpty() && cellText.startsWith(m_filterPrefix)) {
        return true; // HIDE the row
    }

    return false; // SHOW the row
}
