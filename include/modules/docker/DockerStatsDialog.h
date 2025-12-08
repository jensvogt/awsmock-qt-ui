//
// Created by jensv on 08/12/2025.
//

#ifndef AWSMOCK_QT_UI_DOCKER_STATS_DIALOG_H
#define AWSMOCK_QT_UI_DOCKER_STATS_DIALOG_H

#include <QDialog>

#include <modules/docker/DockerService.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class DockerStatsDialog;
}

QT_END_NAMESPACE

class DockerStatsDialog final : public QDialog {
    Q_OBJECT

public:
    explicit DockerStatsDialog(QWidget *parent = nullptr);

    ~DockerStatsDialog() override;

    void HandleAccept();

    void HandleReject();

    void LoadContent(const DockerStatsResponse &dockerStatsResponse);

private:
    /**
     * @brief UI components
     */
    Ui::DockerStatsDialog *_ui;

    /**
     * @brief Container service
     */
    DockerService *_containerService;
};


#endif //AWSMOCK_QT_UI_DOCKER_STATS_DIALOG_H
