//
// Created by vogje01 on 5/14/26.
//

#pragma once

// Qt includes
#include <QWidget>

// Awsmock includes
#include <components/MonitoringChart.h>
#include <components/MonitoringConfig.h>
#include <utils/BasePage.h>
#include <utils/IconUtils.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class ApplicationDashboard;
}

QT_END_NAMESPACE

class ApplicationDashboard : public BasePage {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param parent parent widget
     */
    explicit ApplicationDashboard(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~ApplicationDashboard() override;

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
    Ui::ApplicationDashboard *_ui;

    /**
     * @brief Monitoring configs
     */
    QVector<Awsmock::Components::MonitoringConfig> _monitoringConfigs;

    /**
     * @brief Monitoring charts
     */
    QVector<Awsmock::Components::MonitoringChart *> _monitoringCharts;
};
