//
// Created by vogje01 on 11/23/25.
//

#ifndef AWSMOCK_QT_UI_APPLICATION_LOGS_DIALOG_H
#define AWSMOCK_QT_UI_APPLICATION_LOGS_DIALOG_H

// C++ includes
#include <utility>

// Qt includes
#include <QDialog>
#include <QScrollBar>
#include <QStandardItemModel>

// Awsmock includes
#include <modules/docker/DockerLogClient.h>
#include <utils/IconUtils.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class ApplicationLogsDialog;
}

QT_END_NAMESPACE

class ApplicationLogsDialog final : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param applicationName name of the application
     * @param containerId docker container ID
     * @param parent parent widget
     */
    explicit ApplicationLogsDialog(const QString &applicationName, const QString &containerId, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~ApplicationLogsDialog() override;

    /**
     * @brief Handle OK button
     */
    void HandleAccept();

    /**
     * @brief Handle CANCEL button
     */
    void HandleReject();

    /**
     * @brief Check the log level
     *
     * @param line log line
     * @return true when log level is above current log level
     */
    [[nodiscard]]
    bool CheckLogLevel(const QString &line) const;

    /**
     * @brief Sends the new log level to the docker application container.
     */
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

    /**
     * @brief Auto scroll flag
     */
    bool _autoScroll = true;
};


#endif //AWSMOCK_QT_UI_APPLICATION_LOGS_DIALOG_H
