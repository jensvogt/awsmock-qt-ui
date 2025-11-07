#ifndef BASEPAGE_H
#define BASEPAGE_H

#include <QDateTime>
#include <QObject>
#include <QWidget>
#include <QTimer>

class BasePage : public QWidget {
    Q_OBJECT

public:
    explicit BasePage(QWidget *parent);

    void StartAutoUpdate();

    void StopAutoUpdate() const;

    virtual void LoadContent() = 0;

signals:
    void StatusUpdateRequested(const QString &text);

public slots:
    virtual void NotifyStatusBar() {
        const QString msg = "Last update: " + QDateTime::currentDateTime().toString("hh:mm:ss");
        emit StatusUpdateRequested(msg);
    }

private:
    QTimer *autoUpdateTimer;
};

#endif // BASEPAGE_H
