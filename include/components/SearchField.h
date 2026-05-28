//
// Created by jensv on 24/02/2026.
//

#pragma once

// Qt includes
#include <QWidget>

// Awsmock includes
#include <utils/IconUtils.h>

namespace Awsmock::Components {

    QT_BEGIN_NAMESPACE

    namespace Ui {
        class SearchField;
    }

    QT_END_NAMESPACE

    enum SearchType {
        CONTAINS,
        STARTS_WITH,
        ENDS_WITH,
        REGEXP
    };

    static std::map<SearchType, QString> SearchTypeNames{
        {CONTAINS, "contains"},
        {STARTS_WITH, "startsWith"},
        {ENDS_WITH, "endWith"},
        {REGEXP, "regular expression"}
    };

    [[maybe_unused]] static QString SearchTypeToString(const SearchType &searchType) {
        return SearchTypeNames[searchType];
    }

    [[maybe_unused]] static SearchType SearchTypeFromString(const QString &searchType) {
        for (auto &[fst, snd]: SearchTypeNames) {
            if (snd == searchType) {
                return fst;
            }
        }
        return CONTAINS;
    }

    static QStringList GetSearchTypeNames() {
        QStringList keyStates;
        for (auto &snd: SearchTypeNames | std::views::values) {
            keyStates << snd;
        }
        return keyStates;
    }

    class SearchField : public QWidget {
        Q_OBJECT

    public:
        /**
         * @brief Constructor
         *
         * @param parent parent widget
         */
        explicit SearchField(QWidget *parent = nullptr);

        /**
         * @brief Destructor
         */
        ~SearchField() override;

        /**
         * @brief clear the search field
         */
        void ClearSearchField();

        void showEvent(QShowEvent *event) override;

    signals :
        /**
         * @brief Signaled when the search text should be searched in the forward direction
         *
         * @param searchText changed text
         * @param searchType search type
         */
        void SigSearchForward(const QString &searchText, const SearchType &searchType);

        /**
         * @brief Signaled when the search text should be searched in the backward direction
         *
         * @param searchText changed text
         * @param searchType search type
         */
        void SigSearchBackward(const QString &searchText, const SearchType &searchType);

        /**
         * @brief Signaled when the search has been box closed
         */
        void SigClose();

        /**
         * @brief Signaled when the search type has been changed
         */
        void SigSearchTypeChanged(const SearchType &searchType);

    private:
        /**
         * @brief UI components
         */
        Ui::SearchField *_ui;

        /**
         * @brief Search text
         */
        QString _searchText;

        /**
         * @brief Search type
         */
        QString _searchType;
    };

}

