//
// Created by vogje01 on 11/8/25.
//

#ifndef AWSMOCK_QT_UI_MODULE_SERVICE_H
#define AWSMOCK_QT_UI_MODULE_SERVICE_H

// Qt includes
#include <QMessageBox>
#include <QElapsedTimer>

// Awsmock includes
#include <utils/EventBus.h>
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

    /**
     * @brief Get infrafstructure JSON from server
     */
    void GetInfrastructure();

signals:
    /**
     * @brief Handler for import callbacks
     */
    void ImportResponseSignal();

    /**
     * @brief Handler for export callbacks
     *
     * @param exportFile file to export
     * @param exportResponse response from export call
     */
    void ExportResponseSignal(const QString &exportFile, const QString &exportResponse);

    /**
     * @brief Clean response callbacks
     */
    void CleanResponseSignal();

    /**
     * @brief Clean response callbacks
     *
     * @param serverConfig erver config DTO
     */
    void GetServerConfigSignal(const GatewayConfig &serverConfig);

    /**
     * @brief Handler for the get infrastructure callback
     *
     * @param infrastructure infrastructure JSON
     */
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
