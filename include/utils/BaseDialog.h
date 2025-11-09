#ifndef AWSMOCK_QT_UI_UTILS_BASE_DIALOG_H
#define AWSMOCK_QT_UI_UTILS_BASE_DIALOG_H

#include <QDateTime>
#include <QObject>
#include <QDialog>
#include <QTimer>
#include <QTableWidgetItem>

#include <utils/TableUtils.h>

class BaseDialog : public QDialog, public TableUtils {
    Q_OBJECT

public:
    explicit BaseDialog(QWidget *parent);

signals:
    void StatusUpdateRequested(const QString &text);

public slots:
    virtual void NotifyStatusBar() {
        const QString msg = "Last update: " + QDateTime::currentDateTime().toString("hh:mm:ss");
        emit StatusUpdateRequested(msg);
    }
};

#endif // AWSMOCK_QT_UI_UTILS_BASE_DIALOG_H
