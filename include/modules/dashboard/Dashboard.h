//
// Created by vogje01 on 11/6/25.
//

#ifndef AWSMOCK_QT_UI_DASHBOARD_H
#define AWSMOCK_QT_UI_DASHBOARD_H

// C++ includes
#include <cfloat>

// Qt includes
#include <QBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

// Awsmock includes
#include <components/QCustomPlot.h>
#include <components/MonitoringChart.h>
#include <components/MonitoringConfig.h>
#include <components/MonitoringConfig.h>
#include <modules/dashboard/ChartConfig.h>
#include <modules/dashboard/ChartConfig.h>
#include <modules/dashboard/CrossHairChartView.h>
#include <modules/dashboard/DashboardService.h>
#include <utils/BasePage.h>
#include <utils/Configuration.h>
#include <utils/EditConfigDialog.h>
#include <utils/IconUtils.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class Dashboard;
}

QT_END_NAMESPACE

class Dashboard final : public BasePage {
    Q_OBJECT

public:
    explicit Dashboard(const QString &title, QWidget *parent = nullptr);

    ~Dashboard() override;

    void Initialize();

    void LoadContent() override;

    void ClearContent() override;

private:
    void CounterArrived(const DashboardCounter &dashboardCounters);

    void AddCrossHair(QCustomPlot *customPlot);

    static QCPGraph *findNearestGraphPoint(const QCustomPlot *plot, double x, double y, double &outX, double &outY, QString &outName);

    static void SetLegend(QCustomPlot *customPlot);

    static void AddZoom(QCustomPlot *customPlot);

    static void AddRange(QCustomPlot *customPlot);

    /**
     * @brief UI components
     */
    Ui::Dashboard *_ui;

    /**
     * @brief Parent widget
     */
    QWidget *_parent;

    /**
     * @brief Toolbar
     */
    QHBoxLayout _toolBar;

    /**
     * @brief REST service
     */
    DashboardService *_dashboardService{};

    /**
     * @brief Charts configuration map
     */
    QVector<ChartConfig> _chartConfigs;
    QVector<Awsmock::Components::MonitoringConfig> _monitoringConfigs;
    QVector<Awsmock::Components::MonitoringChart *> _monitoringCharts;

    /**
     * @brief Color palette
     */
    const static QList<QColor> _palette;
};


#endif //AWSMOCK_QT_UI_DASHBOARD_H
