#ifndef AWSMOCK_QT_UI_DOCKER_CONTAINER_H
#define AWSMOCK_QT_UI_DOCKER_CONTAINER_H

#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

struct Container {

    /**
     * AWS region
     */
    QString region;

    /**
     * Image ID
     */
    QString id;

    /**
     * Container names
     */
    std::vector<QString> names;

    /**
     * Image
     */
    QString image;

    /**
     * Image ID
     */
    QString imageId;

    /**
     * Command
     */
    QString command;

    /**
     * The state of this container (e.g. Exited)
     */
    //State state;

    /**
     * Additional human-readable status of this container (e.g. Exit 0)
     */
    QString status;

    /**
     * The vcpkg-ports exposed by this container
     */
    //std::vector<ContainerPort> ports{};

    /**
     * The size of files that have been created or changed by this container
     */
    long sizeRw = 0;

    /**
     * The total size of all the files in this container
     */
    long sizeRootFs = 0;

    QString GetPrincipalName() const {
        if (names.empty()) {
            return "Unknown";
        }
        return names.front()[0] == "/" ? names.front().mid(1) : names.front();
    }

    void FromJson(const QJsonObject &jsonObject) {
        region = jsonObject["Region"].toString();
        id = jsonObject["Id"].toString();
        image = jsonObject["Image"].toString();
        imageId = jsonObject["ImageId"].toString();
        command = jsonObject["Command"].toString();
        status = jsonObject["Status"].toString();
        sizeRw = jsonObject["SizeRw"].toInt();
        sizeRootFs = jsonObject["SizeRootFs"].toInt();

        // Environment
        if (jsonObject.contains("Names")) {
            for (auto jsonNames = jsonObject["Names"].toArray(); const auto &name: jsonNames) {
                names.emplace_back(name.toString());
            }
        }
    }
};

#endif // AWSMOCK_QT_UI_DOCKER_CONTAINER_H
