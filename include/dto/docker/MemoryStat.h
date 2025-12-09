#ifndef AWSMOCK_QT_UI_DOCKER_CONTAINER_MEMORY_STAT_H
#define AWSMOCK_QT_UI_DOCKER_CONTAINER_MEMORY_STAT_H

#include <QList>
#include <QJsonArray>
#include <QJsonObject>

// AwsMock includes
#include <dto/docker/CpuUsage.h>

struct MemoryStat {

    /**
     * Memory usage
     */
    long limit{};

    /**
     * Online CPUs
     */
    long usage{};

    void FromJson(const QJsonObject &jsonObject) {

        limit = jsonObject["limit"].toInteger();
        usage = jsonObject["usage"].toInteger();
    }
};

#endif // AWSMOCK_QT_UI_DOCKER_CONTAINER_MEMORY_STAT_H
