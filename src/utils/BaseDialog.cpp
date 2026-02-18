#include <utils/BaseDialog.h>

BaseDialog::BaseDialog(QWidget *parent) : QDialog(parent), _autoUpdateTimer(nullptr) {
}

void BaseDialog::StartAutoUpdate() {

    // Initial load
    LoadContent();

    // Create a QTimer
    _autoUpdateTimer = new QTimer(this);

    // Connect its timeout signal to a slot/lambda
    connect(_autoUpdateTimer, &QTimer::timeout, this, &BaseDialog::LoadContent);

    // Set the refresh interval (e.g., every 5 seconds)
    _autoUpdateTimer->start(Configuration::instance().GetValue<int>("ui.auto-update-period", 10) * 1000);
}

void BaseDialog::StopAutoUpdate() const {
    // Stop the auto updater
    if (_autoUpdateTimer) {
        _autoUpdateTimer->stop();
        _autoUpdateTimer->deleteLater();
        delete _autoUpdateTimer;
    }
}
