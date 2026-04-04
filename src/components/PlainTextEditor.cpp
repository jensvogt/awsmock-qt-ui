//
// Created by vogje01 on 2/22/26.
//

#include <QMessageBox>
#include <components/PlainTextEditor.h>

#include "utils/StringUtils.h"

namespace Awsmock::Components {

    PlainTextEditor::PlainTextEditor(const bool prettyPrint, const TextContentType &contentType, QWidget *parent) : QWidget(parent), _contentType(contentType), _prettyPrint(prettyPrint) {

        Initialize();
    }

    PlainTextEditor::~PlainTextEditor() {
        delete _replaceWidget;
        delete _plainTextEdit;
    }

    void PlainTextEditor::Initialize() {

        // Setup replace widget
        _replaceWidget = new ReplaceWidget(this);
        _replaceWidget->setHidden(true);
        connect(_replaceWidget, &ReplaceWidget::SigClose, this, [this]() {
            _replaceWidget->setHidden(true);
            _currentCursor = 0;
            _count = 0;
            _replaceWidget->SetCursor(_currentCursor, _count);
        });
        connect(_replaceWidget, &ReplaceWidget::SigSearch, this, [this](const QString &searchText) {
            _count = CountOccurrences(searchText);
            SearchNext(searchText);
        });
        connect(_replaceWidget, &ReplaceWidget::SigForward, this, [this](const QString &searchText) {
            SearchNext(searchText);
        });
        connect(_replaceWidget, &ReplaceWidget::SigBackward, this, [this](const QString &searchText) {
            SearchPrevious(searchText);
        });
        connect(_replaceWidget, &ReplaceWidget::SigReplace, this, [this](const QString &searchText, const QString &replaceText) {
            Replace(searchText, replaceText);
        });
        connect(_replaceWidget, &ReplaceWidget::SigReplaceAll, this, [this](const QString &searchText, const QString &replaceText) {
            ReplaceAll(searchText, replaceText);
        });

        // Setup search widget
        _searchWidget = new SearchField(this);
        _searchWidget->setHidden(true);
        connect(_searchWidget, &SearchField::SigClose, this, [this]() {
            _searchWidget->setHidden(true);
        });
        connect(_searchWidget, &SearchField::SigSearchForward, this, [this](const QString &searchText, const SearchType &searchType) {
            SearchNext(searchText, searchType);
        });
        connect(_searchWidget, &SearchField::SigSearchBackward, this, [this](const QString &searchText, const SearchType &searchType) {
            SearchPrevious(searchText, searchType);
        });

        // Setup plain text edit
        _plainTextEdit = new QPlainTextEdit(this);

        // Add replace shortcut
        auto *replaceAction = new QAction(this);
        replaceAction->setShortcut(QKeySequence("Ctrl+R"));
        replaceAction->setShortcutContext(Qt::WidgetShortcut);
        _plainTextEdit->addAction(replaceAction);
        connect(replaceAction, &QAction::triggered, this, &PlainTextEditor::InitializeReplaceText);

        // Add search shortcut
        auto *searchAction = new QAction(this);
        searchAction->setShortcut(QKeySequence("Ctrl+F"));
        searchAction->setShortcutContext(Qt::WidgetShortcut);
        _plainTextEdit->addAction(searchAction);
        connect(searchAction, &QAction::triggered, this, &PlainTextEditor::InitializeSearchText);

        const auto layout = new QVBoxLayout(this);
        layout->addWidget(_searchWidget);
        layout->addWidget(_replaceWidget);
        layout->addWidget(_plainTextEdit);
    }

    void PlainTextEditor::InitializeReplaceText() const {
        _replaceWidget->setHidden(false);
    }

    void PlainTextEditor::InitializeSearchText() const {
        _searchWidget->setHidden(false);
    }

    void PlainTextEditor::SetText(const QString &text) {
        _text = text;

        if (_contentType == JSON) {
            if (_prettyPrint) {
                _text = StringUtils::ConvertToIndentedJson(_text);
            } else {
                _text = StringUtils::ConvertToCompactJson(_text);
            }
        }

        _plainTextEdit->setPlainText(_text);
        _document = _plainTextEdit->document();
        _cursor = QTextCursor(_document);
    }

