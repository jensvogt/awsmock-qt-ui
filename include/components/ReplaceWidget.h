//
// Created by vogje01 on 2/22/26.
//

#pragma once

// Qt includes
#include <QWidget>

// Awsmock includes
#include <utils/IconUtils.h>

namespace Awsmock::Components {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class ReplaceWidget;
    }

    QT_END_NAMESPACE

    class ReplaceWidget : public QWidget {
        Q_OBJECT

    public:
        explicit ReplaceWidget(QWidget *parent = nullptr);

        /**
         * @brief Destructor
         */
        ~ReplaceWidget() override;

        /**
         * @brief Sets the cursor
         */
        void SetCursor(int current, int total) const;

    signals:
        /**
         * @brief Signaled, when the replace dialog closes
         */
        void SigClose();

        /**
         * @brief Signaled, when the search text changes
         *
         * @param searchText changed text
         */
        void SigSearch(const QString &searchText);

        /**
         * @brief Signaled, when the forward button is clicked
         *
         * @param searchText changed text
         */
        void SigForward(const QString &searchText);

        /**
         * @brief Signaled, when the backward button is clicked
         *
         * @param searchText changed text
         */
        void SigBackward(const QString &searchText);

        /**
         * @brief Signaled, when the replace button is clicked
         *
         * @param searchText current search text
         * @param replaceText replacement text
         */
        void SigReplace(const QString &searchText, const QString &replaceText);

        /**
         * @brief Signaled, when the replace all button is clicked
         *
         * @param searchText current search text
         * @param replaceText replacement text
         */
        void SigReplaceAll(const QString &searchText, const QString &replaceText);

    private:
        /**
         * @brief UI components
         */
        Ui::ReplaceWidget *_ui;

        /**
         * @brief Search text
         */
        QString _searchText;

        /**
         * @brief Replace text
         */
        QString _replaceText;
    };

} // Awsmock::Components

