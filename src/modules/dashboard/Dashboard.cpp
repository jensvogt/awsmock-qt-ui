//
// Created by vogje01 on 11/6/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_dashboard.h" resolved

#include <modules/dashboard/Dashboard.h>
#include "ui_Dashboard.h"

Dashboard::Dashboard(const QString &title, QWidget *parent) : BasePage(parent), _ui(new Ui::Dashboard), _parent(parent) {
    // Connect service
    _dashboardService = new DashboardService();
    connect(_dashboardService, &DashboardService::ReloadMonitoringSignal, this, &Dashboard::CounterArrived);

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
    connect(&Configuration::instance(), &Configuration::ConfigurationChanged, this, [this](const QString &key, const QString &value) {
        if (key == "server.base-url") {
            _chartConfigs.clear();
            Initialize();
            LoadContent();
        }
    });
}

Dashboard::~Dashboard() {
    delete _ui;
}

void Dashboard::Initialize() {
    ChartConfig config;
    config.region = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");
    config.title = "Total CPU";
    config.name = "cpu_usage_total";
    config.series = "cpu_type";
    config.xAxisText = "Time";
    config.xAxisFormat = "HH:mm";
    config.yAxisText = "% CPU";
    config.yAxisFormat = "%.1f";
    config.row = 0;
    config.column = 0;
    config.seriesNames = {"total", "system", "user"};
    _chartConfigs[QUuid::createUuid().toString()] = CreateChart(config);;

    config.title = "AwsMock CPU";
    config.name = "cpu_usage_awsmock";
    config.series = "cpu_type";
    config.xAxisText = "Time";
    config.xAxisFormat = "HH:mm";
    config.yAxisText = "% CPU";
    config.yAxisFormat = "%.3f";
    config.row = 0;
    config.column = 1;
    config.seriesNames = {"total", "system", "user"};
    config.scale = -1;
    _chartConfigs[QUuid::createUuid().toString()] = CreateChart(config);;

    config.title = "Total Memory";
    config.name = "memory_usage_total";
    config.series = "mem_type";
    config.xAxisText = "Time";
    config.xAxisFormat = "HH:mm";
    config.yAxisText = "% Memory";
    config.yAxisFormat = "%.1f";
    config.row = 0;
    config.column = 2;
    config.seriesNames = {"total"};
    config.scale = -1;
    _chartConfigs[QUuid::createUuid().toString()] = CreateChart(config);;

    config.title = "AwsMock Memory";
    config.name = "memory_usage_awsmock";
    config.series = "mem_type";
    config.xAxisText = "Time";
    config.xAxisFormat = "HH:mm";
    config.yAxisText = "Memory [MB]";
    config.yAxisFormat = "%d";
    config.row = 1;
    config.scale = 1024 * 1024;
    config.column = 0;
    config.seriesNames = {"real", "virtual"};
    _chartConfigs[QUuid::createUuid().toString()] = CreateChart(config);;

    config.title = "Total threads";
    config.name = "total_threads";
    config.series = "";
    config.xAxisText = "Time";
    config.xAxisFormat = "HH:mm";
    config.yAxisText = "Threads";
    config.yAxisFormat = "%d";
    config.row = 1;
    config.column = 1;
    config.seriesNames = {"total"};
    config.scale = -1;
    _chartConfigs[QUuid::createUuid().toString()] = CreateChart(config);;

    config.title = "Gateway Response Time";
    config.name = "gateway_http_timer";
    config.series = "method";
    config.xAxisText = "Time";
    config.xAxisFormat = "HH:mm";
    config.yAxisText = "Time [ms]";
    config.yAxisFormat = "%.1f";
    config.row = 1;
    config.column = 2;
    config.limit = 5;
    config.scale = -1;
    _chartConfigs[QUuid::createUuid().toString()] = CreateChart(config);;

    config.title = "Gateway Requests";
    config.name = "gateway_http_counter";
    config.series = "method";
    config.xAxisText = "Time";
    config.xAxisFormat = "HH:mm";
    config.yAxisText = "Requests/s";
    config.yAxisFormat = "%.1f";
    config.row = 2;
    config.column = 0;
    config.limit = 5;
    config.scale = -1;
    _chartConfigs[QUuid::createUuid().toString()] = CreateChart(config);;

    config.title = "Docker CPU";
    config.name = "docker_cpu_total_counter";
    config.series = "container";
    config.xAxisText = "Time";
    config.xAxisFormat = "HH:mm";
    config.yAxisText = "% CPU";
    config.yAxisFormat = "%.3f";
    config.row = 2;
    config.column = 1;
    config.limit = 5;
    config.scale = -1;
    _chartConfigs[QUuid::createUuid().toString()] = CreateChart(config);;

    config.title = "Docker Memory";
    config.name = "docker_memory_counter";
    config.series = "container";
    config.xAxisText = "Time";
    config.xAxisFormat = "HH:mm";
    config.yAxisText = "% Memory";
    config.yAxisFormat = "%.1f";
    config.row = 2;
    config.column = 2;
    config.limit = 5;
    config.scale = -1;
    _chartConfigs[QUuid::createUuid().toString()] = CreateChart(config);
}

