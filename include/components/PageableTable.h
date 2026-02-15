//
// Created by vogje01 on 2/15/26.
//

#ifndef AWSMOCK_QT_UI_PAGEABLE_TABLE_H
#define AWSMOCK_QT_UI_PAGEABLE_TABLE_H

// Qt includes
#include <QList>
#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>

// Awsmock includes
#include <utils/IconUtils.h>
#include <utils/DateTimeUtils.h>
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
        CalculatePageStatus();
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

    /**
     * @brief Sets a string column
     *
     * @param row table row
     * @param column table column
     * @param value column value
     * @param alignment column aligment
     */
    void SetColumn(int row, int column, const QString &value, const Qt::Alignment &alignment = Qt::AlignLeft | Qt::AlignVCenter) const;

    /**
     * @brief Sets a datetime column
     *
     * @param row table row
     * @param column table column
     * @param value column value
     */
    void SetColumn(int row, int column, const QDateTime &value) const;

    /**
     * @brief Sets a long integer column
     *
     * @param row table row
     * @param column table column
     * @param value column value
     */
    void SetColumn(int row, int column, const long &value) const;

    template<class T>
    T GetValue(const QModelIndex &index, const int column) {
        QString sValue = _dataModel->item(index.row(), column)->text();
        if constexpr (std::is_same_v<T, int>) {
            return static_cast<T>(sValue.toInt());
        } else if constexpr (std::is_same_v<T, long>) {
            return static_cast<T>(sValue.toInt());
        } else if constexpr (std::is_same_v<T, double>) {
            return static_cast<T>(sValue.toDouble());
        } else if constexpr (std::is_same_v<T, QString>) {
            return static_cast<T>(sValue);
        } else {
            return {};
        }
    }

    /**
        * @brief Returns the table index
        *
        * @param pos mouse position
        * @return table row/column index
        */
    QModelIndex GetIndexFromPosition(const QPoint &pos) const;

    QPoint GetGlobalPosition(const QPoint &tablePosition) const;

signals:
    /**
     * @brief Send when the page size / page index changed
     *
     * @param pageIndex page index
     * @param pageSize page size
     */
    void PageChanged(long pageIndex, long pageSize);

    /**
     * @brief Send when the context menu is selected
     *
     * @param pos page index
     */
    void ContextMenuSelected(const QPoint &pos);

private:
    /**
     * @brief Sets a status message
     *
     * @param message status message
     */
    void SetStatus(const QString &message) const;

    /**
     * @brief Sets the last update time
     */
    void SetLastUpdate() const;

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
