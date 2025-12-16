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
#include <dto/secretsmanager/SecretGetResponse.h>

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

    /**
     * @brief Get secret details
     *
     * @param secretId Secret AWS ARN
     */
    void GetSecret(const QString &secretId);

    /**
     * @brief Update the secret
     *
     * @param secretCounter secret counter
     */
    void UpdateSecret(const SecretCounter &secretCounter);

signals:
    /**
     * @brief Get secretsManager list signal
     *
     * @param secretsManagerListResponse
     */
    void ReloadSecretsSignal(const SecretsListResponse &secretsManagerListResponse);

    /**
     * @brief Get secret details signal
     *
     * @param secretCounter secrets details
     */
    void GetSecretsDetailsSignal(const SecretCounter &secretCounter);

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
