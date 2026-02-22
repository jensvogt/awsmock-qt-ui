//
// Created by vogje01 on 2/22/26.
//

#include <QMessageBox>
#include <components/PlainTextEditor.h>

#include "utils/StringUtils.h"

namespace Awsmock::Components {

    PlainTextEditor::PlainTextEditor(const bool beautify, const TextContentType &contentType, QWidget *parent) : QWidget(parent), _contentType(contentType), _beautify(beautify) {

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

        // Setup plain text edit
        _plainTextEdit = new QPlainTextEdit(this);

        // Add replace shortcut
        auto *replaceAction = new QAction(this);
        replaceAction->setShortcut(QKeySequence("Ctrl+R"));
        replaceAction->setShortcutContext(Qt::WidgetShortcut);
        _plainTextEdit->addAction(replaceAction);
        connect(replaceAction, &QAction::triggered, this, &PlainTextEditor::InitializeReplaceText);

        const auto layout = new QVBoxLayout(this);
        layout->addWidget(_replaceWidget);
        layout->addWidget(_plainTextEdit);
    }

    void PlainTextEditor::InitializeReplaceText() const {
        _replaceWidget->setHidden(false);
    }

    void PlainTextEditor::SetText(const QString &text) {
        _text = text;

        if (_beautify && _contentType == JSON) {
            _text = StringUtils::ConvertToIndentedJson(text);
        }

        _plainTextEdit->setPlainText(_text);
        _document = _plainTextEdit->document();
        _cursor = QTextCursor(_document);
    }

    void PlainTextEditor::SearchNext(const QString &searchText) {
        if (_plainTextEdit->find(searchText)) {
            _replaceWidget->SetCursor(_currentCursor++, _count);
            return;
        }

        // Wrap: restart from beginning
        _cursor = _plainTextEdit->textCursor();
        _cursor.movePosition(QTextCursor::Down);
        _plainTextEdit->setTextCursor(_cursor);
    }

    void PlainTextEditor::SearchPrevious(const QString &searchText) {
        if (_plainTextEdit->find(searchText, QTextDocument::FindBackward)) {
            _replaceWidget->SetCursor(_currentCursor--, _count);
            return;
        }

        // Wrap: restart from beginning
        _cursor = _plainTextEdit->textCursor();
        _cursor.movePosition(QTextCursor::Up);
        _plainTextEdit->setTextCursor(_cursor);
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

