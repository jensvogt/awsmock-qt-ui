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
    config.scale = -1;
    _chartConfigs.emplace_back(config);

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
    _chartConfigs.emplace_back(config);

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
    _chartConfigs.emplace_back(config);

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
    _chartConfigs.emplace_back(config);

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
    _chartConfigs.emplace_back(config);

    config.title = "Gateway Response Time";
    config.name = "gateway_http_timer";
    config.series = "method";
    config.xAxisText = "Time";
    config.xAxisFormat = "HH:mm";
    config.yAxisText = "Time [ms]";
    config.yAxisFormat = "%.1f";
    config.row = 1;
    config.column = 2;
    config.limit = -1;
    config.scale = -1;
    _chartConfigs.emplace_back(config);

    config.title = "Gateway Requests";
    config.name = "gateway_http_counter";
    config.series = "method";
    config.xAxisText = "Time";
    config.xAxisFormat = "HH:mm";
    config.yAxisText = "Requests/s";
    config.yAxisFormat = "%.1f";
    config.row = 2;
    config.column = 0;
    config.limit = -1;
    config.scale = -1;
    _chartConfigs.emplace_back(config);

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
    _chartConfigs.emplace_back(config);

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
    _chartConfigs.emplace_back(config);
}

void Dashboard::ClearContent() {
}

void Dashboard::LoadContent() {
    const auto start = DateTimeUtils::GetLastMidnight();
    const auto end = QDateTime::currentDateTime();

    for (auto &config: _chartConfigs) {
        config.start = start.toUTC();
        config.end = end.toUTC();
        _dashboardService->GetMultiSeriesCounter(config);
    }
    logInfo << "Dashboard updated";
}

void Dashboard::CounterArrived(const DashboardCounter &dashboardCounters) {

    const ChartConfig config = dashboardCounters.chartConfig;

    // QCustomChart
    if (const QLayoutItem *item = _ui->gridLayout->itemAtPosition(config.row, config.column)) {
        if (auto *customPlot = qobject_cast<QCustomPlot *>(item->widget())) {
            customPlot->clearGraphs();
            int i = 0;
            double s = FLT_MAX;
            for (const auto &seria: dashboardCounters.valueMap.keys()) {

                customPlot->addGraph();
                QVector<QCPGraphData> graphData(dashboardCounters.valueMap[seria].size());
                for (int j = 0; j < dashboardCounters.valueMap[seria].size(); ++j) {
                    graphData[j].key = static_cast<double>(dashboardCounters.valueMap[seria][j].timestamp.toSecsSinceEpoch());
                    graphData[j].value = config.scale > 0 ? dashboardCounters.valueMap[seria][j].value / config.scale : dashboardCounters.valueMap[seria][j].value;
                    if (graphData[j].key < s) {
                        s = graphData[j].key;
                    }
                }
                customPlot->graph()->data()->set(graphData);
                customPlot->graph()->setName(seria);
                customPlot->graph()->setPen(QPen(_palette[i % _palette.size()], 2));
                i++;
            }

            // Set X range
            customPlot->xAxis->setRange(s, static_cast<double>(QDateTime::currentDateTime().toUTC().toSecsSinceEpoch()));

            // Auto scale Y axis
            customPlot->yAxis->rescale();
            customPlot->yAxis->setRangeLower(0);
            customPlot->xAxis->setLabel(config.xAxisText);
            customPlot->yAxis->setLabel(config.yAxisText);

            // Time axis ticker — 5min intervals
            const QSharedPointer<QCPAxisTickerDateTime> ticker(new QCPAxisTickerDateTime);
            ticker->setDateTimeFormat(config.xAxisFormat);
            ticker->setTickStepStrategy(QCPAxisTicker::tssReadability);
            customPlot->xAxis->setTicker(ticker);
            customPlot->xAxis->setLabel("Time");

            // Add a title using QCPTextElement
            if (auto *existing = qobject_cast<QCPTextElement *>(customPlot->plotLayout()->element(0, 0))) {
                existing->setText(config.title); // update existing
            } else {
                customPlot->plotLayout()->insertRow(0);
                customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(customPlot, config.title, QFont("sans", 12, QFont::Bold)));
            }

            // Redraw
            customPlot->replot();

            // Crosshair
            if (!customPlot->property("crosshair_init").toBool()) {
                AddCrossHair(customPlot);
            }

            AddZoom(customPlot);
            AddRange(customPlot);
        }
    }
}

