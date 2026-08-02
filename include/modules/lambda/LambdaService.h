#pragma once

// Qt includes
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QObject>
#include <QElapsedTimer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

// Awsmock includes
#include <utils/Logging.h>
#include <utils/Configuration.h>
#include <utils/RestManager.h>
#include <utils/EventBus.h>
#include <utils/BaseService.h>
#include <dto/lambda/LambdaListResponse.h>
#include <dto/lambda/LambdaGetResponse.h>
#include <dto/lambda/LambdaListEnvironmentResponse.h>
#include <dto/lambda/LambdaListInstancesResponse.h>
#include <dto/lambda/LambdaListResultsResponse.h>
#include <dto/lambda/LambdaGetResultResponse.h>
#include <dto/lambda/LambdaUploadRequest.h>
#include <dto/lambda/LambdaListArnsResponse.h>

#include "dto/lambda/LambdaGetInstanceResponse.h"

class LambdaService final : public BaseService {
    Q_OBJECT

public:
    /**
     * @brief LambdaService
     */
    LambdaService() {
        setApis({
            "ListLambdas", "GetLambda", "GetLambdaInstances", "GetLambdaEnvironment", "AddLambdaEnvironment", "RemoveLambdaEnvironment",
            "ListLambdaLogs", "GetLambdaResult", "GetLambdaResults", "UploadLambdaCode", "UpdateLambda", "UpdateLambdaEnvironment", "RebuildLambda",
            "ListLambdaArns", "StartInstance", "StopInstance", "DeleteLambda", "DeleteLambdaResults", "AddEventSource"
        });
    }

    /**
     * @brief List lambdas
     *
     * @param prefix lambda name prefix
     * @param pageSize
     * @param pageIndex
     * @param sortAttribute
     * @param sortDirection
     */
    void ListLambdas(const QString &prefix, long pageSize, long pageIndex, const QString &sortAttribute, int sortDirection);

    /**
     * @brief Gets a lambda
     *
     * @param lambdaArn lambda ARN
     */
    void GetLambda(const QString &lambdaArn);

    /**
     * @brief Get the lambda instances list
     *
     * @param lambdaArn lambda AWS ARN
     */
    void GetLambdaInstances(const QString &lambdaArn);

    /**
     * @brief Get a single instance
     *
     * @param lambdaArn lambda AWS ARN
     * @param instanceId lambda instance ID
     */
    void GetLambdaInstance(const QString &lambdaArn, const QString &instanceId);

    /**
     * @brief Get the lambda environment list
     *
     * @param lambdaArn lambda AWS ARN
     */
    void GetLambdaEnvironment(const QString &lambdaArn);

    /**
     * @brief Add a lambda environment variable
     *
     * @param lambdaArn lambda AWS ARN
     * @param key environment key
     * @param value environment value
     */
    void AddLambdaEnvironment(const QString &lambdaArn, const QString &key, const QString &value);

    /**
     * @brief Remove a lambda environment variable
     *
     * @param lambdaArn lambda AWS ARN
     * @param key environment key
     */
    void RemoveLambdaEnvironment(const QString &lambdaArn, const QString &key);

    /**
     * @brief List lambda execution logs
     *
     * @param lambdaArn lambda AWS ARN
     */
    void ListLambdaLogs(const QString &lambdaArn);

    /**
     * @brief Get the lambda execution logs
     *
     * @param oid lambda OID
     */
    void GetLambdaResult(const QString &oid);

    /**
     * @brief Get a list of lambda results
     *
     * @param oid lambda oid
     */
    void GetLambdaResults(const QString &oid);

    /**
     * @brief Update the lambda function code
     *
     * @param request lambda update code request
     */
    void UploadLambdaCode(const LambdaUploadRequest &request);

    /**
     * @brief Update the lambda function
     *
     * @param lambdaArn lambda AWS ARN
     * @param enabled enabled flag
     */
    void UpdateLambda(const QString &lambdaArn, bool enabled);

