#ifndef AWSMOCK_QT_UI_DOCKER_CONTAINER_CPU_STAT_H
#define AWSMOCK_QT_UI_DOCKER_CONTAINER_CPU_STAT_H

#include <QList>
#include <QJsonArray>
#include <QJsonObject>

// AwsMock includes
#include <dto/docker/CpuUsage.h>

struct CpuStat {

    /**
     * CPU usage
     */
    CpuUsage cpuUsage;

    /**
     * Online CPUs
     */
    int onlineCpus;

    void FromJson(const QJsonObject &jsonObject) {

        cpuUsage.FromJson(jsonObject["cpu_usage"].toObject());
        onlineCpus = jsonObject["online_cpus"].toInt();
    }
};

#endif // AWSMOCK_QT_UI_DOCKER_CONTAINER_CPU_STAT_H
