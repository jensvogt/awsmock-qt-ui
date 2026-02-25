
#ifndef AWSMOCK_QT_UI_PLAIN_TEXT_EDITOR_H
#define AWSMOCK_QT_UI_PLAIN_TEXT_EDITOR_H

// Qt includes
#include <QString>
#include <QWidget>
#include <QPlainTextEdit>
#include <QVBoxLayout>

// Awsmock includes
#include <utils/StringUtils.h>
#include <components/ReplaceWidget.h>

#include "SearchField.h"

namespace Awsmock::Components {

    enum TextContentType {
        JSON,
        XML,
        ASCII
    };

    class PlainTextEditor : public QWidget {
        Q_OBJECT

    public:
        explicit PlainTextEditor(bool prettyPrint = true, const TextContentType &contentType = JSON, QWidget *parent = nullptr);

        /**
         * @bried Destructor
         */
        ~PlainTextEditor() override;

        /**
         * @brief Sets the text to display
         *
         * @param text text to edit
         */
        void SetText(const QString &text);

        /**
         * @brief Search for the next occurrence of the search text
         *
         * @param searchText text to search for
         */
        void SearchNext(const QString &searchText = nullptr);

        /**
         * @brief Search for the previous occurrence of the search text
         *
         * @param searchText text to search for
         */
        void SearchPrevious(const QString &searchText);

        void Replace(const QString &searchString, const QString &replaceText);

        void ReplaceAll(const QString &searchString, const QString &replaceText);

        /**
         * @brief Sets the text to display
         *
         * @return edited text
         */
        [[nodiscard]] QString GetText() const {
            return _plainTextEdit->toPlainText().toUtf8();
        }

        void SetPrettyPrint(const bool prettyPrint) {
            _prettyPrint = prettyPrint;
        }

    private:
        /**
         * @brief Initialize the components
         */
        void Initialize();

        /**
         * @brief Initialize the replace text widget
         */
        void InitializeReplaceText() const;

        /**
         * @brief Initialize the search text field
         */
        void InitializeSearchText() const;

        /**
         * @brief Count the number of occurrences of the search text
         *
         * @param searchText text to search
         * @return number of occurrences
         */
        int CountOccurrences(const QString &searchText);

        /**
         * @brief Text to edit
         */
        QString _text;

        /**
         * @brief Plain text edit widget
         */
        QPlainTextEdit *_plainTextEdit{};

        /**
         * @brief Replace widget
         */
        ReplaceWidget *_replaceWidget{};

        /**
         * @brief Search field widget
         */
        SearchField *_searchWidget{};

        /**
         * @brief Content type
         */
        TextContentType _contentType = JSON;

        /**
         * @brief Text cursor
         */
        QTextCursor _cursor{};

        /**
         * @brief The actual document
         */
        QTextDocument *_document{};

        /**
         * @brief Pretty print flag
         */
        bool _prettyPrint = true;

        /**
         * @brief Occurrence count
         */
        int _count = 0;

        /**
         * @brief Current cursor position
         */
        int _currentCursor = 1;
    };

}

#endif //AWSMOCK_QT_UI_PLAIN_TEXT_EDITOR_H
