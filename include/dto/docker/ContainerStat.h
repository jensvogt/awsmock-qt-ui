#ifndef AWSMOCK_QT_UI_DOCKER_CONTAINER_STAT_H
#define AWSMOCK_QT_UI_DOCKER_CONTAINER_STAT_H

#include <QJsonObject>

// AwsMock includes
#include <dto/docker/CpuStat.h>
#include <dto/docker/MemoryStat.h>

struct ContainerStat {

    /**
     * Container ID
     */
    QString containerId;

    /**
     * Container name
     */
    QString name;

    /**
     * OS type
     */
    QString osType;

    /**
     * Number of processors
     */
    int numProcs;

    /**
     * CPU statistics
     */
    CpuStat cpuStats;

    /**
     * Previous CPU statistics
     */
    CpuStat preCpuStats;

    /**
     * Previous CPU statistics
     */
    MemoryStat memoryStat;

    /**
     * @brief Returns the CPU percentage
     *
     * @return CPU percentage
     */
    double GetCpuPercent() const {
        double cpuPercent = 0.0;
        const double delta = static_cast<double>(cpuStats.cpuUsage.totalUsage - preCpuStats.cpuUsage.totalUsage);
        const double deltaSystem = static_cast<double>(cpuStats.cpuUsage.systemUsage - preCpuStats.cpuUsage.systemUsage);
        if (deltaSystem > 0 && delta > 0) {
            cpuPercent = delta / deltaSystem / cpuStats.onlineCpus * 100;
        } else {
            cpuPercent = 0.0;
        }
        return cpuPercent;
    }

    /**
     * @brief Returns the memory usage in MB
     *
     * @return memory usage in MB
     */
    long GetTotalMemory() const {
        return memoryStat.usage / (1024 * 1024);
    }

    /**
     * @brief Returns the memory percentage
     *
     * @return memory percentage
     */
    double GetPercentMemory() const {
        return memoryStat.limit > 0 ? static_cast<double>(memoryStat.usage) / static_cast<double>(memoryStat.limit) * 100.0 : 0.0;
    }

    /**
     * @brief Deserialization
     *
     * @param jsonObject JSON object
     */
    void FromJson(const QJsonObject &jsonObject) {
        containerId = jsonObject["id"].toString();
        name = jsonObject["name"].toString();
        osType = jsonObject["os_type"].toString();
        numProcs = jsonObject["num_procs"].toInt();

        // CPU statistics
        if (jsonObject.contains("cpu_stats")) {
            cpuStats.FromJson(jsonObject["cpu_stats"].toObject());
        }

        // Previous CPU statistics
        if (jsonObject.contains("precpu_stats")) {
            preCpuStats.FromJson(jsonObject["precpu_stats"].toObject());
        }

        // Memory
        if (jsonObject.contains("memory_stats")) {
            memoryStat.FromJson(jsonObject["memory_stats"].toObject());
        }
    }
};

#endif // AWSMOCK_QT_UI_DOCKER_CONTAINER_STAT_H
