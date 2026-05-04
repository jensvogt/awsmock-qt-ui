#ifndef AWSMOCK_QT_UI_UTILS_BASE_DIALOG_H
#define AWSMOCK_QT_UI_UTILS_BASE_DIALOG_H

#include <QObject>
#include <QDialog>
#include <QTimer>

#include <utils/TableUtils.h>
#include <utils/BasePage.h>
#include <utils/Configuration.h>

class BaseDialog : public QDialog, public TableUtils {
    Q_OBJECT

public:
    explicit BaseDialog(QWidget *parent);

    void StartAutoUpdate();

    void StopAutoUpdate() const;

    virtual void LoadContent() = 0;

    QWidget *GetParent() const { return _parent; }

signals:
    void DialogStatusUpdateSignal(const QString &text);

public slots:
    virtual void NotifyStatusBar() {
        const QString msg = "Last update: " + QDateTime::currentDateTime().toString("hh:mm:ss");
        emit DialogStatusUpdateSignal(msg);
    }

private:
    /**
     * @brief Parent widget
     */
    QWidget *_parent;

    /**
     * @brief Auto update timer
     */
    QTimer *_autoUpdateTimer;
};

#endif // AWSMOCK_QT_UI_UTILS_BASE_DIALOG_H
