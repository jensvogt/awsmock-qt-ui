//
// Created by vogje01 on 12/11/25.
//

#ifndef AWSMOCK_QT_UI_STRING_UTILS_H
#define AWSMOCK_QT_UI_STRING_UTILS_H

// C++ standard includes
#include <string>

// Qt includes
#include <QString>
#include <QRandomGenerator>

// Awsmock includes
#include <utils/Logging.h>

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

    static QString ConvertToIndentedJson(const QString &input) {

        QJsonParseError parseError;
        const QJsonDocument doc = QJsonDocument::fromJson(input.toUtf8(), &parseError);
        if (parseError.error != QJsonParseError::NoError) {
            logError << "Cannot parse JSON document: " << parseError.errorString();
            return input;
        }
        return doc.toJson(QJsonDocument::Indented);
    }

    static QString GenerateRandomString(const int length) {
        const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
        QString randomString;
        randomString.reserve(length);

        for (int i = 0; i < length; ++i) {
            const int index = QRandomGenerator::global()->bounded(possibleCharacters.length());
            randomString.append(possibleCharacters.at(index));
        }
        return randomString;
    }

    static QString GenerateRandomNumericString(const int length) {
        const QString digits = "0123456789";
        QString result;
        result.reserve(length); // Efficiency: Allocate memory once

        for (int i = 0; i < length; ++i) {
            // Generates a random index from 0 to 9
            const quint32 index = QRandomGenerator::global()->bounded(10);
            result.append(digits.at(index));
        }
        return result;
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
