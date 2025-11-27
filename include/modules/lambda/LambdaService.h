#ifndef AWSMOCK_QT_UI_LAMBDA_SERVICE_H
#define AWSMOCK_QT_UI_LAMBDA_SERVICE_H

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
#include <dto/lambda/LambdaListResponse.h>
#include <dto/lambda/LambdaGetResponse.h>
#include <dto/lambda/LambdaListEnvironmentResponse.h>
#include <dto/lambda/LambdaListInstancesResponse.h>

class LambdaService final : public QObject {
    Q_OBJECT

public:
    /**
     * @brief LambdaService
     */
    LambdaService();

    /**
     * @brief List lambdas
     *
     * @param prefix lambda name prefix
     */
    void ListLambdas(const QString &prefix);

    /**
     * @brief Upload new lambda code
     *
     * @param request lambda upload request
     */
    //void UploadLambda(const LambdaUploadRequest &request);

    /**
     * @brief Creates new lambda
     *
     * @param request lambda create request
     */
    //void CreateLambda(const LambdaCreateRequest &request);

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

    void GetLambdaLogs(const QString &oid);

    /**
     * @brief Stop an lambdas
     *
     * @param lambdaListResponse
     * @param name lambda name
     */
    //void StartLambda(const QString &name);

    /**
     * @brief Start an lambdas
     *
     * @param name lambda name
     */
    //void StopLambda(const QString &name);

    /**
     * @brief Restart an lambdas
     *
     * @param name lambda name
     */
    //void RestartLambda(const QString &name);

    /**
     * @brief Restart all lambdas
     */
    //void RestartAllLambdas();

    //void RebuildLambda(const QString &name);

    /**
     * @brief Reload the lambda code
     *
     * @param lambdaName lambda name
     * @param version
     * @param lambdaCode
     */
    //void UploadLambdaCode(const QString &lambdaName, const QString &version, const QString &lambdaCode);

    //void ListLambdaNames();

    /**
     * @brief Deletes new lambda
     *
     * @param name lambda name
     */
    void DeleteLambda(const QString &name);

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
     * @brief List environment signal
     *
     * @param listEnvironmentResponse lambda environment list response
     */
    void ListLambdaEnvironmentSignal(const LambdaListEnvironmentResponse &listEnvironmentResponse);

    /**
     * @brief List lambda logs signal
     *
     * @param listEnvironmentResponse lambda environment list response
     */
    void ListLambdaLogsSignal(const LambdaListEnvironmentResponse &listEnvironmentResponse);

    /**
     * @brief Reload all lambdas signal
     */
    void LoadAllLambdas();

    /**
     * @brief Reload all environment signal
     */
    void LoadLambdaEnvironment();

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


#endif // AWSMOCK_QT_UI_LAMBDA_SERVICE_H
