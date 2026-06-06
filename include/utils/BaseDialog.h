#pragma once

// Qt includes
#include <QObject>
#include <QDialog>
#include <QTimer>

// Awsmock includes
#include <utils/TableUtils.h>
#include <utils/BasePage.h>
#include <utils/Configuration.h>

class BaseDialog : public QDialog, public TableUtils {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param parent parent widget
     */
    explicit BaseDialog(QWidget *parent);

    /**
     * @brief Start the auto update
     */
    void StartAutoUpdate();

    /**
     * @brief Stopps the auto update
     */
    void StopAutoUpdate() const;

    /**
     * @brief Load the content
     */
    virtual void LoadContent() = 0;

    /**
     * @brief Returns the parent widget
     */
    [[nodiscard]]
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
