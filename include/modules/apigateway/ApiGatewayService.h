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
#include <dto/apigateway/RestApiListResponse.h>
#include <utils/BaseService.h>
#include <utils/RestManager.h>

class ApiGatewayService final : public BaseService {
    Q_OBJECT

public:
    /**
     * @brief ApiGatewayService
     */
    ApiGatewayService() = default;

    /**
     * @brief List applications
     *
     * @param prefix application name prefix
     * @param pageSize
     * @param pageIndex
     * @param sortAttribute
     * @param sortDirection
     */
    void ListRestApis(const QString &prefix, long pageSize, long pageIndex, const QString &sortAttribute, int sortDirection);

signals:
    /**
     * @brief Sent when a REST API list arrived.
     *
     * @param applicationNames
     */
    void ListRestApisSignal(const RestApiListResponse &applicationNames);

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
