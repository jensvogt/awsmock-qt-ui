//
// Created by vogje01 on 11/6/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_dashboard.h" resolved

#include <modules/dashboard/Dashboard.h>
#include "ui_Dashboard.h"


Dashboard::Dashboard(const QString& title, QWidget *parent) : BasePage(parent), ui(new Ui::Dashboard) {

    ui->setupUi(this);

    // Toolbar title
    ui->titleLabel->setText(title);
    ui->refreshButton->setIcon(IconUtils::GetIcon("dark", "refresh"));
    ui->refreshButton->setText(nullptr);

    // Toolbar refresh action
    ui->refreshButton->setIcon(IconUtils::GetIcon("dark", "refresh"));
    ui->refreshButton->setToolTip("Refresh the Dashboard");
    ui->refreshButton->setText(nullptr);
    connect(ui->refreshButton, &QPushButton::clicked, this, [this](){
        LoadContent();
    });

    Dashboard::LoadContent();
}

Dashboard::~Dashboard() {
    delete ui;
}

void Dashboard::LoadContent() {

    ui->gridLayout->addWidget(GetChart(), 0, 0, 1, 1);
    ui->gridLayout->addWidget(GetChart(), 0, 1, 1, 1);
    ui->gridLayout->addWidget(GetChart(), 0, 2, 1, 1);
    ui->gridLayout->addWidget(GetChart(), 1, 0, 1, 1);
    ui->gridLayout->addWidget(GetChart(), 1, 1, 1, 1);
    ui->gridLayout->addWidget(GetChart(), 1, 2, 1, 1);
}

QChartView* Dashboard::GetChart() {

    QLineSeries *series = new QLineSeries();
    series->append(0, 6);
    series->append(2, 4);
    series->append(3, 8);
    series->append(7, 4);
    series->append(10, 5);

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Simple Line Chart Example");

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(100, 100);
    chartView->show();
    return chartView;
}