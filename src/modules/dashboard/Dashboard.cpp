//
// Created by vogje01 on 11/6/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_dashboard.h" resolved

#include <modules/dashboard/Dashboard.h>
#include "ui_Dashboard.h"

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
}

Dashboard::~Dashboard() {
    delete ui;
}

void Dashboard::LoadContent() {
    const auto start = QDateTime(QDateTime::currentDateTime().date(), QTime(0, 0, 0));
    const auto end = QDateTime::currentDateTime();

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
    config.start = start;
    config.end = end;
    dashboardService->GetMultiSeriesCounter(config);

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
    config.start = start;
    config.end = end;
    dashboardService->GetMultiSeriesCounter(config);

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
    config.start = start;
    config.end = end;
    dashboardService->GetMultiSeriesCounter(config);

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
    config.start = start;
    config.end = end;
    dashboardService->GetMultiSeriesCounter(config);

    config.title = "Total threads";
    config.region = Configuration::instance().GetRegion();
    config.name = "total_threads";
    config.series = "";
    config.xAxisText = "Time";
    config.xAxisFormat = "HH:mm";
    config.yAxisText = "Threads";
    config.yAxisFormat = "%d";
    config.row = 1;
    config.column = 1;
    config.start = start;
    config.end = end;
    dashboardService->GetMultiSeriesCounter(config);

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
    config.start = start;
    config.end = end;
    config.limit = 5;
    dashboardService->GetMultiSeriesCounter(config);

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
    config.start = start;
    config.end = end;
    config.limit = 5;
    dashboardService->GetMultiSeriesCounter(config);

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
    config.start = start;
    config.end = end;
    config.limit = 5;
    dashboardService->GetMultiSeriesCounter(config);

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
    config.start = start;
    config.end = end;
    config.limit = 5;
    dashboardService->GetMultiSeriesCounter(config);
}

void Dashboard::CounterArrived(const DashboardCounter &dashboardCounters) {
    const auto chart = new QChart();
    if (dashboardCounters.valueMap.size() == 1) {
        chart->legend()->hide();
    }
    chart->createDefaultAxes();
    chart->setTitle(dashboardCounters.chartConfig.title);

    // X axis: QDateTimeAxis
    const auto axisX = new QDateTimeAxis(this);
    axisX->setFormat(dashboardCounters.chartConfig.xAxisFormat);
    axisX->setTitleText(dashboardCounters.chartConfig.xAxisText);
    axisX->setTickCount(6);
    chart->addAxis(axisX, Qt::AlignBottom);

    // Y axis: numeric
    const auto axisY = new QValueAxis(this);
    axisY->setLabelFormat(dashboardCounters.chartConfig.yAxisFormat);
    axisY->setTitleText(dashboardCounters.chartConfig.yAxisText);
    chart->addAxis(axisY, Qt::AlignLeft);

    // Auto-range axes to series range
    axisX->setRange(dashboardCounters.chartConfig.start, dashboardCounters.chartConfig.end);

    double max = 0;
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
        chart->addSeries(series);
        series->attachAxis(axisX);
        series->attachAxis(axisY);
        series->setName(key);
    }

    axisY->setRange(0, max + 0.5 * max);

    const auto chartView = new CrosshairChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setRubberBand(QChartView::RectangleRubberBand);
    chartView->show();

    ui->gridLayout->addWidget(chartView, dashboardCounters.chartConfig.row, dashboardCounters.chartConfig.column, 1, 1);
    NotifyStatusBar();
}
