//
// Created by vogje01 on 11/23/25.
//

#ifndef AWSMOCK_QT_UI_APPLICATION_LOGS_DIALOG_H
#define AWSMOCK_QT_UI_APPLICATION_LOGS_DIALOG_H

#include <utility>

#include <QDialog>
#include <QScrollBar>
#include <QStandardItemModel>

#include <utils/IconUtils.h>
#include <modules/docker/DockerLogClient.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class ApplicationLogsDialog;
}

QT_END_NAMESPACE

class ApplicationLogsDialog final : public QDialog {
    Q_OBJECT

public:
    explicit ApplicationLogsDialog(const QString &applicationName, const QString &containerId,
                                   QWidget *parent = nullptr);

    ~ApplicationLogsDialog() override;

    void HandleAccept();

    void HandleReject();

    bool CheckLogLevel(const QString &line) const;

    void UpdateLogLevel() const;

private:
    /**
     * @brief UI components
     */
    Ui::ApplicationLogsDialog *_ui;

    /**
     * @brief Docker container ID
     */
    QString _containerId;

    /**
     * @brief Docker cogs client connection
     */
    DockerLogClient *_dockerLogClient;

    /**
     * @brief List model
     */
    QStandardItemModel *_model;

    /**
     * @brief Docker cogs client connection
     */
    QString _currentLogLevel = "INFO";
};


#endif //AWSMOCK_QT_UI_APPLICATION_LOGS_DIALOG_H
