//
// Created by vogje01 on 11/6/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_dashboard.h" resolved

#include <modules/dashboard/Dashboard.h>
#include "ui_Dashboard.h"
#include "utils/EditConfigDialog.h"


Dashboard::Dashboard(const QString &title, QWidget *parent) : BasePage(parent), ui(new Ui::Dashboard), parent(parent) {
    // Connect service
    dashboardService = new DashboardService();
    connect(dashboardService, &DashboardService::ReloadMonitoringSignal, this, &Dashboard::CounterArrived);

    ui->setupUi(this);

    // Toolbar title
    ui->titleLabel->setText(title);
    ui->refreshButton->setIcon(IconUtils::GetIcon("dark", "refresh"));
    ui->refreshButton->setText(nullptr);

    // Toolbar refresh action
    ui->refreshButton->setIcon(IconUtils::GetIcon("dark", "refresh"));
    ui->refreshButton->setToolTip("Refresh the Dashboard");
    ui->refreshButton->setText(nullptr);
    connect(ui->refreshButton, &QPushButton::clicked, this, [this]() {
        LoadContent();
    });

    // Initialize charts
    Initialize();

    // Edit config dialog
    connect(&Configuration::instance(), &Configuration::ConfigurationChanged, this, [this](const QString &key, const QString &value) {
        if (key == "server.base-url") {
            chartConfigs.clear();
            Initialize();
            LoadContent();
        }
    });
}

Dashboard::~Dashboard() {
    delete ui;
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
    chartConfigs[QUuid::createUuid().toString()] = CreateChart(config);;

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
    chartConfigs[QUuid::createUuid().toString()] = CreateChart(config);;

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
    chartConfigs[QUuid::createUuid().toString()] = CreateChart(config);;

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
    chartConfigs[QUuid::createUuid().toString()] = CreateChart(config);;

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
    chartConfigs[QUuid::createUuid().toString()] = CreateChart(config);;

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
    chartConfigs[QUuid::createUuid().toString()] = CreateChart(config);;

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
    chartConfigs[QUuid::createUuid().toString()] = CreateChart(config);;

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
    chartConfigs[QUuid::createUuid().toString()] = CreateChart(config);;

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
    chartConfigs[QUuid::createUuid().toString()] = CreateChart(config);
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
    chartView->setAttribute(Qt::WA_OpaquePaintEvent);
    chartView->setAttribute(Qt::WA_NoSystemBackground);
    chartView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    chartView->setRubberBand(QChartView::RectangleRubberBand);
    chartView->show();

    ui->gridLayout->addWidget(chartView, chartConfig.row, chartConfig.column, 1, 1);
    chartConfig.chart = chart;
    return chartConfig;
}

void Dashboard::LoadContent() {

    if (Configuration::instance().GetConnectionState()) {
        const auto start = QDateTime(QDateTime::currentDateTime().date(), QTime(0, 0, 0));
        const auto end = QDateTime::currentDateTime();

        for (auto &config: chartConfigs) {
            config.start = start;
            config.end = end;
            dashboardService->GetMultiSeriesCounter(config);
        }
    } else {
        QMessageBox::critical(nullptr, "Error", "Backend is not reachable");
    }
    NotifyStatusBar();
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
