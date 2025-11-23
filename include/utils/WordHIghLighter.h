//
// Created by vogje01 on 11/23/25.
//

#ifndef AWSMOCK_QT_UI_WORD_HIGH_LIGHTER_H
#define AWSMOCK_QT_UI_WORD_HIGH_LIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class WordHighlighter final : public QSyntaxHighlighter {
public:
    explicit WordHighlighter(QTextDocument *parent = nullptr)
        : QSyntaxHighlighter(parent) {
    }

    QString word;
    QTextCharFormat format;

protected:
    void highlightBlock(const QString &text) override {
        if (word.isEmpty())
            return;

        const QRegularExpression re(QRegularExpression::escape(word));
        auto it = re.globalMatch(text);

        while (it.hasNext()) {
            auto match = it.next();
            setFormat(static_cast<int>(match.capturedStart()), static_cast<int>(match.capturedLength()), format);
        }
    }
};
#endif //AWSMOCK_QT_UI_WORD_HIGH_LIGHTER_H
