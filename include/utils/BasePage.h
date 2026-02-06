#ifndef AWSMOCK_QT_UI_UTILS_BASE_PAGE_H
#define AWSMOCK_QT_UI_UTILS_BASE_PAGE_H

#include <QDateTime>
#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QTableWidgetItem>

#include <utils/TableUtils.h>
#include <utils/Configuration.h>

class BasePage : public QWidget, public TableUtils {
    Q_OBJECT

public:
    explicit BasePage(QWidget *parent);

    void StartAutoUpdate();

    void StopAutoUpdate() const;

    QAction *GetRefreshAction(QWidget *parent) const;

    virtual void LoadContent() = 0;

    void SetArguments(const QMap<QString, QString> &arguments) {
        _arguments = arguments;
    }

    QMap<QString, QString> GetArguments() {
        return _arguments;
    }

    template<class T>
    T GetArgument(const QString &name) {
        const QVariant *v = new QVariant(_arguments[name]);
        if constexpr (std::is_same_v<T, int>) {
            return static_cast<T>(v->toInt());
        } else if constexpr (std::is_same_v<T, long>) {
            return static_cast<T>(v->toInt());
        } else if constexpr (std::is_same_v<T, double>) {
            return static_cast<T>(v->toDouble());
        } else if constexpr (std::is_same_v<T, QString>) {
            return static_cast<T>(v->toString());
        } else if constexpr (std::is_same_v<T, bool>) {
            return static_cast<T>(v->toString() == "true");
        }
        return {};
    }

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
    /**
     * @brief Auto update timer
     */
    QTimer *_autoUpdateTimer;

    /**
     * @brief Page arguments
     */
    QMap<QString, QString> _arguments;
};

#endif // AWSMOCK_QT_UI_UTILS_BASE_PAGE_H
