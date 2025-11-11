//
// Created by vogje01 on 11/6/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_dashboard.h" resolved

#include <modules/dashboard/Dashboard.h>
#include "ui_Dashboard.h"

#include <QGuiApplication>

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
}

Dashboard::~Dashboard() {
    delete ui;
}

void Dashboard::Initialize() {

    ChartConfig config;
    config.title = "Total CPU";
    config.region = Configuration::instance().GetRegion();
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
    config.region = Configuration::instance().GetRegion();
    config.name = "cpu_usage_awsmock";
    config.series = "cpu_type";
    config.xAxisText = "Time";
    config.xAxisFormat = "HH:mm";
    config.yAxisText = "% CPU";
    config.yAxisFormat = "%.3f";
    config.row = 0;
    config.column = 1;
    config.seriesNames = {"total", "system", "user"};
    chartConfigs[QUuid::createUuid().toString()] = CreateChart(config);;

    config.title = "Total Memory";
    config.region = Configuration::instance().GetRegion();
    config.name = "memory_usage_total";
    config.series = "mem_type";
    config.xAxisText = "Time";
    config.xAxisFormat = "HH:mm";
    config.yAxisText = "% Memory";
    config.yAxisFormat = "%.1f";
    config.row = 0;
    config.column = 2;
    config.seriesNames = {"total"};
    chartConfigs[QUuid::createUuid().toString()] = CreateChart(config);;

    config.title = "AwsMock Memory";
    config.region = Configuration::instance().GetRegion();
    config.name = "memory_usage_awsmock";
    config.series = "mem_type";
    config.xAxisText = "Time";
    config.xAxisFormat = "HH:mm";
    config.yAxisText = "Memory [MB]";
    config.yAxisFormat = "%.1f";
    config.row = 1;
    config.scale = 1024 * 1024;
    config.column = 0;
    config.seriesNames = {"real", "virtual"};
    chartConfigs[QUuid::createUuid().toString()] = CreateChart(config);;

    config.title = "Total threads";
    config.region = Configuration::instance().GetRegion();
    config.name = "total_threads";
    config.series = "";
    config.xAxisText = "Time";
    config.xAxisFormat = "HH:mm";
    config.yAxisText = "Threads";
    config.yAxisFormat = "%.0f";
    config.row = 1;
    config.column = 1;
    config.seriesNames = {"total"};
    chartConfigs[QUuid::createUuid().toString()] = CreateChart(config);;

    config.title = "Gateway Response Time";
    config.region = Configuration::instance().GetRegion();
    config.name = "gateway_http_timer";
    config.series = "method";
    config.xAxisText = "Time";
    config.xAxisFormat = "HH:mm";
    config.yAxisText = "Time [ms]";
    config.yAxisFormat = "%.1f";
    config.row = 1;
    config.column = 2;
    config.limit = 5;
    config.seriesNames = {"GET", "PUT", "POST", "DELETE", "HEAD"};
    chartConfigs[QUuid::createUuid().toString()] = CreateChart(config);;

    config.title = "Gateway Requests";
    config.region = Configuration::instance().GetRegion();
    config.name = "gateway_http_counter";
    config.series = "method";
    config.xAxisText = "Time";
    config.xAxisFormat = "HH:mm";
    config.yAxisText = "Requests/s";
    config.yAxisFormat = "%.1f";
    config.row = 2;
    config.column = 0;
    config.limit = 5;
    config.seriesNames = {"GET", "PUT", "POST", "DELETE", "HEAD"};
    chartConfigs[QUuid::createUuid().toString()] = CreateChart(config);;

    config.title = "Docker CPU";
    config.region = Configuration::instance().GetRegion();
    config.name = "docker_cpu_total_counter";
    config.series = "container";
    config.xAxisText = "Time";
    config.xAxisFormat = "HH:mm";
    config.yAxisText = "% CPU";
    config.yAxisFormat = "%.1f";
    config.row = 2;
    config.column = 1;
    config.limit = 5;
    config.seriesNames = {"GET", "PUT", "POST", "DELETE", "HEAD"};
    chartConfigs[QUuid::createUuid().toString()] = CreateChart(config);;

    config.title = "Docker Memory";
    config.region = Configuration::instance().GetRegion();
    config.name = "docker_memory_counter";
    config.series = "container";
    config.xAxisText = "Time";
    config.xAxisFormat = "HH:mm";
    config.yAxisText = "% Memory";
    config.yAxisFormat = "%.1f";
    config.row = 2;
    config.column = 2;
    config.limit = 5;
    config.seriesNames = {"GET", "PUT", "POST", "DELETE", "HEAD"};
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

    // Auto-range axes to series range
    axisX->setRange(chartConfig.start, chartConfig.end);

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
}

void Dashboard::CounterArrived(const DashboardCounter &dashboardCounters) {

    const auto chart = dashboardCounters.chartConfig.chart;

    // Set legends
    if (dashboardCounters.valueMap.size() == 1) {
        chart->legend()->hide();
    }

    double max = 0;
    chart->removeAllSeries();
    for (auto const &[key, val]: dashboardCounters.valueMap) {

        const auto series = new QLineSeries();
        for (const auto &[timestamp, value]: val) {
            if (dashboardCounters.chartConfig.scale > 0) {
                max = std::max(value / dashboardCounters.chartConfig.scale, max);
                series->append(static_cast<qreal>(timestamp.toMSecsSinceEpoch()), value / dashboardCounters.chartConfig.scale);
            } else {
                max = std::max(value, max);
                series->append(static_cast<qreal>(timestamp.toMSecsSinceEpoch()), value);
            }
        }
        series->setName(key);
        chart->addSeries(series);
    }

    if (auto xAxis = chart->axes(Qt::Vertical); !xAxis.isEmpty()) {
        const auto axisX = qobject_cast<QValueAxis *>(xAxis.first());
        axisX->setRange(static_cast<qreal>(dashboardCounters.chartConfig.start.toMSecsSinceEpoch()), static_cast<qreal>(dashboardCounters.chartConfig.end.toMSecsSinceEpoch()));
    }
    if (auto yAxis = chart->axes(Qt::Vertical); !yAxis.isEmpty()) {
        const auto axisY = qobject_cast<QValueAxis *>(yAxis.first());
        axisY->setRange(0, max + 0.5 * max);
    }
    chart->update();
}
