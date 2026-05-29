//
// Created by vogje01 on 5/14/26.
//

#include <modules/application/ApplicationDashboard.h>
#include "ui_ApplicationDashboard.h"

ApplicationDashboard::ApplicationDashboard(QWidget *parent) : BasePage(parent), _ui(new Ui::ApplicationDashboard) {

    // Setup UI
    _ui->setupUi(this);

    // Toolbar title
    _ui->titleLabel->setText("Application Dashboard");

    // Back button
    _ui->backButton->setText(nullptr);
    _ui->backButton->setIcon(IconUtils::GetIcon("back"));
    connect(_ui->backButton, &QPushButton::clicked, [] {
        emit EventBus::instance().RouteChanged("Application");
    });

    // Refresh button
    _ui->refreshButton->setText(nullptr);
    _ui->refreshButton->setIcon(IconUtils::GetIcon("refresh"));
    connect(_ui->refreshButton, &QPushButton::clicked, [this] {
        LoadContent();
    });

    // Initialize charts
    Initialize();
}

ApplicationDashboard::~ApplicationDashboard() {
    delete _ui;
}

void ApplicationDashboard::Initialize() {

    // Service timer
    _monitoringConfigs.emplace_back(Awsmock::Components::MonitoringConfigBuilder()
        .setRegion(Configuration::instance().GetValue<QString>("aws.region", "eu-central-1"))
        .setTitle("Service Time")
        .setName("application_service_timer")
        .setSeries("action")
        .setXAxisText("Time")
        .setXAxisFormat("HH:mm")
        .setYAxisText("elapsed [ms]").setYAxisFormat("%.1f")
        .setRow(0)
        .setColumn(0)
        .setLimit(5)
        .build());

    // Service counter
    _monitoringConfigs.emplace_back(Awsmock::Components::MonitoringConfigBuilder()
        .setRegion(Configuration::instance().GetValue<QString>("aws.region", "eu-central-1"))
        .setTitle("Service Count")
        .setName("application_service_counter")
        .setSeries("action")
        .setXAxisText("Time")
        .setXAxisFormat("HH:mm")
        .setYAxisText("count").setYAxisFormat("%d")
        .setRow(0)
        .setColumn(1)
        .setLimit(5)
        .build());

    // Queue sizes
    _monitoringConfigs.emplace_back(Awsmock::Components::MonitoringConfigBuilder()
        .setRegion(Configuration::instance().GetValue<QString>("aws.region", "eu-central-1"))
        .setTitle("CPU Usage")
        .setName("application_cpu_usage")
        .setSeries("application")
        .setXAxisText("Time")
        .setXAxisFormat("HH:mm")
        .setYAxisText("time").setYAxisFormat("%d")
        .setRow(0)
        .setColumn(2)
        .setLimit(5)
        .build());

    // Message counter
    _monitoringConfigs.emplace_back(Awsmock::Components::MonitoringConfigBuilder()
        .setRegion(Configuration::instance().GetValue<QString>("aws.region", "eu-central-1"))
        .setTitle("Memory Usage")
        .setName("application_mem_usage")
        .setSeries("application")
        .setXAxisText("Time")
        .setXAxisFormat("HH:mm")
        .setYAxisText("mb").setYAxisFormat("%d")
        .setRow(1)
        .setColumn(0)
        .setLimit(5)
        .build());
}

void ApplicationDashboard::LoadContent() {
    _monitoringCharts.clear();
    const qint64 start = QDateTime::currentMSecsSinceEpoch();
    for (auto &config: _monitoringConfigs) {
        config.start = DateTimeUtils::GetLastMidnight().toUTC();
        config.end = QDateTime::currentDateTime().toUTC();
        _monitoringCharts.emplace_back(new Awsmock::Components::MonitoringChart(config, _ui->gridLayout));
    }
    const qint64 elapsed = QDateTime::currentMSecsSinceEpoch() - start;
    _ui->statusLabel->setText("Last update: " + QDateTime::currentDateTime().toString("hh:mm:ss") + " [" + QString::number(elapsed) + "ms]");
    logInfo << "Application dashboard updated";
}
