//
// Created by vogje01 on 5/8/26.
//

#ifndef AWSMOCK_QT_UI_MONITORING_CONFIG_H
#define AWSMOCK_QT_UI_MONITORING_CONFIG_H

#pragma once
#include <QString>
#include <QDateTime>
#include <QStringList>

namespace Awsmock::Components {

    class MonitoringConfigBuilder;

    class MonitoringConfig {
    public:
        /**
         * @brief Monitoring AWS region
         */
        QString region;

        /**
         * @brief Chart title
         */
        QString title;

        /**
         * @brief Dashboard row
         */
        int row{};

        /**
         * @brief Dashboard column
         */
        int column{};

        /**
         * @brief Monitoring name
         */
        QString name;

        /**
         * @brief Monitoring seria name
         */
        QString series;

        /**
         * @brief Start timestamp
         */
        QDateTime start;

        /**
         * @brief End timestamp
         */
        QDateTime end;

        /**
         * @brief X-Axis text
         */
        QString xAxisText;

        /**
         * @brief X-Axis format
         */
        QString xAxisFormat;

        /**
         * @brief Y-Axis text
         */
        QString yAxisText;

        /**
         * @brief Y-Axis format
         */
        QString yAxisFormat;

        /**
         * @brief Maximal number of series
         */
        int limit = -1;

        /**
         * @brief stepping in minutes
         */
        int step = 5;

        /**
         * @brief Scaling factor
         */
        int scale = -1;

        /**
         * @brief Seria names array
         */
        QStringList seriesNames;
        using Builder = MonitoringConfigBuilder;
    };

    class MonitoringConfigBuilder {
    public:
        MonitoringConfigBuilder() : _config() {
        }

        MonitoringConfig build() {
            return std::move(_config);
        }

        MonitoringConfigBuilder &setRegion(const QString &v) {
            _config.region = v;
            return *this;
        }

        MonitoringConfigBuilder &setTitle(const QString &v) {
            _config.title = v;
            return *this;
        }

        MonitoringConfigBuilder &setRow(int v) {
            _config.row = v;
            return *this;
        }

        MonitoringConfigBuilder &setColumn(int v) {
            _config.column = v;
            return *this;
        }

        MonitoringConfigBuilder &setName(const QString &v) {
            _config.name = v;
            return *this;
        }

        MonitoringConfigBuilder &setSeries(const QString &v) {
            _config.series = v;
            return *this;
        }

        MonitoringConfigBuilder &setStart(const QDateTime &v) {
            _config.start = v;
            return *this;
        }

        MonitoringConfigBuilder &setEnd(const QDateTime &v) {
            _config.end = v;
            return *this;
        }

        MonitoringConfigBuilder &setXAxisText(const QString &v) {
            _config.xAxisText = v;
            return *this;
        }

        MonitoringConfigBuilder &setXAxisFormat(const QString &v) {
            _config.xAxisFormat = v;
            return *this;
        }

        MonitoringConfigBuilder &setYAxisText(const QString &v) {
            _config.yAxisText = v;
            return *this;
        }

        MonitoringConfigBuilder &setYAxisFormat(const QString &v) {
            _config.yAxisFormat = v;
            return *this;
        }

        MonitoringConfigBuilder &setLimit(int v) {
            _config.limit = v;
            return *this;
        }

        MonitoringConfigBuilder &setStep(int v) {
            _config.step = v;
            return *this;
        }

        MonitoringConfigBuilder &setScale(int v) {
            _config.scale = v;
            return *this;
        }

        MonitoringConfigBuilder &setSeriesNames(const QStringList &v) {
            _config.seriesNames = v;
            return *this;
        }

    private:
        MonitoringConfig _config;
    };

};

#endif //AWSMOCK_QT_UI_MONITORING_CONFIG_H
