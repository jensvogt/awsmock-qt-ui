//
// Created by vogje01 on 5/14/26.
//

#ifndef AWSMOCK_QT_UI_SNS_DASHBOARD_H
#define AWSMOCK_QT_UI_SNS_DASHBOARD_H

// Qt includes
#include <QWidget>

// Awsmock includes
#include <components/MonitoringChart.h>
#include <components/MonitoringConfig.h>
#include <utils/BasePage.h>
#include <utils/IconUtils.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class SNSDashboard;
}

QT_END_NAMESPACE

class SNSDashboard : public BasePage {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param parent parent widget
     */
    explicit SNSDashboard(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~SNSDashboard() override;

    /**
     * @brief Initialization
     */
    void Initialize();

    /**
     * @brief Load content
     */
    void LoadContent() override;

    /**
     * @brief Clear content
     */
    void ClearContent() override {
    }

private:
    /**
     *  @brief UI components
     */
    Ui::SNSDashboard *_ui;

    /**
     * @brief Monitoring configs
     */
    QVector<Awsmock::Components::MonitoringConfig> _monitoringConfigs;

    /**
     * @brief Monitoring charts
     */
    QVector<Awsmock::Components::MonitoringChart *> _monitoringCharts;
};


#endif // AWSMOCK_QT_UI_SNS_DASHBOARD_H
