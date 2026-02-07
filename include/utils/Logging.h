//
// Created by vogje01 on 2/7/26.
//

#ifndef AWSMOCK_QT_UI_LOGGING_H
#define AWSMOCK_QT_UI_LOGGING_H

// C++ include
#include<map>

// Qt includes
#include <QList>
#include <QString>

// Awsmock includes
#include <utils/EventBus.h>

#include "DateTimeUtils.h"
#include "DateTimeUtils.h"

enum LogLevel {
    ERROR,
    WARN,
    INFO,
    DEBUG,
    TRACE,
    NONE
};

static std::map<LogLevel, int> logLevelValue{
    {ERROR, 5000},
    {WARN, 4000},
    {INFO, 3000},
    {DEBUG, 2000},
    {TRACE, 1000},
    {NONE, 0},
};

static std::map<LogLevel, QString> logLevelNames{
    {ERROR, "error"},
    {WARN, "warn"},
    {INFO, "info"},
    {DEBUG, "debug"},
    {TRACE, "trace"},
};

[[maybe_unused]] static QString LogLevelToString(const LogLevel &logLevel) {
    return logLevelNames[logLevel];
}

[[maybe_unused]] static LogLevel LogLevelFromString(const QString &logLevel) {
    for (auto &[fst, snd]: logLevelNames) {
        if (snd == logLevel) {
            return fst;
        }
    }
    return NONE;
}

[[maybe_unused]] static int LogLevelToValue(const LogLevel &logLevel) {
    return logLevelValue[logLevel];
}

[[maybe_unused]] static LogLevel LogLevelFromValue(const int &logLevel) {
    for (auto &[fst, snd]: logLevelValue) {
        if (snd == logLevel) {
            return fst;
        }
    }
    return NONE;
}

static QStringList GetLogLevelNames() {
    QStringList logLevels;
    for (auto &snd: logLevelNames | std::views::values) {
        logLevels << snd;
    }
    return logLevels;
}

class LogSignaler : public QObject {
    Q_OBJECT
signals:
    void newLog(int type, const QString &msg);

public:
    static LogSignaler &instance() {
        static LogSignaler s;
        return s;
    }

    [[nodiscard]] int GetLevel() const {
        return _level;
    }

    void SetLevel(const int level) {
        _level = level;
    }

private:
    int _level = 5000;

    LogSignaler() = default; // Private constructor
};

inline QString ToStrLogLevel(const QtMsgType &logLevel) {
    switch (logLevel) {
        case QtDebugMsg:
            return "debug";
        case QtInfoMsg:
            return "info";
        case QtWarningMsg:
            return "warn";
        case QtCriticalMsg:
            return "error";
        case QtFatalMsg:
            return "fatal";
    }
    return "none";
}

inline int ToValueLogLevel(const QtMsgType &logLevel) {
    switch (logLevel) {
        case QtDebugMsg:
            return 2000;
        case QtInfoMsg:
            return 3000;
        case QtWarningMsg:
            return 4000;
        case QtCriticalMsg:
        case QtFatalMsg:
            return 5000;
    }
    return 0;
}

inline void myCustomMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    // Format the string with a timestamp
    const QString timestamp = DateTimeUtils::GetLogDateTimeFormat(QDateTime::currentDateTime());
    // Remove prefix from filename
    const char *lastSlash = strrchr(context.file, '/');
    const QString fileStr = lastSlash ? lastSlash + 1 : context.file;
    const QString finalMsg = QString("%1 [%2] [%3:%4] %5").arg(timestamp).arg(ToStrLogLevel(type)).arg(fileStr).arg(context.line).arg(msg);

    // 2. Emit the signal via our Singleton
    if (ToValueLogLevel(type) >= LogSignaler::instance().GetLevel()) {
        emit LogSignaler::instance().newLog(ToValueLogLevel(type), finalMsg);
    }
}

#define logError qCritical()
#define logWarning qWarning()
#define logInfo qInfo()
#define logDebug qDebug()
#define logTrace qDebug()

#endif //AWSMOCK_QT_UI_LOGGING_H
