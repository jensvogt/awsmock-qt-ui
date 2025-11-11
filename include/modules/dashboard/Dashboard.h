//
// Created by vogje01 on 11/6/25.
//

#ifndef AWSMOCK_QT_UI_DASHBOARD_H
#define AWSMOCK_QT_UI_DASHBOARD_H

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

#include <utils/BasePage.h>
#include <utils/IconUtils.h>

#include <modules/dashboard/ChartConfig.h>
#include <modules/dashboard/DashboardService.h>
#include <modules/dashboard/ChartConfig.h>
#include <modules/dashboard/CrossHairChartView.h>

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

    ChartConfig CreateChart(ChartConfig &chartConfig);

private:
    void LoadContent() override;

    static void CounterArrived(const DashboardCounter &dashboardCounters);

    /**
     * @brief UI components
     */
    Ui::Dashboard *ui;

    /**
     * @brief Parent widget
     */
    QWidget *parent;

    /**
     * @brief Toolbar
     */
    QHBoxLayout toolBar;

    /**
     * @brief REST service
     */
    DashboardService *dashboardService;

    /**
     * @brief Charts configuration map
     */
    QMap<QString, ChartConfig> chartConfigs;
};


#endif //AWSMOCK_QT_UI_DASHBOARD_H
