//
// Created by vogje01 on 5/14/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LambdaDashboard.h" resolved

#include <modules/lambda/LambdaDashboard.h>
#include "ui_LambdaDashboard.h"

LambdaDashboard::LambdaDashboard(QWidget *parent) : BasePage(parent), _ui(new Ui::LambdaDashboard) {

    // Setup UI
    _ui->setupUi(this);

    // Toolbar title
    _ui->titleLabel->setText("Lambda Dashboard");

    // Back button
    _ui->backButton->setText(nullptr);
    _ui->backButton->setIcon(IconUtils::GetIcon("back"));
    connect(_ui->backButton, &QPushButton::clicked, [] {
        emit EventBus::instance().RouteChanged("Lambda");
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

LambdaDashboard::~LambdaDashboard() {
    delete _ui;
}

void LambdaDashboard::Initialize() {

    // Service timer
    _monitoringConfigs.emplace_back(Awsmock::Components::MonitoringConfigBuilder()
        .setRegion(Configuration::instance().GetValue<QString>("aws.region", "eu-central-1"))
        .setTitle("Service Time")
        .setName("lambda_service_timer")
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
        .setName("lambda_service_counter")
        .setSeries("action")
        .setXAxisText("Time")
        .setXAxisFormat("HH:mm")
        .setYAxisText("count").setYAxisFormat("%d")
        .setRow(0)
        .setColumn(1)
        .setLimit(5)
        //.setSeriesNames({"total", "system", "user"})
        .build());

    // Queue sizes
    _monitoringConfigs.emplace_back(Awsmock::Components::MonitoringConfigBuilder()
        .setRegion(Configuration::instance().GetValue<QString>("aws.region", "eu-central-1"))
        .setTitle("Invocation time")
        .setName("lambda_invocation_timer")
        .setSeries("function_name")
        .setXAxisText("Time")
        .setXAxisFormat("HH:mm")
        .setYAxisText("time").setYAxisFormat("%d")
        .setRow(0)
        .setColumn(2)
        .setLimit(5)
        //.setSeriesNames({"total"})
        .build());

    // Message counter
    _monitoringConfigs.emplace_back(Awsmock::Components::MonitoringConfigBuilder()
        .setRegion(Configuration::instance().GetValue<QString>("aws.region", "eu-central-1"))
        .setTitle("Invocation Count")
        .setName("lambda_invocation_counter")
        .setSeries("function_name")
        .setXAxisText("Time")
        .setXAxisFormat("HH:mm")
        .setYAxisText("count").setYAxisFormat("%d")
        .setRow(1)
        .setColumn(0)
        .setLimit(5)
        //.setSeriesNames({"total"})
        .build());
}

void LambdaDashboard::LoadContent() {
    _monitoringCharts.clear();
    const qint64 start = QDateTime::currentMSecsSinceEpoch();
    for (auto &config: _monitoringConfigs) {
        config.start = DateTimeUtils::GetLastMidnight().toUTC();
        config.end = QDateTime::currentDateTime().toUTC();
        _monitoringCharts.emplace_back(new Awsmock::Components::MonitoringChart(config, _ui->gridLayout));
    }
    const qint64 elapsed = QDateTime::currentMSecsSinceEpoch() - start;
    _ui->statusLabel->setText("Last update: " + QDateTime::currentDateTime().toString("hh:mm:ss") + " [" + QString::number(elapsed) + "ms]");
    logInfo << "Lambda dashboard updated";
}
