//
// Created by vogje01 on 2/15/26.
//

#ifndef AWSMOCK_QT_UI_PAGEABLE_TABLE_H
#define AWSMOCK_QT_UI_PAGEABLE_TABLE_H

// Qt includes
#include <QWidget>
#include <QStandardItemModel>

// Awsmock includes
#include <qheaderview.h>
#include <QList>
#include <utils/Configuration.h>
#include <utils/PrefixFilterModel.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class PageableTable;
}

QT_END_NAMESPACE

class PageableTable : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param parent parent widget
     */
    explicit PageableTable(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~PageableTable() override;

    /**
     * @brief Return the page index
     *
     * @return current page index
     */
    [[nodiscard]] long GetPageIndex() const {
        return _pageIndex;
    }

    /**
     * @brief Return the page index
     *
     * @return current page size
     */
    [[nodiscard]] long GetPageSize() const {
        return _pageSize;
    }

    /**
     * @brief Set the header names
     *
     * @param headerNames list of header names
     */
    void SetHeaderNames(const QStringList &headerNames);

    /**
     * @brief Sets the column resize mode
     *
     * @param resizeModes column resize mode list
     */
    void SetResizeModes(const QList<QHeaderView::ResizeMode> &resizeModes) const;

    /**
     * @brief Sets the total size
     *
     * @param totalSize total item count
     */
    void SetTotalSize(const long totalSize) {
        _totalSize = totalSize;
        _maxPage = (_totalSize + _pageSize - 1) / _pageSize;
    }

    /**
     * @brief Sets the total size
     *
     * @return total item count
     */
    [[nodiscard]] long GetTotalSize() const {
        return _totalSize;
    }

    /**
     * @brief Returns the sort column
     *
     * @return sort column name
     */
    [[nodiscard]] QString GetSortColumn() const {
        return _sortColumn;
    }

    /**
     * @brief Returns the sort column
     *
     * @param sortColumn column name
     */
    void SetSortColumn(const QString &sortColumn) {
        _sortColumn = sortColumn;
    }

    /**
     * @brief Returns the sort direction as integer
     *
     * @return sort direction, 1 = ascending, -1 = descending
     */
    [[nodiscard]] int GetSortDirection() const {
        return _sortDirection;
    }

    /**
     * @brief Returns the sort direction
     *
     * @param sortDirection sort direction
     */
    void SetSortDirection(const int sortDirection) {
        _sortDirection = sortDirection;
    }

signals:
    /**
     * @brief Send when the page size / page index changed
     *
     * @param pageIndex page index
     * @param pageSize page size
     */
    void pageChanged(long pageIndex, long pageSize);

private:
    /**
     * @brief Calculate the pageing status
     */
    void CalculatePageStatus() const;

    /**
     * @brief Page index
     */
    Ui::PageableTable *_ui;

    /**
     * @brief Page index
     */
    long _pageIndex{};

    /**
     * @brief Page index
     */
    long _pageSize{};

    /**
     * @brief Page index
     */
    long _maxPage{};

    /**
     * @brief Total size
     */
    long _totalSize{};

    /**
     * @brief Column header names
     */
    QStringList _headerNames;

    /**
     * @brief Data model
     */
    QStandardItemModel *_dataModel;

    /**
     * @brief Prefix table model
     */
    PrefixFilterProxyModel *_proxyModel;

    /**
     * @brief Sort column
     */
    QString _sortColumn;

    /**
     * @brief Sort column
     */
    int _sortDirection = 1;
};


#endif // AWSMOCK_QT_UI_PAGEABLE_TABLE_H
