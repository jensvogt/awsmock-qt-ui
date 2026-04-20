//
// Created by vogje01 on 12/20/25.
//

#ifndef AWSMOCK_QT_UI_DATE_TIME_UTILS_H
#define AWSMOCK_QT_UI_DATE_TIME_UTILS_H

// Qt includes
#include <QLocale>
#include <QString>

// Awsmock includes
#include "Configuration.h"

class DateTimeUtils {
public:
    static QString GetDateTimeFormat(const QDateTime &dateTime) {
        if (!Configuration::instance().GetValue<QString>("ui.datetime-format").isEmpty()) {
            return dateTime.toString(Configuration::instance().GetValue<QString>("ui.datetime-format"));
        }
        if (const auto locale = Configuration::instance().GetValue<QString>("ui.default-locale"); locale == "US") {
            const QLocale us(QLocale::English, QLocale::UnitedStates);
            return us.toString(dateTime, QLocale::ShortFormat);
        } else {
            if (locale == "UK") {
                const QLocale uk(QLocale::English, QLocale::UnitedKingdom);
                return uk.toString(dateTime, QLocale::ShortFormat);
            }
            if (locale == "DE") {
                const QLocale de(QLocale::German, QLocale::Germany);
                return de.toString(dateTime, QLocale::ShortFormat);
            }
        }
        return "Unknown locale";
    }

    static QString GetLogTimeFormat(const QDateTime &dateTime) {
        if (!Configuration::instance().GetValue<QString>("ui.time-format").isEmpty()) {
            return dateTime.toString(Configuration::instance().GetValue<QString>("ui.time-format"));
        }
        if (const auto locale = Configuration::instance().GetValue<QString>("ui.default-locale"); locale == "US") {
            const QLocale us(QLocale::English, QLocale::UnitedStates);
            return us.toString(dateTime, QLocale::ShortFormat);
        } else {
            if (locale == "UK") {
                const QLocale uk(QLocale::English, QLocale::UnitedKingdom);
                return uk.toString(dateTime, QLocale::ShortFormat);
            }
            if (locale == "DE") {
                const QLocale de(QLocale::German, QLocale::Germany);
                return de.toString(dateTime, QLocale::ShortFormat);
            }
        }
        return "Unknown locale";
    }

    static QString GetLogDateTimeFormat(const QDateTime &dateTime) {
        if (!Configuration::instance().GetValue<QString>("ui.datetime-format").isEmpty()) {
            return dateTime.toString(Configuration::instance().GetValue<QString>("ui.datetime-format") + ".zzz");
        }
        if (const auto locale = Configuration::instance().GetValue<QString>("ui.datetime-locale"); locale == "US") {
            const QLocale us(QLocale::English, QLocale::UnitedStates);
            return us.toString(dateTime, QLocale::ShortFormat);
        } else {
            if (locale == "UK") {
                const QLocale uk(QLocale::English, QLocale::UnitedKingdom);
                return uk.toString(dateTime, QLocale::ShortFormat);
            }
            if (locale == "DE") {
                const QLocale de(QLocale::German, QLocale::Germany);
                return de.toString(dateTime, QLocale::ShortFormat);
            }
        }
        return "Unknown locale";
    }
};
#endif //AWSMOCK_QT_UI_DATE_TIME_UTILS_H
