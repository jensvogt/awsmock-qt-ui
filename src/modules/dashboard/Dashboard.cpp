//
// Created by vogje01 on 11/6/25.
//

#include <modules/dashboard/Dashboard.h>
#include "ui_Dashboard.h"

const QList<QColor> Dashboard::_palette = {
    QColor(31, 119, 180), // blue
    QColor(255, 127, 14), // orange
    QColor(44, 160, 44), // green
    QColor(214, 39, 40), // red
    QColor(148, 103, 189), // purple
    QColor(140, 86, 75), // brown
    QColor(227, 119, 194), // pink
    QColor(127, 127, 127), // grey
    QColor(188, 189, 34), // yellow-green
    QColor(23, 190, 207), // cyan
};

Dashboard::Dashboard(const QString &title, QWidget *parent) : BasePage(parent), _ui(new Ui::Dashboard), _parent(parent) {

    // Setup UI components
    _ui->setupUi(this);

    // Toolbar title
    _ui->titleLabel->setText(title);

    // Toolbar refresh action
    _ui->refreshButton->setText(nullptr);
    _ui->refreshButton->setIcon(IconUtils::GetIcon("refresh"));
    _ui->refreshButton->setToolTip("Refresh the dashboard");
    connect(_ui->refreshButton, &QPushButton::clicked, this, [this]() {
        LoadContent();
    });

    // Initialize charts
    Initialize();

    // Handle configuration changes, specially when the base URL changes
    connect(&Configuration::instance(), &Configuration::ConfigurationChanged, this, [this](const QString &key, const QString &) {
        if (key == "server.base-url") {
            _monitoringConfigs.clear();
            Initialize();
            LoadContent();
        }
    });
}

Dashboard::~Dashboard() {
    delete _ui;
}

void Dashboard::Initialize() {

    _monitoringConfigs.emplace_back(Awsmock::Components::MonitoringConfigBuilder()
        .setRegion(Configuration::instance().GetValue<QString>("aws.region", "eu-central-1"))
        .setTitle("Total CPU")
        .setName("cpu_usage_total")
        .setSeries("cpu_type")
        .setXAxisText("Time")
        .setXAxisFormat("HH:mm")
        .setYAxisText("% CPU").setYAxisFormat("%.1f")
        .setRow(0)
        .setColumn(0)
        .setSeriesNames({"total", "system", "user"})
        .build());

    _monitoringConfigs.emplace_back(Awsmock::Components::MonitoringConfigBuilder()
        .setRegion(Configuration::instance().GetValue<QString>("aws.region", "eu-central-1"))
        .setTitle("AwsMock CPU")
        .setName("cpu_usage_awsmock")
        .setSeries("cpu_type")
        .setXAxisText("Time")
        .setXAxisFormat("HH:mm")
        .setYAxisText("% CPU")
        .setYAxisFormat("%.3f")
        .setRow(0)
        .setColumn(1)
        .setSeriesNames({"total", "system", "user"})
        .build());

    _monitoringConfigs.emplace_back(Awsmock::Components::MonitoringConfigBuilder()
        .setRegion(Configuration::instance().GetValue<QString>("aws.region", "eu-central-1"))
        .setTitle("Total Memory")
        .setName("memory_usage_total")
        .setSeries("mem_type")
        .setXAxisText("Time")
        .setXAxisFormat("HH:mm")
        .setYAxisText("% Memory")
        .setYAxisFormat("%.1f")
        .setRow(0)
        .setColumn(2)
        .setSeriesNames({"total"})
        .build());

    _monitoringConfigs.emplace_back(Awsmock::Components::MonitoringConfigBuilder()
        .setRegion(Configuration::instance().GetValue<QString>("aws.region", "eu-central-1"))
        .setTitle("AwsMock Memory")
        .setName("memory_usage_awsmock")
        .setSeries("mem_type")
        .setXAxisText("Time")
        .setXAxisFormat("HH:mm")
        .setYAxisText("% Memory [MB]")
        .setYAxisFormat("%d")
        .setRow(1)
        .setColumn(0)
        .setSeriesNames({"real", "virtual"})
        .setScale(1024 * 1024)
        .build());

    _monitoringConfigs.emplace_back(Awsmock::Components::MonitoringConfigBuilder()
        .setRegion(Configuration::instance().GetValue<QString>("aws.region", "eu-central-1"))
        .setTitle("Total threads")
        .setName("total_threads")
        .setSeries("")
        .setXAxisText("Time")
        .setXAxisFormat("HH:mm")
        .setYAxisText("Threads")
        .setYAxisFormat("%d")
        .setRow(1)
        .setColumn(1)
        .setSeriesNames({"total"})
        .build());

    _monitoringConfigs.emplace_back(Awsmock::Components::MonitoringConfigBuilder()
        .setRegion(Configuration::instance().GetValue<QString>("aws.region", "eu-central-1"))
        .setTitle("Gateway Response Time")
        .setName("gateway_http_timer")
        .setSeries("method")
        .setXAxisText("Time")
        .setXAxisFormat("HH:mm")
        .setYAxisText("Time [ms]")
        .setYAxisFormat("%.1f")
        .setRow(1)
        .setColumn(2)
        .setSeriesNames({"total"})
        .build());

    _monitoringConfigs.emplace_back(Awsmock::Components::MonitoringConfigBuilder()
        .setRegion(Configuration::instance().GetValue<QString>("aws.region", "eu-central-1"))
        .setTitle("Gateway Requests")
        .setName("gateway_http_counter")
        .setSeries("method")
        .setXAxisText("Time")
        .setXAxisFormat("HH:mm")
        .setYAxisText("Requests/s")
        .setYAxisFormat("%.1f")
        .setRow(2)
        .setColumn(0)
        .setSeriesNames({"total"})
        .build());

    _monitoringConfigs.emplace_back(Awsmock::Components::MonitoringConfigBuilder()
        .setRegion(Configuration::instance().GetValue<QString>("aws.region", "eu-central-1"))
        .setTitle("Docker CPU")
        .setName("docker_cpu_total_counter")
        .setSeries("container")
        .setXAxisText("Time")
        .setXAxisFormat("HH:mm")
        .setYAxisText("% CPU")
        .setYAxisFormat("%.3f")
        .setRow(2)
        .setColumn(1)
        .setSeriesNames({"total"})
        .build());

    _monitoringConfigs.emplace_back(Awsmock::Components::MonitoringConfigBuilder()
        .setRegion(Configuration::instance().GetValue<QString>("aws.region", "eu-central-1"))
        .setTitle("Docker Memory")
        .setName("docker_memory_counter")
        .setSeries("container")
        .setXAxisText("Time")
        .setXAxisFormat("HH:mm")
        .setYAxisText("% CPU")
        .setYAxisFormat("%.1f")
        .setRow(2)
        .setColumn(2)
        .setLimit(5)
        .setSeriesNames({"total"})
        .build());
}

void Dashboard::ClearContent() {
}

void Dashboard::LoadContent() {
    _monitoringCharts.clear();
    for (auto &config: _monitoringConfigs) {
        config.start = DateTimeUtils::GetLastMidnight().toUTC();
        config.end = QDateTime::currentDateTime().toUTC();
        _monitoringCharts.emplace_back(new Awsmock::Components::MonitoringChart(config, _ui->gridLayout));
    }
    logInfo << "Dashboard updated";
}
