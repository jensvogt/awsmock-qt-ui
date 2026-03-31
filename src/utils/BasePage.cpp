#include <utils/BasePage.h>

BasePage::BasePage(QWidget *parent) : QWidget(parent) {

    // Function key F5 refresh action
    _refreshAction = new QAction(this);
    _refreshAction->setShortcut(QKeySequence(Qt::Key_F5));
    _refreshAction->setShortcutContext(Qt::ApplicationShortcut);

    connect(_refreshAction, &QAction::triggered, this, &BasePage::LoadContent);
}

void BasePage::StartAutoUpdate() {

    // Initial load
    LoadContent();

    // Create a QTimer
    _autoUpdateTimer = new QTimer(this);

    // Connect its timeout signal to a slot/lambda
    connect(_autoUpdateTimer, &QTimer::timeout, this, &BasePage::LoadContent);

    // Set the refresh interval (e.g., every 5 seconds)
    _autoUpdateTimer->start(Configuration::instance().GetValue<int>("ui.auto-update-period", 10) * 1000);
}

void BasePage::StopAutoUpdate() const {
    // Stop the auto updater
    if (_autoUpdateTimer && _autoUpdateTimer->isActive()) {
        _autoUpdateTimer->stop();
        _autoUpdateTimer->deleteLater();
    }
}

QAction *BasePage::GetRefreshAction(QWidget *parent) const {
    return _refreshAction;
}
