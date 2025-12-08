#ifndef AWSMOCK_QT_UI_DOCKER_SERVICE_H
#define AWSMOCK_QT_UI_DOCKER_SERVICE_H

// QT includes
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QObject>
#include <QElapsedTimer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

// Awsmock Qt includes
#include <utils/Configuration.h>
#include <utils/RestManager.h>
#include <utils/EventBus.h>
#include <dto/docker/DockerStatsResponse.h>

class DockerService final : public QObject {
    Q_OBJECT

public:
    /**
     * @brief DockerService
     */
    DockerService();

    /**
     * @brief List docker statistics
     *
     * @param prefix application name prefix
     */
    void ListDockerStats(const QString &prefix);

signals:
    void ReloadDockerStatsSignal(const DockerStatsResponse &dockerStatsResponse);

    //    void GetApplicationDetailsSignal(const ApplicationGetResponse &applicationGetResponse);

    //    void ListApplicationNamedSignal(const QStringList &applicationNames);

    //    void LoadAllApplications();

private:
    /**
     * @brief HTTP REST manager
     */
    RestManager _restManager;

    /**
     * @brief Base URL
     */
    QUrl url;
};


#endif // AWSMOCK_QT_UI_DOCKER_SERVICE_H
