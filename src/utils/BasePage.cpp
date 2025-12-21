#include <utils/BasePage.h>

BasePage::BasePage(QWidget *parent) : QWidget(parent), _autoUpdateTimer(nullptr) {

    // Function key F5 refresh action
    const auto refreshAction = new QAction(this);
    refreshAction->setShortcut(QKeySequence(Qt::Key_F5));
    refreshAction->setShortcutContext(Qt::ApplicationShortcut);

    connect(refreshAction, &QAction::triggered, this, &BasePage::LoadContent);
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
    if (_autoUpdateTimer) {
        _autoUpdateTimer->stop();
    }
}

QAction *BasePage::GetRefreshAction(QWidget *parent) const {
    // Function key F5 refresh action
    const auto refreshAction = new QAction(parent);
    refreshAction->setShortcut(QKeySequence(Qt::Key_F5));
    refreshAction->setShortcutContext(Qt::ApplicationShortcut);
    connect(refreshAction, &QAction::triggered, this, &BasePage::LoadContent);
    return refreshAction;
}
