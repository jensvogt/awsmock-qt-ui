#ifndef AWSMOCK_QT_UI_DOCKER_CONTAINERS_RESPONSE_H
#define AWSMOCK_QT_UI_DOCKER_CONTAINERS_RESPONSE_H

#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <dto/docker/Container.h>

struct DockerContainersResponse {
    long total{};

    QList<Container> containers;

    void FromJson(const QJsonDocument &jsonDoc) {
        for (QJsonArray jArray = jsonDoc["containers"].toArray(); const auto &element: jArray) {
            Container container;
            container.FromJson(element.toObject());
            containers.append(container);
        }
    }
};

#endif // AWSMOCK_QT_UI_DOCKER_CONTAINERS_RESPONSE_H
