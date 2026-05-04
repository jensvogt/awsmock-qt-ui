//
// Created by vogje01 on 5/3/26.
//

#ifndef AWSMOCK_QT_UI_MONITORING_CHART_H
#define AWSMOCK_QT_UI_MONITORING_CHART_H

#include <QWidget>


namespace Awsmock::Components {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class MonitoringChart;
    }

    QT_END_NAMESPACE

    class MonitoringChart : public QWidget {
        Q_OBJECT

    public:
        explicit MonitoringChart(QWidget *parent = nullptr);

        ~MonitoringChart() override;

    private:
        /**
         * @brief UI components
         */
        Ui::MonitoringChart *_ui{};
    };

} // namespace Awsmock::Components

#endif //AWSMOCK_QT_UI_MONITORING_CHART_H
