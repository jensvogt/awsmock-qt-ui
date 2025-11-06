//
// Created by vogje01 on 11/6/25.
//

#ifndef AWSMOCK_QT_UI_DASHBOARD_H
#define AWSMOCK_QT_UI_DASHBOARD_H

#include <qboxlayout.h>
#include <QWidget>
#include <QLabel>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

#include <utils/BasePage.h>
#include <utils/IconUtils.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class Dashboard;
}

QT_END_NAMESPACE

class Dashboard final : public BasePage {
    Q_OBJECT

public:
    explicit Dashboard(const QString& title, QWidget *parent = nullptr);

    ~Dashboard() override;

private:

    void LoadContent() override;
    QChartView* GetChart();

    Ui::Dashboard *ui;
    QHBoxLayout toolBar;
};


#endif //AWSMOCK_QT_UI_DASHBOARD_H