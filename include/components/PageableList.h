//
// Created by vogje01 on 2/15/26.
//

#ifndef AWSMOCK_QT_UI_PAGEABLE_LIST_H
#define AWSMOCK_QT_UI_PAGEABLE_LIST_H

// Qt includes
#include <QWidget>
#include <QStandardItemModel>

// Awsmock includes
#include <utils/IconUtils.h>
#include <utils/DateTimeUtils.h>
#include <utils/Configuration.h>
#include <utils/PrefixFilterModel.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class PageableList;
}

QT_END_NAMESPACE

class PageableList : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param parent parent widget
     */
    explicit PageableList(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~PageableList() override;

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
     * @brief Sets the total size
     *
     * @return total item count
     */
    [[nodiscard]] QString GetPrefix() const {
        return _prefix;
    }

    /**
      * @brief Returns the table index
      *
      * @param pos mouse position
      * @return table row/column index
      */
    QModelIndex GetIndexFromPosition(const QPoint &pos) const;

    /**
     * @brief Returns the global position
     *
     * @param tablePosition table position
     * @return global position
     */
    QPoint GetGlobalPosition(const QPoint &tablePosition) const;

    /**
     * @brief Clear all rows
     */
    void Clear() const {
        _dataModel->removeRows(0, _dataModel->rowCount());
    }

    /**
     * @brief Append a row
     *
     * @param item standard item
     */
    void Append(QStandardItem *item) const {
        _dataModel->appendRow(item);
    }

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

signals:
    /**
     * @brief Send when the page size / page index changed
     *
     * @param pageIndex page index
     * @param pageSize page size
     */
    void PageChanged(long pageIndex, long pageSize);

    /**
     * @brief Send when a context menu is requested
     */
    void ContextMenuRequested(const QPoint &pos);

private:
    void CalculatePageStatus() const;

    void SetStatus(const QString &message) const;

    void SetLastUpdate() const;

    /**
     * UI Components
     */
    Ui::PageableList *_ui;

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
     * @brief Prefix
     */
    QString _prefix{};

    /**
     * @brief Data model
     */
    QStandardItemModel *_dataModel;

    /**
     * @brief Prefix table model
     */
    PrefixFilterProxyModel *_proxyModel;
};


#endif // AWSMOCK_QT_UI_PAGEABLE_LIST_H
