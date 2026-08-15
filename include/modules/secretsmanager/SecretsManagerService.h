#pragma once

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
#include <utils/Logging.h>
#include <dto/secretsmanager/SecretsManagerListSecretsResponse.h>
#include <dto/secretsmanager/SecretGetResponse.h>
#include <dto/secretsmanager/SecretGetVersionsResponse.h>

class SecretsManagerService final : public BaseService {
    Q_OBJECT

public:
    /**
     * @brief Secrets Manager Service
     */
    SecretsManagerService() {
        setApis({"ListSecrets", "GetSecret", "GetSecretVersions", "CreateSecret", "UpdateSecret", "DeleteSecret", "RotateSecret"});
    }

    /**
     * @brief Creates a new secret in the Secrets Manager.
     *
     * This method creates a new secret with the specified attributes and sends a request to the Secrets Manager
     * service to persist the secret. The method emits relevant signals to update the list of secrets or
     * logs an error if the operation fails.
     *
     * @param secretCounter The secret counter object containing details such as name, secretString, and description.
     */
    void CreateSecret(const SecretCounter &secretCounter);

    /**
     * @brief Lists secrets based on the specified criteria.
     *
     * This method retrieves a list of secrets filtered by a given prefix, sorted by a specific attribute,
     * and paginated according to the specified page size and index.
     *
     * @param prefix The prefix to filter secrets by.
     * @param pageSize The number of secrets to retrieve in a single page.
     * @param pageIndex The page index to retrieve.
     * @param sortAttribute The attribute to sort the secrets by.
     * @param sortDirection The sorting direction, represented as an integer (e.g., ascending or descending).
     */
    void ListSecrets(const QString &prefix, long pageSize, long pageIndex, const QString &sortAttribute, int sortDirection);

    /**
     * @brief Retrieves the details of a specified secret.
     *
     * This method sends a request to the Secrets Manager service to get the details
     * of a secret identified by its unique secretId. Upon receiving the response,
     * it parses the data, emits a signal with the retrieved secret details, and logs any errors
     * in case the request fails.
     *
     * @param secretId The unique identifier of the secret whose details are to be retrieved.
     */
    void GetSecret(const QString &secretId);

    /**
     * @brief Retrieves the versions of a specified secret.
     *
     * This method retrieves all versions of a secret identified by its secretId.
     * It sends a request to the Secrets Manager service for version details, processes the response,
     * and emits the relevant signal with the parsed data or logs an error if the request fails.
     *
     * @param secretId The unique identifier of the secret whose versions are to be retrieved.
     */
    void GetVersions(const QString &secretId);

    /**
     * @brief Updates an existing secret with the provided details.
     *
     * This method sends a request to update the details of a secret
     * specified in the provided SecretCounter object. The request is sent
     * asynchronously, and the response is processed to emit the updated secret details.
     *
     * @param secretCounter Reference to a SecretCounter object containing the secret details to update.
     */
    void UpdateSecret(const SecretCounter &secretCounter);

    /**
     * @brief Deletes a secret from the secrets manager.
     *
     * Sends a request to delete the specified secret, with options to bypass recovery and specify recovery window settings.
     *
     * @param secretId The unique identifier of the secret to be deleted.
     */
    void DeleteSecret(const QString &secretId);

    /**
     * @brief Manually triggers rotation of a secret.
     *
     * Sends a request to rotate the specified secret using its configured rotation Lambda function.
     *
     * @param secretId The unique identifier of the secret to be rotated.
     */
    void RotateSecret(const QString &secretId);

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
     * @brief Get secret versions signal
     *
     * @param secretVersionResponse secrets versions
     */
    void GetSecretsVersionsSignal(const SecretGetVersionResponse &secretVersionResponse);

    /**
     * @brief Reload all secrets signal
     */
    void LoadAllSecrets();

private:
    /**
     * @brief HTTP REST manager
     */
    RestManager _restManager;
};
