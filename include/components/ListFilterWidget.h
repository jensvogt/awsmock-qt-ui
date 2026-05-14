//
// Created by vogje01 on 5/12/26.
//

#ifndef AWSMOCK_QT_UI_LIST_FILTER_WIDGET_H
#define AWSMOCK_QT_UI_LIST_FILTER_WIDGET_H

// Qt includes
#include <QSortFilterProxyModel>
#include <QWidget>

// Awsmock includes
#include <utils/IconUtils.h>

namespace Awsmock::Components {

    QT_BEGIN_NAMESPACE

    namespace Ui {
        class ListFilterWidget;
    }

    QT_END_NAMESPACE

    enum FilterType {
        LIST_FILTER_CONTAINS,
        LIST_FILTER_PREFIX,
        LIST_FILTER_POSTFIX,
        LIST_FILTER_REGULAR_EXPRESSION
    };

    static QMap<FilterType, QString> FilterTypeNames{
        {LIST_FILTER_CONTAINS, "Contains"},
        {LIST_FILTER_PREFIX, "Prefix"},
        {LIST_FILTER_POSTFIX, "Postfix"},
        {LIST_FILTER_REGULAR_EXPRESSION, "Regular Expression"},
    };

    [[maybe_unused]]
    static QString FilterTypeToString(const FilterType &filterType) {
        return FilterTypeNames.value(filterType, "Contains");
    }

    [[maybe_unused]]
    static FilterType FilterTypeFromString(const QString &filterType) {
        return FilterTypeNames.key(filterType, LIST_FILTER_CONTAINS);
    }

    static QStringList FilterTypesToList() {
        return FilterTypeNames.values();
    }

    /**
     * @brief List filter widget
     *
     * @par
     * Provides a filter edit box, a filter type combo box and a clear button, to support QListView filtering. The Filter types are 'Contains',
     * 'Prefix', 'Postfix' and 'Regular Expression'.
     */
    class ListFilterWidget : public QWidget {
        Q_OBJECT

    public:
        /**
         * @brief Constructor
         *
         * @param parent parent widget
         */
        explicit ListFilterWidget(QWidget *parent = nullptr);

        /**
         * @brief Destructor
         */
        ~ListFilterWidget() override;

        void ApplyFilter(const QString &text) const;

        /**
         * @brief Set filter proxy model
         *
         * @param proxyModel filter proxy model
         */
        void SetFilterProxyModel(QSortFilterProxyModel *proxyModel) {
            _proxyModel = proxyModel;
        }

        /**
         * @brief Clear
         */
        void ClearFilter();

    signals:
        /**
         * @brief Sent when the filter edit is cleared
         */
        void FilterCleared();

    private:
        /**
         * @brief UI components
         */
        Ui::ListFilterWidget *_ui;

        /**
         * @brief current filter type
         */
        FilterType _currentFilter;

        /**
         * @brief QListView proxy model
         */
        QSortFilterProxyModel *_proxyModel{};
    };
} // Awsmock::Components

#endif // AWSMOCK_QT_UI_LIST_FILTER_WIDGET_H
