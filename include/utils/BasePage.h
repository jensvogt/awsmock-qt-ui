#ifndef BASEPAGE_H
#define BASEPAGE_H

#include <QDateTime>
#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QTableWidgetItem>

class BasePage : public QWidget {
    Q_OBJECT

public:
    explicit BasePage(QWidget *parent);

    void StartAutoUpdate();

    void StopAutoUpdate() const;

    virtual void LoadContent() = 0;

    static void SetColumn(QTableWidget *tableWidget, int row, int col, const QString &value);

    static void SetColumn(QTableWidget *tableWidget, int row, int col, int value);

    static void SetColumn(QTableWidget *tableWidget, int row, int col, long value);

    static void SetColumn(QTableWidget *tableWidget, int row, int col, const QDateTime &value);

    static void SetHiddenColumn(QTableWidget *tableWidget, int row, int col, const QString &value);

    static void SetHiddenColumn(QTableWidget *tableWidget, int row, int col, bool value);

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