    /**
     *
     * @param lambdaArn lambda AWS ARN
     * @param key environment variable key
     * @param value environment variable value
     */
    void UpdateLambdaEnvironment(const QString &lambdaArn, const QString &key, const QString &value);

    /**
     * @brief Rebuild lambda
     *
     * @param name lambda name
     * @param version lambda version
     */
    void RebuildLambda(const QString &name, const QString &version);

    /**
     * @brief Return a list of lambda ARNs
     */
    void ListLambdaArns();

    /**
     * @brief Start a lambda instance
     *
     * @param lambdaArn lambda AWS ARN
     */
    void StartInstance(const QString &lambdaArn);

    /**
     * @brief Kill a lambda docker image
     *
     * @param lambdaArn dockewr container ID
     * @param instanceId
     * @param instanceId
     */
    void StopInstance(const QString &lambdaArn, const QString &instanceId);

    /**
     * @brief Deletes a lambda function
     *
     * @param name lambda name
     */
    void DeleteLambda(const QString &name);

    /**
     * @brief Delete lambda results
     *
     * @param lambdaArn lambda ARN
     */
    void DeleteLambdaResults(const QString &lambdaArn);

    /**
     * @brief Add an event source mapping to a lambda function
     *
     * @param type event source type, e.g. 'SQS', 'S3', 'SNS'
     * @param functionArn lambda function ARN
     * @param eventSourceArn ARN of the event source, e.g. the SQS queue ARN
     * @param enabled enabled flag
     */
    void AddEventSource(const QString &type, const QString &functionArn, const QString &eventSourceArn, bool enabled);

signals:
    /**
     * @brief Get lambda list signal
     *
     * @param lambdaListResponse
     */
    void ReloadLambdasSignal(const LambdaListResponse &lambdaListResponse);

    /**
     * @brief Get lambda response
     *
     * @param lambdaGetResponse lambda get response
     */
    void GetLambdaDetailsSignal(const LambdaGetResponse &lambdaGetResponse);

    /**
     * @brief List instances signal
     *
     * @param listInstancesResponse lambda instances list response
     */
    void ListLambdaInstancesSignal(const LambdaListInstancesResponse &listInstancesResponse);

    /**
     * @brief Get instance signal
     *
     * @param getInstanceResponse lambda get instance response
     */
    void GetLambdaInstanceSignal(const LambdaGetInstanceResponse &getInstanceResponse);

    /**
     * @brief List environment signal
     *
     * @param listEnvironmentResponse lambda environment list response
     */
    void ListLambdaEnvironmentSignal(const LambdaListEnvironmentResponse &listEnvironmentResponse);

    /**
     * @brief List lambda results signal
     *
     * @param listResultsResponse lambda results list response
     */
    void ListLambdaResultsSignal(const LambdaListResultsResponse &listResultsResponse);

    /**
     * @brief Get a single lambda invocation result
     *
     * @param getResultsResponse lambda result reponse
     */
    void GetLambdaResultSignal(const LambdaGetResultsResponse &getResultsResponse);

    /**
     * @brief Reload all lambdas signal
     */
    void ReloadLambdas();

    /**
     * @brief Reload all environment signal
     */
    void LoadLambdaEnvironment();

    /**
     * @brief List lambda ARNs signal
     *
     * @param listArnsResponse list of lambda ARNs
     */
    void ListLambdaArnsSignal(const LambdaListArnsResponse &listArnsResponse);

    /**
     * @brief Reload lambdas details signal
     */
    void ReloadLambdaDetails();

    /**
     * @brief Reload lambda instance list signal
     */
    void ReloadLambdaInstances();

    /**
     * @brief Event source added signal
     */
    void EventSourceAddedSignal();

private:
    /**
     * @brief HTTP REST manager
     */
    RestManager _restManager;
};
