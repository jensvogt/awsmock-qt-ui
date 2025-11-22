#ifndef AWSMOCK_QT_UI_APPLICATION_SERVICE_H
#define AWSMOCK_QT_UI_APPLICATION_SERVICE_H

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
#include <dto/application/ApplicationListResponse.h>
#include <dto/application/ApplicationUploadRequest.h>
#include <dto/application/ApplicationCreateRequest.h>
#include <dto/application/ApplicationGetResponse.h>

class ApplicationService final : public QObject {
    Q_OBJECT

public:
    /**
     * @brief ApplicationService
     */
    ApplicationService();

    /**
     * @brief List applications
     *
     * @param prefix application name prefix
     */
    void ListApplications(const QString &prefix);

    /**
     * @brief Upload new application code
     *
     * @param request application upload request
     */
    void UploadApplication(const ApplicationUploadRequest &request);

    /**
     * @brief Creates new application
     *
     * @param request application create request
     */
    void CreateApplication(const ApplicationCreateRequest &request);

    /**
     * @brief Gets an application
     *
     * @param name application name
     */
    void GetApplication(const QString &name);

    void UpdateApplication(const Application &application);

    void EnableApplication(const QString &name);

    void DisableApplication(const QString &name);

    /**
     * @brief Stop an applications
     *
     * @param name application name
     */
    void StartApplication(const QString &name);

    /**
     * @brief Start an applications
     *
     * @param name application name
     */
    void StopApplication(const QString &name);

    /**
     * @brief Restart an applications
     *
     * @param name application name
     */
    void RestartApplication(const QString &name);

    /**
     * @brief Restart all applications
     */
    void RestartAllApplications();

    void RebuildApplication(const QString &name);

    /**
     * @brief Reload the application code
     *
     * @param applicationName application name
     * @param version
     * @param applicationCode
     */
    void UploadApplicationCode(const QString &applicationName, const QString &version, const QString &applicationCode);

    void ListApplicationNames();

    /**
     * @brief Deletes new application
     *
     * @param name application name
     */
    void DeleteApplication(const QString &name);

signals:
    void ReloadApplicationsSignal(const ApplicationListResponse &applicationListResponse);

    void GetApplicationDetailsSignal(const ApplicationGetResponse &applicationGetResponse);

    void ListApplicationNamedSignal(const QStringList &applicationNames);

    void LoadAllApplications();

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


#endif // AWSMOCK_QT_UI_APPLICATION_SERVICE_H
