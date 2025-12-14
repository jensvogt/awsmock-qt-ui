//
// Created by vogje01 on 12/11/25.
//

#ifndef AWSMOCK_QT_UI_STRING_UTILS_H
#define AWSMOCK_QT_UI_STRING_UTILS_H

// C++ standard includes
#include <algorithm>
#include <string>

// Qt includes
#include <QString>

class StringUtils {

public:
    static QString CleanupLogString(const QString &log_string) {
        QString result;

        for (unsigned char c: log_string.toStdString()) {
            // Check if it's printable OR is a necessary formatting character
            if (std::isprint(c) || c == '\n' || c == '\r' || c == '\t') {
                result.append(QChar(c));
            }
            // Note: This approach discards any non-printable character.
        }
        return QString(result);
    }

private:
    static bool IsControlOrNonAscii(const unsigned char c) {
        // Check if the character is NOT a standard printable character (ASCII 32-126)
        // AND is NOT a tab (\t), newline (\n), or carriage return (\r).
        // This allows you to keep the line breaks.

        // isprint returns true for printable characters
        // We want to remove characters where isprint is false,
        // UNLESS it's a known whitespace character we want to keep.

        return !std::isprint(c) && c != '\n' && c != '\r' && c != '\t';
    }
};
#endif //AWSMOCK_QT_UI_STRING_UTILS_H
