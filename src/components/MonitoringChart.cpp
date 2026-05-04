//
// Created by vogje01 on 5/3/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MonitoringChart.h" resolved

#include <components/MonitoringChart.h>
#include "ui_MonitoringChart.h"

namespace Awsmock::Components {

    MonitoringChart::MonitoringChart(QWidget *parent) : QWidget(parent), _ui(new Ui::MonitoringChart) {
        _ui->setupUi(this);
    }

    MonitoringChart::~MonitoringChart() {
        delete _ui;
    }
}
