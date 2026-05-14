//
// Created by vogje01 on 5/3/26.
//

#ifndef AWSMOCK_QT_UI_MONITORING_CHART_H
#define AWSMOCK_QT_UI_MONITORING_CHART_H

// C++ include
#pragma once
#include <cfloat>

// Qt includes
#include <QDateTime>
#include <QString>
#include <QStringList>
#include <QWidget>

// Awsmock includes
#include <components/MonitoringConfig.h>
#include <components/QCustomPlot.h>
#include <dto/dashboard/DashboardCounterResult.h>
#include <modules/dashboard/DashboardService.h>
#include <utils/BasePage.h>

namespace Awsmock::Components {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class MonitoringChart;
    }

    QT_END_NAMESPACE


    class MonitoringChart : public BasePage {
        Q_OBJECT

    public:
        /**
         * @brief Constructor
         *
         * @param config monitoring config
         * @param layout grid layout
         * @param parent parent widget
         */
        explicit MonitoringChart(const MonitoringConfig &config, QGridLayout *layout, QWidget *parent = nullptr);

        /**
         * @brief Destructor
         */
        ~MonitoringChart() override;

        /**
         * @brief Update monitoring data
         *
         * @param dashboardCounters list of monitoring values
         */
        void Update(const DashboardCounter &dashboardCounters);

        /**
         * @brief Load page content
         */
        void LoadContent() override;

        void ClearContent();

    private:
        /**
         * @brief UI components
         */
        Ui::MonitoringChart *_ui{};

        /**
         * @brief Add the crosshair lines
         *
         * @param customPlot QCustom plot
         */
        void AddCrossHair(QCustomPlot *customPlot);

        /**
         *
         * @param plot QCustomPlot
         * @param x X position
         * @param y Y position
         * @param outX output X position
         * @param outY output Y position
         * @param outName output seria name
         * @return QCustom plot graph
         */
        static QCPGraph *FindNearestGraphPoint(const QCustomPlot *plot, double x, double y, double &outX, double &outY, QString &outName);

        /**
         * @brief Add the zooming
         *
         * @param customPlot custom plot
         */
        static void AddZoom(QCustomPlot *customPlot);

        /**
         * @brief Add range selection
         *
         * @param customPlot custom plot
         */
        static void AddRange(QCustomPlot *customPlot);

        /**
         * @brief Monitoring service
         */
        DashboardService *_dashboardService{};

        /**
         * @brief Configuration
         */
        MonitoringConfig _config;

        QGridLayout *_layout;

        /**
         * @brief Color palette
         */
        const static QList<QColor> _palette;
    };

} // namespace Awsmock::Components

#endif //AWSMOCK_QT_UI_MONITORING_CHART_H
