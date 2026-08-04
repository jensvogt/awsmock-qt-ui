//
// Created by jensv on 08/12/2025.
//

#pragma once

// Qt includes
#include <QDialog>
#include <QList>
#include <QMenu>

// Awsmock includes
#include <components/PageableTable.h>
#include <modules/application/ApplicationEditDialog.h>
#include <modules/application/ApplicationUploadCodeDialog.h>
#include <modules/docker/DockerService.h>
#include <utils/BaseDialog.h>
#include <utils/IconUtils.h>
#include <utils/StringUtils.h>
#include <utils/PrefixFilterModel.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class DockerStatsDialog;
}

QT_END_NAMESPACE

class DockerStatsDialog final : public BaseDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param parent parent widget
     */
    explicit DockerStatsDialog(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~DockerStatsDialog() override;

    /**
     * @brief Handle OK button
     */
    void HandleAccept();

    /**
     * @brief Handle CANCEL button
     */
    void HandleReject();

    /**
     * @brief Returns the CPU value
     *
     * @param containerStats container statistics
     * @return CPU percentage
     */
    double GetCpuPercent(const ContainerStat &containerStats);

    /**
     * @brief Show the context menu
     *
     * @param pos table position
     */
    void ShowContextMenu(const QPoint &pos) const;

    /**
     * @brief Load the container statistics
     *
     * @param dockerStatsResponse container statistics
     */
    void LoadContainerStatsContent(const DockerStatsResponse &dockerStatsResponse);

    /**
     * @brief Load content
     */
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

    /**
     *  @brief Table data model
     */
    QStandardItemModel *_dataModel{};

    /**
     * @brief Data proxy model
     */
    PrefixFilterProxyModel *_proxyModel{};
};
