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

    void CalculatePageStatus() const;

    void SetStatus(const QString &message) const;

    void SetLastUpdate() const;

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
    void ContextMenuRequested();

private:
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
     * @brief Data model
     */
    QStandardItemModel *_dataModel;

    /**
     * @brief Prefix table model
     */
    PrefixFilterProxyModel *_proxyModel;
};


#endif // AWSMOCK_QT_UI_PAGEABLE_LIST_H