    void PlainTextEditor::SetReadOnly(const bool readOnly) const {
        _plainTextEdit->setReadOnly(readOnly);
    }

    void PlainTextEditor::Search(const QString &searchText, const SearchType &searchType, const bool forward) {

        const QTextDocument::FindFlags flags = forward ? QTextDocument::FindFlags{} : QTextDocument::FindBackward;
        bool found = false;

        if (searchType == REGEXP) {
            found = _plainTextEdit->find(QRegularExpression(searchText), flags);
        } else {
            found = _plainTextEdit->find(QRegularExpression(GetPattern(searchText, searchType)), flags);
        }

        if (found) {
            forward ? _currentCursor++ : _currentCursor--;
            _replaceWidget->SetCursor(_currentCursor, _count);
            return;
        }

        // Wrap
        _cursor = _plainTextEdit->textCursor();
        _cursor.movePosition(forward ? QTextCursor::Start : QTextCursor::End);
        _plainTextEdit->setTextCursor(_cursor);

        // Retry after wrap
        if (searchType == REGEXP) {
            _plainTextEdit->find(QRegularExpression(searchText), flags);
        } else {
            _plainTextEdit->find(QRegularExpression(GetPattern(searchText, searchType)), flags);
        }
    }

    void PlainTextEditor::SearchNext(const QString &searchText, const SearchType &searchType) {
        Search(searchText, searchType, true);
    }

    void PlainTextEditor::SearchPrevious(const QString &searchText, const SearchType &searchType) {
        Search(searchText, searchType, false);
    }

    QString PlainTextEditor::GetPattern(const QString &searchText, const SearchType &searchType) {
        switch (searchType) {
            case CONTAINS:
                return QRegularExpression::escape(searchText);
            case STARTS_WITH:
                return "\\b" + QRegularExpression::escape(searchText);
            case ENDS_WITH:
                return QRegularExpression::escape(searchText) + "\\b";
            default:
                return QRegularExpression::escape(searchText);
        }
    }

    void PlainTextEditor::Replace(const QString &searchString, const QString &replaceText) {
        _cursor.beginEditBlock();
        _cursor = _document->find(searchString, _cursor);
        if (!_cursor.isNull() && !_cursor.atEnd()) {
            _cursor.insertText(replaceText);
            _replaceWidget->SetCursor(_currentCursor++, _count);
            SearchNext(searchString);
        } else {
            const QString message = "Replaced " + QString::number(_count) + "'" + searchString + "' by '" + replaceText + "'";
            QMessageBox::information(this, "Info", message);
        }
        _cursor.endEditBlock();
    }

    void PlainTextEditor::ReplaceAll(const QString &searchString, const QString &replaceText) {
        _cursor.beginEditBlock();
        _cursor.setPosition(0);
        _cursor = _document->find(searchString, _cursor);
        _currentCursor = 0;
        while (!_cursor.isNull() && !_cursor.atEnd()) {
            if (!_cursor.isNull()) {
                _replaceWidget->SetCursor(_currentCursor++, _count);
                _cursor.insertText(replaceText);
            }
            _cursor = _document->find(searchString, _cursor);
        }
        _replaceWidget->SetCursor(_count, _count);
        const QString message = "Replaced " + QString::number(_count) + " occurrences of '" + searchString + "' by '" + replaceText + "'";
        QMessageBox::information(this, "Info", message);
        _cursor.endEditBlock();
    }

    int PlainTextEditor::CountOccurrences(const QString &searchText) {
        _currentCursor = 1;
        int count = 0;
        QTextCursor cursor = _plainTextEdit->textCursor();
        cursor = _document->find(searchText, cursor);
        while (!cursor.isNull() && !cursor.atEnd()) {
            count++;
            cursor = _document->find(searchText, cursor);
        }
        _replaceWidget->SetCursor(_currentCursor, count);
        return count;
    }
}

