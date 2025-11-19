#include <utils/BasePage.h>

#include "utils/Configuration.h"

BasePage::BasePage(QWidget *parent) : QWidget(parent), _autoUpdateTimer(nullptr) {
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
    _autoUpdateTimer->stop();
}