void Dashboard::AddCrossHair(QCustomPlot *customPlot) {
    auto *tracer = new QCPItemTracer(customPlot);
    tracer->setStyle(QCPItemTracer::tsCircle);
    tracer->setPen(QPen(Qt::red));
    tracer->setBrush(Qt::red);
    tracer->setSize(7);
    tracer->setVisible(false);

    auto *hLine = new QCPItemLine(customPlot);
    auto *vLine = new QCPItemLine(customPlot);

    auto *label = new QCPItemText(customPlot);
    label->setPositionAlignment(Qt::AlignTop | Qt::AlignLeft);
    label->setPadding(QMargins(5, 5, 5, 5));
    label->setBrush(QColor(255, 255, 255, 220));
    label->setPen(QPen(Qt::gray));
    label->setVisible(false);
    connect(customPlot, &QCustomPlot::mouseMove, this, [=](QMouseEvent *event) {
        // Check if mouse is inside the actual plotting area
        if (!customPlot->axisRect()->rect().contains(event->pos())) {
            tracer->setVisible(false);
            hLine->setVisible(false);
            vLine->setVisible(false);
            label->setVisible(false);
            customPlot->replot(QCustomPlot::rpQueuedReplot);
            return;
        }

        const double x = customPlot->xAxis->pixelToCoord(event->pos().x());
        const double y = customPlot->yAxis->pixelToCoord(event->pos().y());

        // ===== Find nearest data point =====
        double nx = 0, ny = 0;
        QString name;

        if (QCPGraph *g = findNearestGraphPoint(customPlot, x, y, nx, ny, name); !g) return;

        // ===== Snap crosshair to data =====
        hLine->start->setCoords(customPlot->xAxis->range().lower, ny);
        hLine->end->setCoords(customPlot->xAxis->range().upper, ny);

        vLine->start->setCoords(nx, customPlot->yAxis->range().lower);
        vLine->end->setCoords(nx, customPlot->yAxis->range().upper);

        // ===== Move tracer =====
        tracer->position->setCoords(nx, ny);
        tracer->setVisible(true);

        // ===== Tooltip =====
        label->position->setCoords(nx, ny);
        label->setTextAlignment(Qt::AlignLeft);
        label->setText(QString("Seria: %1\nX: %2\nY: %3")
            .arg(name)
            .arg(QDateTime::fromSecsSinceEpoch(static_cast<qint64>(nx)).toLocalTime().toString("HH:mm:ss"))
            .arg(ny, 0, 'f', 3));
        label->setClipToAxisRect(false);
        label->setPositionAlignment(Qt::AlignTop | Qt::AlignLeft);
        // 1. Determine Horizontal Alignment (Left or Right)
        Qt::Alignment hAlign = Qt::AlignLeft;
        if (nx > customPlot->xAxis->range().center()) {
            // If we are in the right half of the plot, align text to the right
            // of the point so it stays inside the left area.
            hAlign = Qt::AlignRight;
        }

        // 2. Determine Vertical Alignment (Top or Bottom)
        Qt::Alignment vAlign = Qt::AlignTop;
        if (ny < customPlot->yAxis->range().center()) {
            // If we are in the bottom half, place label above the point.
            vAlign = Qt::AlignBottom;
        }

        // 3. Apply the combined alignment
        label->setPositionAlignment(hAlign | vAlign);
        label->setVisible(true);

        customPlot->replot(QCustomPlot::rpQueuedReplot);

        // Ensure lines are visible when we find a point
        hLine->setVisible(true);
        vLine->setVisible(true);
        tracer->setVisible(true);
        label->setVisible(true);
    });
}

QCPGraph *Dashboard::findNearestGraphPoint(const QCustomPlot *plot, const double x, const double y, double &outX, double &outY, QString &outName) {
    QCPGraph *bestGraph = nullptr;
    double bestDist = std::numeric_limits<double>::max();

    for (int i = 0; i < plot->graphCount(); ++i) {
        QCPGraph *g = plot->graph(i);
        if (!g || g->data()->isEmpty())
            continue;

        for (auto it = g->data()->constBegin(); it != g->data()->constEnd(); ++it) {

            const double dx = it->key - x;
            const double dy = it->value - y;

            if (const double dist = dx * dx + dy * dy; dist < bestDist) {
                bestDist = dist;
                bestGraph = g;
                outX = it->key;
                outY = it->value;
                outName = g->name();
            }
        }
    }

    return bestGraph;
}

void Dashboard::SetLegend(QCustomPlot *customPlot) {

    if (!customPlot || !customPlot->legend) return;

    auto *subLayout = new QCPLayoutGrid;
    customPlot->plotLayout()->addElement(1, 0, subLayout);
    for (int i = 0; i < customPlot->legend->itemCount(); ++i) {
        subLayout->addElement(0, i, customPlot->legend->item(i));
    }
    subLayout->setMaximumSize(QSize(10, 20));
    customPlot->legend->setVisible(true);

    customPlot->replot();
}

void Dashboard::AddZoom(QCustomPlot *customPlot) {
    // Enable zoom with mouse wheel
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    // Zoom only on x-axis (useful for time series)
    customPlot->axisRect()->setRangeZoom(Qt::Horizontal);
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal);

    // Zoom both axes
    customPlot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
}

void Dashboard::AddRange(QCustomPlot *customPlot) {

    // Left click + drag = rubber band zoom
    customPlot->setSelectionRectMode(QCP::srmZoom);

    // Right click = reset zoom
    // connect(customPlot, &QCustomPlot::mousePress, this, [this, customPlot](QMouseEvent *e) {
    //     if (e->button() == Qt::RightButton) {
    //         customPlot->xAxis->setRange(customPlot->graph()->data.first().key, _data.last().key);
    //         customPlot->yAxis->rescale();
    //         customPlot->replot();
    //     }
    // });
}
