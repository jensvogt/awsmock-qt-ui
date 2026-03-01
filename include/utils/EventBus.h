//
// Created by vogje01 on 11/19/25.
//

#ifndef AWSMOCK_QT_UI_EVENT_BUS_H
#define AWSMOCK_QT_UI_EVENT_BUS_H

#include <QObject>

class EventBus final : public QObject {
    Q_OBJECT

public:
    static EventBus &instance() {
        static EventBus b;
        return b;
    }

signals:
    void TimerSignal(const QString &name, qint64 elapsed);

    void DockerStatsTimerSignal(const QString &name, qint64 elapsed);

    void RouteChanged(const QString &name, const QMap<QString, QString> &arguments = {});

    void MainStatusSignal(const QString &message);

    void PingSignal(bool result);

    void LogSignal(int logLevel, const QString &message);
};

#endif //AWSMOCK_QT_UI_EVENT_BUS_H
