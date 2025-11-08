#ifndef AWSMOCK_QT_UI_APPLICATION_SERVICE_H
#define AWSMOCK_QT_UI_APPLICATION_SERVICE_H

// QT includes
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

// Awsmock Qt includes
#include <utils/Configuration.h>
#include <utils/RestManager.h>
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

    /**
     * @brief Reload the application code
     *
     * @param name application name
     */
    void ReloadApplication(const QString &name);

    /**
     * @brief Deletes new application
     *
     * @param name application name
     */
    void DeleteApplication(const QString &name);

signals:
    void ReloadApplicationsSignal(const ApplicationListResponse &applicationListResponse);
    void GetApplicationDetailsSignal(const ApplicationGetResponse &applicationGetResponse);

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
