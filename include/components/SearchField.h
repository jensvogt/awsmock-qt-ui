//
// Created by jensv on 24/02/2026.
//

#ifndef AWSMOCK_QT_UI_SEARCH_FIELD_H
#define AWSMOCK_QT_UI_SEARCH_FIELD_H

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

    signals :
        /**
         * @brief Signaled when the search text should be searched in the forward direction
         *
         * @param searchText changed text
         */
        void SigSearchForward(const QString &searchText);

        /**
         * @brief Signaled when the search text should be searched in the backward direction
         *
         * @param searchText changed text
         */
        void SigSearchBackward(const QString &searchText);

        /**
         * @brief Signaled when the search has been box closed
         */
        void SigClose();

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

#endif //AWSMOCK_QT_UI_SEARCH_FIELD_H
