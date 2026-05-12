#ifndef DASHBOARD_COUNTER_H
#define DASHBOARD_COUNTER_H

// Qt includes
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

// Awsmock includes
#include <modules/dashboard/ChartConfig.h>
#include <components/MonitoringConfig.h>

struct Value {

    QDateTime timestamp;

    double value{};
};

typedef QMap<QString, QVector<Value> > ValueMap;

struct DashboardCounter {

    Awsmock::Components::MonitoringConfig chartConfig;

    ValueMap valueMap;

    void FromJson(const QJsonObject &jsonObject) {

        // Message attributes
        for (const QString &key: jsonObject.keys()) {
            QJsonArray valueArray = jsonObject.value(key).toArray();
            QVector<Value> values;
            for (const auto &value: valueArray) {
                Value v;
                v.timestamp = QDateTime::fromString(value[0].toString(), Qt::ISODate);
                v.value = value[1].toDouble();
                values.emplace_back(v);
            }
            valueMap[key] = values;
        }
    }
};


#endif // DASHBOARD_COUNTER_H
