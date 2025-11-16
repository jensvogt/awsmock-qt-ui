//
// Created by vogje01 on 10/23/23.
//

#ifndef AWSMOCK_QT_UI_MODULE_GATEWAY_CONFIG_H
#define AWSMOCK_QT_UI_MODULE_GATEWAY_CONFIG_H

#include <QString>

/**
 * Gateway configuration
 *
 * @author jens.vogt\@opitz-consulting.com
 */
struct GatewayConfig {
    /**
     * Default region
     */
    QString region;

    /**
     * Endpoint
     */
    QString endpoint;

    /**
     * Protocol
     */
    QString protocol = "http";

    /**
     * Host
     */
    QString host = "localhost";

    /**
     * Listening address
     */
    QString address = "0.0.0.0";

    /**
     * Port
     */
    int port = 4566;

    /**
     * PID
     */
    int pid;

    /**
     * User ID
     */
    QString user = "none";

    /**
     * Access ID
     */
    QString accessId = "000000000000";

    /**
     * Client ID
     */
    QString clientId = "000000000000";

    /**
     * Access key ID
     */
    QString accessKeyId = "none";

    /**
     * Secret access key
     */
    QString secretAccessKey = "none";

    /**
     * Data directory
     */
    QString dataDir = "/tmp/awsmock/data";

    /**
     * Version
     */
    QString version;

    /**
     * Pretty print
     */
    bool prettyPrint = false;

    /**
     * Database active
     */
    bool databaseActive = false;

    void FromJson(const QJsonObject &jsonObject) {
        region = jsonObject["region"].toString();
        endpoint = jsonObject["endpoint"].toString();
        protocol = jsonObject["protocol"].toString();
        host = jsonObject["host"].toString();
        address = jsonObject["address"].toString();
        port = jsonObject["port"].toInt();
        pid = jsonObject["pid"].toInt();
        version = jsonObject["version"].toString();
    }
};

#endif// AWSMOCK_QT_UI_MODULE_GATEWAY_CONFIG_H
