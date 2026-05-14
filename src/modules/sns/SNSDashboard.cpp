//
// Created by vogje01 on 5/14/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SQSDashboard.h" resolved

#include <modules/sns/SNSDashboard.h>
#include "ui_SNSDashboard.h"

SNSDashboard::SNSDashboard(QWidget *parent) : BasePage(parent), _ui(new Ui::SNSDashboard) {

    // Setup UI
    _ui->setupUi(this);

    // Toolbar title
    _ui->titleLabel->setText("SNS Dashboard");

    // Back button
    _ui->backButton->setText(nullptr);
    _ui->backButton->setIcon(IconUtils::GetIcon("back"));
    connect(_ui->backButton, &QPushButton::clicked, [] {
        emit EventBus::instance().RouteChanged("SNS");
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

SNSDashboard::~SNSDashboard() {
    delete _ui;
}

void SNSDashboard::Initialize() {

    // Service timer
    _monitoringConfigs.emplace_back(Awsmock::Components::MonitoringConfigBuilder()
        .setRegion(Configuration::instance().GetValue<QString>("aws.region", "eu-central-1"))
        .setTitle("Service Time")
        .setName("sns_service_timer")
        .setSeries("action")
        .setXAxisText("Time")
        .setXAxisFormat("HH:mm")
        .setYAxisText("elapsed [ms]").setYAxisFormat("%.1f")
        .setRow(0)
        .setColumn(0)
        .setLimit(5)
        //.setSeriesNames({"total", "system", "user"})
        .build());

    // Service counter
    _monitoringConfigs.emplace_back(Awsmock::Components::MonitoringConfigBuilder()
        .setRegion(Configuration::instance().GetValue<QString>("aws.region", "eu-central-1"))
        .setTitle("Service Count")
        .setName("sns_service_counter")
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
        .setTitle("Topic Sizes")
        .setName("sns_topic_size")
        .setSeries("topic")
        .setXAxisText("Time")
        .setXAxisFormat("HH:mm")
        .setYAxisText("count").setYAxisFormat("%d")
        .setRow(0)
        .setColumn(2)
        .setLimit(5)
        //.setSeriesNames({"total"})
        .build());

    // Message counter
    _monitoringConfigs.emplace_back(Awsmock::Components::MonitoringConfigBuilder()
        .setRegion(Configuration::instance().GetValue<QString>("aws.region", "eu-central-1"))
        .setTitle("Message Count")
        .setName("sns_message_by_topic_counter")
        .setSeries("topic")
        .setXAxisText("Time")
        .setXAxisFormat("HH:mm")
        .setYAxisText("count").setYAxisFormat("%d")
        .setRow(1)
        .setColumn(0)
        .setLimit(5)
        //.setSeriesNames({"total"})
        .build());

    // Message wait time
    // _monitoringConfigs.emplace_back(Awsmock::Components::MonitoringConfigBuilder()
    //     .setRegion(Configuration::instance().GetValue<QString>("aws.region", "eu-central-1"))
    //     .setTitle("Message Wait Time")
    //     .setName("sqs_message_wait_time")
    //     .setSeries("queue")
    //     .setXAxisText("Time")
    //     .setXAxisFormat("HH:mm")
    //     .setYAxisText("waittime [s]").setYAxisFormat("%d")
    //     .setRow(1)
    //     .setColumn(1)
    //     .setLimit(5)
    //     .setScale(1000)
    //     //.setSeriesNames({"total"})
    //     .build());
}

void SNSDashboard::LoadContent() {
    _monitoringCharts.clear();
    const qint64 start = QDateTime::currentMSecsSinceEpoch();
    for (auto &config: _monitoringConfigs) {
        config.start = DateTimeUtils::GetLastMidnight().toUTC();
        config.end = QDateTime::currentDateTime().toUTC();
        _monitoringCharts.emplace_back(new Awsmock::Components::MonitoringChart(config, _ui->gridLayout));
    }
    const qint64 elapsed = QDateTime::currentMSecsSinceEpoch() - start;
    _ui->statusLabel->setText("Last update: " + QDateTime::currentDateTime().toString("hh:mm:ss") + " [" + QString::number(elapsed) + "ms]");
    logInfo << "SNS dashboard updated";
}
