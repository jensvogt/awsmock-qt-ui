#ifndef AWSMOCK_QT_UI_UTILS_BASE_PAGE_H
#define AWSMOCK_QT_UI_UTILS_BASE_PAGE_H

#include <QDateTime>
#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QTableWidgetItem>

#include <utils/TableUtils.h>

class BasePage : public QWidget, public TableUtils {
    Q_OBJECT

public:
    explicit BasePage(QWidget *parent);

    void StartAutoUpdate();

    void StopAutoUpdate() const;

    virtual void LoadContent() = 0;

signals:
    void StatusUpdateRequested(const QString &text);

public slots:
    /**
     * @brief Notify the status bar that the auto update finished
     */
    virtual void NotifyStatusBar() {
        const QString msg = "Last update: " + QDateTime::currentDateTime().toString("hh:mm:ss");
        //emit StatusUpdateRequested(msg);
    }

private:
    QTimer *_autoUpdateTimer;
};

#endif // AWSMOCK_QT_UI_UTILS_BASE_PAGE_H
