//
// Created by vogje01 on 11/8/25.
//

#ifndef AWSMOCK_QT_UI_MODULE_SERVICE_H
#define AWSMOCK_QT_UI_MODULE_SERVICE_H

// Qt includes
#include <QFile>
#include <QJsonDocument>
#include <QMessageBox>

// Awsmock includes
#include <utils/Configuration.h>
#include <utils/RestManager.h>
#include <dto/gateway/GatewayConfig.h>

class ModuleService final : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     */
    ModuleService();

    /**
     * @brief Destructor
     */
    ~ModuleService() override = default;

    /**
     * @brief Export infrastructure
     *
     * @param exportFilename export file name
     */
    void ExportInfrastructure(const QString &exportFilename);

    /**
     * @brief Import infrastructure
     *
     * @param content export file name
     */
    void ImportInfrastructure(const QString &content);

    /**
     * @brief Clean infrastructure
     */
    void CleanInfrastructure();

    /**
     * @brief Get server config
     *
     * @return server configuration DTO
     */
    void GetServerConfig();

    void GetInfrastructure();

signals:
    void ImportResponseSignal();

    void ExportResponseSignal(const QString &exportFile, const QString &exportResponse);

    void CleanResponseSignal();

    void GetServerConfigSignal(const GatewayConfig &serverConfig);

    void GetInfrastructureSignal(const QString &infrastructure);

private:
    /**
     * @brief HTTP REST manager
     */
    RestManager _restManager;

    /**
     * @brief Base URL
     */
    QUrl url;
    QFile _exportFile;
};
#endif //AWSMOCK_QT_UI_MODULE_SERVICE_H
