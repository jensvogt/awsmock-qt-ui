#ifndef AWSMOCK_QT_UI_DOCKER_CONTAINER_STAT_H
#define AWSMOCK_QT_UI_DOCKER_CONTAINER_STAT_H

#include <QJsonObject>

// AwsMock includes
#include <dto/docker/CpuStat.h>
#include <dto/docker/MemoryStat.h>

#include "State.h"

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
     * The state of this container (e.g. Exited)
     */
    State state;

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
    [[nodiscard]] double GetCpuPercent() const {
        const auto delta = static_cast<double>(cpuStats.cpuUsage.totalUsage - preCpuStats.cpuUsage.totalUsage);
        if (const auto deltaSystem = static_cast<double>(cpuStats.cpuUsage.systemUsage - preCpuStats.cpuUsage.systemUsage); deltaSystem > 0 && delta > 0) {
            return delta / deltaSystem / cpuStats.onlineCpus * 100;
        }
        return 0.0;
    }

    /**
     * @brief Returns the memory usage in MB
     *
     * @return memory usage in MB
     */
    [[nodiscard]] long GetTotalMemory() const {
        return memoryStat.usage / (1024 * 1024);
    }

    /**
     * @brief Returns the memory percentage
     *
     * @return memory percentage
     */
    [[nodiscard]] double GetPercentMemory() const {
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

        // State
        if (jsonObject.contains("state")) {
            state.FromJson(jsonObject["state"].toObject());
        }
    }
};

#endif // AWSMOCK_QT_UI_DOCKER_CONTAINER_STAT_H
