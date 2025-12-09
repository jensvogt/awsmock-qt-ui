//
// Created by jensv on 08/12/2025.
//

#ifndef AWSMOCK_QT_UI_DOCKER_STATS_DIALOG_H
#define AWSMOCK_QT_UI_DOCKER_STATS_DIALOG_H

#include <QDialog>
#include <QList>

#include <modules/docker/DockerService.h>
#include <utils/IconUtils.h>
#include <utils/BaseDialog.h>

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

    void LoadContainerStatsContent(const DockerStatsResponse &dockerStatsResponse) const;

    void LoadContainers(const DockerContainersResponse &dockerContainersResponse);

    void LoadContent() const;

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

};


#endif //AWSMOCK_QT_UI_DOCKER_STATS_DIALOG_H
