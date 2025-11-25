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
//#include <dto/lambda/LambdaUploadRequest.h>
//#include <dto/lambda/LambdaCreateRequest.h>
//#include <dto/lambda/LambdaGetResponse.h>

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
     * @brief Gets an lambda
     *
     * @param name lambda name
     */
    //void GetLambda(const QString &name);

    //void UpdateLambda(const Lambda &lambda);

    //void EnableLambda(const QString &name);

    //void DisableLambda(const QString &name);

    /**
     * @brief Stop an lambdas
     *
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
    //void DeleteLambda(const QString &name);

signals:
    void ReloadLambdasSignal(const LambdaListResponse &lambdaListResponse);

    //void GetLambdaDetailsSignal(const LambdaGetResponse &lambdaGetResponse);

    void ListLambdaNamedSignal(const QStringList &lambdaNames);

    void LoadAllLambdas();

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
