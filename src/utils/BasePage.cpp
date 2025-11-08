#include <utils/BasePage.h>

BasePage::BasePage(QWidget *parent) : QWidget(parent), autoUpdateTimer(nullptr) {
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

void BasePage::StopAutoUpdate() const {
    // Stop the auto updater
    autoUpdateTimer->stop();
}

void BasePage::SetColumn(QTableWidget *tableWidget, const int row, const int col, const QString &value) {
    tableWidget->setItem(row, col, new QTableWidgetItem(value));
}

void BasePage::SetColumn(QTableWidget *tableWidget, const int row, const int col, const int value) {
    const auto item = new QTableWidgetItem();
    item->setData(Qt::EditRole, QVariant::fromValue(value));
    tableWidget->setItem(row, col, item);
}

void BasePage::SetColumn(QTableWidget *tableWidget, const int row, const int col, const long value) {
    const auto item = new QTableWidgetItem();
    item->setData(Qt::EditRole, QVariant::fromValue(value));
    tableWidget->setItem(row, col, item);
}

void BasePage::SetColumn(QTableWidget *tableWidget, const int row, const int col, const QDateTime &value) {
    const auto item = new QTableWidgetItem();
    item->setData(Qt::EditRole, value.toString("yyyy-MM-dd hh:mm:ss"));
    tableWidget->setItem(row, col, item);
}

void BasePage::SetHiddenColumn(QTableWidget *tableWidget, int row, int col, const QString &value) {
    const auto item = new QTableWidgetItem;
    item->setData(Qt::EditRole, value);
    tableWidget->setItem(row, col, item);
}

void BasePage::SetHiddenColumn(QTableWidget *tableWidget, const int row, const int col, const bool value) {
    const auto checkItem = new QTableWidgetItem();
    checkItem->setCheckState(value ? Qt::Checked : Qt::Unchecked);
    checkItem->setFlags(checkItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    tableWidget->setItem(row, col, checkItem);
}
