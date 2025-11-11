//
// Created by vogje01 on 11/6/25.
//

#ifndef AWSMOCK_QT_CHART_CONFIG_H
#define AWSMOCK_QT_CHART_CONFIG_H

#include <QString>
#include <QDateTime>

struct ChartConfig {

    QString region;

    QString title;

    int row;

    int column;

    QString name;

    QString series;

    QDateTime start;

    QDateTime end;

    QString xAxisText;

    QString yAxisText;

    QString xAxisFormat;

    QString yAxisFormat;

    int limit = -1;

    int step = 5;

    int scale = -1;
};

#endif //AWSMOCK_QT_CHART_CONFIG_H
