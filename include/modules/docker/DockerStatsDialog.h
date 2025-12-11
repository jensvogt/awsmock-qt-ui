//
// Created by jensv on 08/12/2025.
//

#ifndef AWSMOCK_QT_UI_DOCKER_STATS_DIALOG_H
#define AWSMOCK_QT_UI_DOCKER_STATS_DIALOG_H

#include <QMenu>
#include <QDialog>
#include <QList>

#include <modules/docker/DockerService.h>
#include <utils/IconUtils.h>
#include <utils/BaseDialog.h>
#include <modules/application/ApplicationEditDialog.h>
#include <modules/application/ApplicationUploadCodeDialog.h>
#include <utils/PrefixFilterModel.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class DockerStatsDialog;
}

QT_END_NAMESPACE

class DockerStatsDialog final : public BaseDialog {
    Q_OBJECT

public:
    explicit DockerStatsDialog(QWidget *parent = nullptr);

    ~DockerStatsDialog() override;

    void HandleAccept();

    void HandleReject();

    double GetCpuPercent(const ContainerStat &containerStats);

    void ShowContextMenu(const QPoint &pos);

    void LoadContainers(const DockerContainersResponse &dockerContainersResponse);

    void LoadContainerStatsContent(const DockerStatsResponse &dockerStatsResponse);

    void LoadContent() override;

private:
    /**
     * @brief UI components
     */
    Ui::DockerStatsDialog *_ui;

    /**
     * @brief Container service
     */
    DockerService *_containerService;

    /**
     * @brief Application service
     */
    ApplicationService *_applicationService;

    /**
     * @brief Sort column index
     */
    int _sortColumn = 0;

    /**
     * @brief Sort order
     */
    Qt::SortOrder _sortOrder = Qt::AscendingOrder;

    /**
     * @brief List of container IDs
     */
    QList<QString> _containerIds;

    /**
     * @brief Prefix suche
     */
    QString _prefixValue = "";

    /**
     * @brief Previous CPU total value
     */
    std::map<QString, long> _oldCpuTotal{};

    /**
     * @brief Previous CPU system value
     */
    std::map<QString, long> _oldCpuSystem{};

    QStandardItemModel *_dataModel;
    PrefixFilterProxyModel *_proxyModel;

    QMetaObject::Connection _connection;
};


#endif //AWSMOCK_QT_UI_DOCKER_STATS_DIALOG_H