ChartConfig Dashboard::CreateChart(ChartConfig &chartConfig) {
    const auto chart = new QChart();
    chart->createDefaultAxes();
    chart->setTitle(chartConfig.title);

    // X axis: QDateTimeAxis
    const auto axisX = new QDateTimeAxis(this);
    axisX->setFormat(chartConfig.xAxisFormat);
    axisX->setTitleText(chartConfig.xAxisText);
    axisX->setTickCount(6);
    chart->addAxis(axisX, Qt::AlignBottom);

    // Y axis: numeric
    const auto axisY = new QValueAxis(this);
    axisY->setLabelFormat(chartConfig.yAxisFormat);
    axisY->setTitleText(chartConfig.yAxisText);
    chart->addAxis(axisY, Qt::AlignLeft);
    chart->setAnimationOptions(QChart::NoAnimation);

    const auto chartView = new CrosshairChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    chartView->setRubberBand(QChartView::RectangleRubberBand);
    chartView->setAutoFillBackground(true);
    if (Configuration::instance().GetValue<QString>("ui.style-type") == "Dark") {
        chartView->setStyleSheet("background-color: #2b2b2b;");
    } else {
        chartView->setStyleSheet("background-color: white;");
    }
    chartView->show();

    _ui->gridLayout->addWidget(chartView, chartConfig.row, chartConfig.column, 1, 1);
    chartConfig.chart = chart;
    return chartConfig;
}

void Dashboard::ClearContent() {
}

void Dashboard::LoadContent() {
    const auto start = QDateTime(QDateTime::currentDateTime().date(), QTime(0, 0, 0));
    const auto end = QDateTime::currentDateTime();

    for (auto &config: _chartConfigs) {
        config.start = start;
        config.end = end;
        _dashboardService->GetMultiSeriesCounter(config);
    }
    logInfo << "Dashboard updated";
}

void Dashboard::CounterArrived(const DashboardCounter &dashboardCounters) {
    const auto chart = dashboardCounters.chartConfig.chart;

    // Reset chart
    chart->removeAllSeries();
    chart->removeAxis(chart->axes(Qt::Horizontal).value(0));
    chart->removeAxis(chart->axes(Qt::Vertical).value(0));

    // Hide legend when single series
    if (dashboardCounters.valueMap.size() == 1) {
        chart->legend()->hide();
    } else {
        chart->legend()->show();
    }

    // X AXIS (DateTime)
    auto *axisX = new QDateTimeAxis(chart);
    axisX->setFormat(dashboardCounters.chartConfig.xAxisFormat);
    axisX->setTitleText(dashboardCounters.chartConfig.xAxisText);
    axisX->setTickCount(6);
    axisX->setRange(dashboardCounters.chartConfig.start, dashboardCounters.chartConfig.end);
    chart->addAxis(axisX, Qt::AlignBottom);

    // Y AXIS (Numeric)
    auto *axisY = new QValueAxis(chart);
    axisY->setLabelFormat(dashboardCounters.chartConfig.yAxisFormat);
    axisY->setTitleText(dashboardCounters.chartConfig.yAxisText);
    chart->addAxis(axisY, Qt::AlignLeft);

    chart->setAnimationOptions(QChart::NoAnimation);

    // Build series
    double maxValue = 0;
    for (auto const &[key, val]: dashboardCounters.valueMap) {
        auto *series = new QLineSeries(chart);
        series->setName(key);

        for (const auto &[timestamp, value]: val) {
            double scaled = dashboardCounters.chartConfig.scale > 0 ? value / dashboardCounters.chartConfig.scale : value;
            maxValue = std::max(maxValue, scaled);
            series->append(static_cast<qreal>(timestamp.toMSecsSinceEpoch()), scaled);
        }

        chart->addSeries(series);

        // Attach after addSeries
        series->attachAxis(axisX);
        series->attachAxis(axisY);
    }

    axisY->setRange(0, maxValue);

    chart->update();
}
