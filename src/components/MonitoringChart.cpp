//
// Created by vogje01 on 5/3/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MonitoringChart.h" resolved

#include <components/MonitoringChart.h>
#include "ui_MonitoringChart.h"
#include "utils/IconUtils.h"

namespace Awsmock::Components {

    const QList<QColor> MonitoringChart::_palette = {
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

    MonitoringChart::MonitoringChart(const MonitoringConfig &config, QGridLayout *layout, QWidget *parent) : BasePage(parent), _ui(new Ui::MonitoringChart), _config(config), _layout(layout) {

        // Stup dashboard service
        _dashboardService = new DashboardService();
        connect(_dashboardService, &DashboardService::ReloadMonitoringSignal, this, [this](const DashboardCounter &dashboardCounter) {
            Update(dashboardCounter);
        });

        // Setup UI components
        _ui->setupUi(this);

        // Settings
        _ui->settingsButton->setText(nullptr);
        _ui->settingsButton->setIcon(IconUtils::GetIcon("settings"));

        // Start dashboard
        MonitoringChart::LoadContent();
    }

    MonitoringChart::~MonitoringChart() {
        delete _ui;
    }

    void MonitoringChart::LoadContent() {
        _dashboardService->GetMultiSeriesCounter(_config);
    }

    void MonitoringChart::ClearContent() {
    }

    void MonitoringChart::Update(const DashboardCounter &dashboardCounters) {

        // QCustomChart
        //        if (const QLayoutItem *item = _layout->itemAtPosition(_config.row, _config.column)) {
        if (auto *customPlot = qobject_cast<QCustomPlot *>(_ui->monitoringGraph)) {
            customPlot->clearGraphs();
            int i = 0;
            double s = FLT_MAX;
            for (const auto &seria: dashboardCounters.valueMap.keys()) {

                customPlot->addGraph();
                QVector<QCPGraphData> graphData(dashboardCounters.valueMap[seria].size());
                for (int j = 0; j < dashboardCounters.valueMap[seria].size(); ++j) {
                    graphData[j].key = static_cast<double>(dashboardCounters.valueMap[seria][j].timestamp.toSecsSinceEpoch());
                    graphData[j].value = _config.scale > 0 ? dashboardCounters.valueMap[seria][j].value / _config.scale : dashboardCounters.valueMap[seria][j].value;
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
            customPlot->xAxis->setLabel(_config.xAxisText);
            customPlot->yAxis->setLabel(_config.yAxisText);

            // Time axis ticker — 5min intervals
            const QSharedPointer<QCPAxisTickerDateTime> ticker(new QCPAxisTickerDateTime);
            ticker->setDateTimeFormat(_config.xAxisFormat);
            ticker->setTickStepStrategy(QCPAxisTicker::tssReadability);
            customPlot->xAxis->setTicker(ticker);
            customPlot->xAxis->setLabel("Time");

            // Add a title using QCPTextElement
            if (auto *existing = qobject_cast<QCPTextElement *>(customPlot->plotLayout()->element(0, 0))) {
                existing->setText(_config.title); // update existing
            } else {
                customPlot->plotLayout()->insertRow(0);
                customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(customPlot, _config.title, QFont("sans", 12, QFont::Bold)));
            }

            // Redraw
            customPlot->replot();

            // Crosshair
            if (!customPlot->property("crosshair_init").toBool()) {
                AddCrossHair(customPlot);
            }

            AddZoom(customPlot);
            AddRange(customPlot);
            _layout->addWidget(this, _config.row, _config.column);
            //}
        }
    }

    void MonitoringChart::AddCrossHair(QCustomPlot *customPlot) {
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

            if (QCPGraph *g = FindNearestGraphPoint(customPlot, x, y, nx, ny, name); !g) return;

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

    QCPGraph *MonitoringChart::FindNearestGraphPoint(const QCustomPlot *plot, const double x, const double y, double &outX, double &outY, QString &outName) {
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

    void MonitoringChart::AddZoom(QCustomPlot *customPlot) {
        // Enable zoom with mouse wheel
        customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

        // Zoom only on x-axis (useful for time series)
        customPlot->axisRect()->setRangeZoom(Qt::Horizontal);
        customPlot->axisRect()->setRangeDrag(Qt::Horizontal);

        // Zoom both axes
        customPlot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
        customPlot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    }

    void MonitoringChart::AddRange(QCustomPlot *customPlot) {

        // Left click + drag = rubber band zoom
        customPlot->setSelectionRectMode(QCP::srmZoom);
    }

}
