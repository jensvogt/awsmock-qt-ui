#ifndef AWSMOCK_QT_UI_DOCKER_CONTAINER_CPU_USAGE_H
#define AWSMOCK_QT_UI_DOCKER_CONTAINER_CPU_USAGE_H

#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

struct CpuUsage {

    /**
     * CPU total usage
     */
    long totalUsage{};

    /**
     * CPU system usage
     */
    long systemUsage;

    /**
     * CPU user usage
     */
    long userUsage;

    void FromJson(const QJsonObject &jsonObject) {
        totalUsage = jsonObject["total_usage"].toInteger();
        systemUsage = jsonObject["usage_in_kernelmode"].toInteger();
        userUsage = jsonObject["usage_in_usermode"].toInteger();
    }
};

#endif // AWSMOCK_QT_UI_DOCKER_CONTAINER_CPU_USAGE_H
