//
// Created by vogje01 on 11/19/25.
//

#ifndef AWSMOCK_QT_UI_SCOPED_TIMER_H
#define AWSMOCK_QT_UI_SCOPED_TIMER_H

// Qt includes
#include <QDateTime>
#include <QString>
#include <utility>

#include "EventBus.h"

class ScopeTimer final : public QObject {
    Q_OBJECT

public:
    explicit ScopeTimer(QString name) : name(std::move(name)), start(QDateTime::currentMSecsSinceEpoch()) {
    }

    ~ScopeTimer() override {
        const qint64 elapsed = QDateTime::currentMSecsSinceEpoch() - start;
        EventBus::instance().TimerSignal(name, elapsed);
    }

signals:
    void ScopedTimerSignal(const QString &name, qint64 millis);

private:
    QString name;
    qint64 start;
};

#endif //AWSMOCK_QT_UI_SCOPED_TIMER_H
