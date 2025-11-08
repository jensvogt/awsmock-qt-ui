//
// Created by vogje01 on 11/8/25.
//

#ifndef AWSMOCK_QT_UI_INFRASTRUCTURE_SERVICE_H
#define AWSMOCK_QT_UI_INFRASTRUCTURE_SERVICE_H

// Qt includes
#include <QFile>

// Awsmock includes
#include <utils/Configuration.h>
#include <utils/RestManager.h>

class InfraStructureService final : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     */
    InfraStructureService();

    /**
     * @brief Destructor
     */
    ~InfraStructureService() override = default;

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

signals:
    void ImportResponseSignal();

    void ExportResponseSignal(const QString &exportFile, const QString &exportResponse);

    void CleanResponseSignal();

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
#endif //AWSMOCK_QT_UI_INFRASTRUCTURE_SERVICE_H
