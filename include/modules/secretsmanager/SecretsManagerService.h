#ifndef AWSMOCK_QT_UI_SECRETSMANAGER_SERVICE_H
#define AWSMOCK_QT_UI_SECRETSMANAGER_SERVICE_H

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
#include <utils/BaseService.h>
#include <dto/secretsmanager/SecretsManagerListSecretsResponse.h>

class SecretsManagerService final : public BaseService {
    Q_OBJECT

public:
    /**
     * @brief Secrets Manager Service
     */
    SecretsManagerService() = default;

    /**
     * @brief List all secrets
     *
     */
    void ListSecrets();

signals:
    /**
     * @brief Get secretsManager list signal
     *
     * @param secretsManagerListResponse
     */
    void ReloadSecretsSignal(const SecretsListResponse &secretsManagerListResponse);

    /**
     * @brief Get secretsManager response
     *
     * @param secretsManagerGetResponse secretsManager get response
     */
    //void GetSecretsManagerDetailsSignal(const SecretsManagerGetResponse &secretsManagerGetResponse);

    /**
     * @brief List instances signal
     *
     * @param listInstancesResponse secretsManager instances list response
     */
    //void ListSecretsManagerInstancesSignal(const SecretsManagerListInstancesResponse &listInstancesResponse);

    /**
     * @brief List environment signal
     *
     * @param listEnvironmentResponse secretsManager environment list response
     */
    //void ListSecretsManagerEnvironmentSignal(const SecretsManagerListEnvironmentResponse &listEnvironmentResponse);

    /**
     * @brief List secretsManager results signal
     *
     * @param listResultsResponse secretsManager results list response
     */
    //void ListSecretsManagerResultsSignal(const SecretsManagerListResultsResponse &listResultsResponse);

    /**
     * @brief Get a single secretsManager invocation result
     *
     * @param getResultsResponse secretsManager result reponse
     */
    //void GetSecretsManagerResultSignal(const SecretsManagerGetResultsResponse &getResultsResponse);

    /**
     * @brief Reload all secrets signal
     */
    void LoadAllSecrets();

    /**
     * @brief Reload all environment signal
     */
    //void LoadSecretsManagerEnvironment();

private:
    /**
     * @brief HTTP REST manager
     */
    RestManager _restManager;
};


#endif // AWSMOCK_QT_UI_SECRETSMANAGER_SERVICE_H
