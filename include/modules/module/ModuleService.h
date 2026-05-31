//
// Created by vogje01 on 11/8/25.
//

#ifndef AWSMOCK_QT_UI_MODULE_SERVICE_H
#define AWSMOCK_QT_UI_MODULE_SERVICE_H

// Qt includes
#include <QMessageBox>
#include <QElapsedTimer>

// Awsmock includes
#include <utils/Logging.h>
#include <utils/EventBus.h>
#include <utils/Configuration.h>
#include <utils/RestManager.h>
#include <utils/BaseService.h>
#include <dto/gateway/GatewayConfig.h>
#include <dto/module/ListModuleNamesResponse.h>

enum ExportType {
    INFRA_STRUCTURE,
    OBJECTS,
    BOTH
};


static std::map<ExportType, QString> ExportTypeNames{
    {INFRA_STRUCTURE, "INFRA_STRUCTURE"},
    {OBJECTS, "OBJECTS"},
    {BOTH, "BOTH"},
};

[[maybe_unused]] static QString ExportTypeToString(const ExportType &exportType) {
    return ExportTypeNames[exportType];
}

[[maybe_unused]] static ExportType ExportTypeFromString(const QString &exportType) {
    for (auto &[fst, snd]: ExportTypeNames) {
        if (snd == exportType) {
            return fst;
        }
    }
    return INFRA_STRUCTURE;
}

[[maybe_unused]] static ExportType ExportTypeFromIndex(const int index) {
    if (index < 0 || index >= static_cast<int>(BOTH) + 1) {
        throw logError << "Invalid enum index: " << index;
    }
    return static_cast<ExportType>(index);
}

class ModuleService final : public BaseService {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     */
    ModuleService() = default;

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
     * @brief Exports the given modul
     * @param exportFilename export file name
     * @param modules
     */
    void ExportInfrastructure(const QString &exportFilename, const QStringList &modules);

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
     * @brief Get infrastructure JSON from server
     */
    void GetInfrastructure(const QStringList &modules, const ExportType &exportType, bool prettyPrint);

    /**
     * @brief Send a ping to the server
     */
    void PingServer();

    void SetLogLevel(const QString &logLevel, const QString &logChannel);

    void GetLogLevel();

    void ListModuleNames();

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

    /**
     * @brief Handler for the get loglevel callback
     *
     * @param logLevel logging level
     */
    void GetLoglevelSignal(const QString &logLevel);

    /**
     * @brief Handler for the get module names callback
     *
     * @param response list of module names
     */
    void ListModuleNamesSignal(const ListModuleNamesResponse &response);

private:
    /**
     * @brief HTTP REST manager
     */
    RestManager _restManager;

    /**
     * @brief Base URL
     */
    QUrl url;

    /**
     * @brief Export file
     */
    QFile _exportFile;
};
#endif //AWSMOCK_QT_UI_MODULE_SERVICE_H
