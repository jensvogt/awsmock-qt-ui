
#include <utils/BasePage.h>

BasePage::BasePage(QWidget* parent) : QWidget(parent) {
}

void BasePage::StartAutoUpdate() {

    // Initial load
    LoadContent();

    // Create a QTimer
    autoUpdateTimer = new QTimer(this);

    // Connect its timeout signal to a slot/lambda
    connect(autoUpdateTimer, &QTimer::timeout, this, &BasePage::LoadContent);

    // Set the refresh interval (e.g., every 5 seconds)
    autoUpdateTimer->start(10000);
}

void BasePage::StopAutoUpdate(){

    // Stop the auto updater
    autoUpdateTimer->stop();
}
