#pragma once

// QT includes
#include <QElapsedTimer>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

// Awsmock includes
#include <dto/apigateway/RestApiGetResponse.h>
#include <dto/apigateway/RestApiKeyCreateRequest.h>
#include <dto/apigateway/RestApiKeyListResponse.h>
#include <dto/apigateway/RestApiKeyUpdateRequest.h>
#include <dto/apigateway/RestApiListResponse.h>
#include <dto/apigateway/RestApiUpdateRequest.h>
#include <dto/apigateway/model/RestApiKey.h>
#include <utils/BaseService.h>
#include <utils/RestManager.h>

#include "dto/apigateway/RestApiCreateRequest.h"

class ApiGatewayService final : public BaseService {
    Q_OBJECT

public:
    /**
     * @brief ApiGatewayService
     */
    ApiGatewayService() {
        setApis({
            "ListRestApis", "GetRestApi", "UpdateRestApi", "DeleteRestApi", "DeleteResource", "ListApiKeys", "GetApiKey", "CreateApiKey", "UpdateApiKey", "DeleteApiKey", "EnableApiKey", "DisableApiKey"
        });
    };

    /**
     * @brief Create a new REST API
     *
     * @param request create request
     */
    void CreateRestApi(const RestApiCreateRequest &request);

    /**
     * @brief List REST APIs
     *
     * @param prefix application name prefix
     * @param pageSize page size
     * @param pageIndex page index
     * @param sortAttribute sort attributes
     * @param sortDirection sort direction
     */
    void ListRestApis(const QString &prefix, long pageSize, long pageIndex, const QString &sortAttribute, int sortDirection);

    /**
     * @brief Get the REST API details
     *
     * @param name API name
     */
    void GetRestApi(const QString &name);

    /**
     * @brief Update the REST API
     *
     * @param request update request
     */
    void UpdateRestApi(const RestApiUpdateRequest &request);

    /**
     * @brief Delete a REST API
     *
     * @param name REST API name
     */
    void DeleteRestApi(const QString &name);

    /**
     * @brief Delete a resource from a REST API
     *
     * @param restApiId REST API name
     * @param resourceId resource ID
     */
    void DeleteResource(const QString &restApiId, const QString &resourceId);

    /**
     * @brief List API keys
     *
     * @param prefix name prefix filter
     * @param pageSize page size
     * @param pageIndex page index
     * @param sortAttribute sort attribute
     * @param sortDirection sort direction
     */
    void ListApiKeys(const QString &prefix, long pageSize, long pageIndex, const QString &sortAttribute, int sortDirection);

    /**
     * @brief Get an API key
     *
     * @param keyId API key ID
     */
    void GetApiKey(const QString &keyId);

    /**
     * @brief Create an API key
     *
     * @param request create request
     */
    void CreateApiKey(const RestApiKeyCreateRequest &request);

    /**
     * @brief Update an API key
     *
     * @param request update request
     */
    void UpdateApiKey(const RestApiKeyUpdateRequest &request);

    /**
     * @brief Delete an API key
     *
     * @param keyId API key ID
     */
    void DeleteApiKey(const QString &keyId);

    /**
     * @brief Enable an API key
     *
     * @param keyId API key ID
     */
    void EnableApiKey(const QString &keyId);

    /**
     * @brief Disable an API key
     *
     * @param keyId API key ID
     */
    void DisableApiKey(const QString &keyId);

signals:
    /**
     * @brief Sent when a REST API list arrived.
     *
     * @param restApiListResponse response from server
     */
    void ListRestApisSignal(const RestApiListResponse &restApiListResponse);

    /**
     * @brief Sent when a REST API get arrived.
     *
     * @param restApiGetResponse response from server
     */
    void GetRestApiSignal(const RestApiGetResponse &restApiGetResponse);

    /**
     * @brief Sent when a REST API list should be reloaded
     */
    void ReloadRestApisSignal();

    /**
     * @brief Sent when an API key was retrieved.
     *
     * @param apiKey API key model
     */
    void GetApiKeySignal(const RestApiKey &apiKey);

    /**
     * @brief Sent when an API key list arrived.
     *
     * @param response response from server
     */
    void ListApiKeysSignal(const RestApiKeyListResponse &response);

    /**
     * @brief Sent when the API key list should be reloaded
     */
    void ReloadApiKeysSignal();

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
