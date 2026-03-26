#ifndef AWSMOCK_QT_UI_DOCKER_STATS_RESPONSE_H
#define AWSMOCK_QT_UI_DOCKER_STATS_RESPONSE_H

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <dto/docker/ContainerStat.h>

struct DockerStatsResponse {
    long total{};

    QList<ContainerStat> containerStats;

    void FromJson(const QJsonDocument &jsonDoc) {

        total = jsonDoc.object()["total"].toInt();

        for (QJsonArray jArray = jsonDoc["containerStats"].toArray(); const auto &element: jArray) {
            ContainerStat containerStat;
            containerStat.FromJson(element.toObject());
            containerStats.append(containerStat);
        }
    }
};

#endif // AWSMOCK_QT_UI_DOCKER_STATS_RESPONSE_H
