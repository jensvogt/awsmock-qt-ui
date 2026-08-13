//
// Created by vogje01 on 11/7/25.
//

#ifndef AWSMOCK_QT_UI_APPLICATION_EDIT_DIALOG_H
#define AWSMOCK_QT_UI_APPLICATION_EDIT_DIALOG_H

#include <QDialog>
#include <QMenu>

#include <components/HelpDialog.h>
#include <modules/application/ApplicationDependencyDialog.h>
#include <modules/application/ApplicationEnvironmentDialog.h>
#include <modules/application/ApplicationLogsDialog.h>
#include <modules/application/ApplicationLogsDialog.h>
#include <modules/application/ApplicationService.h>
#include <modules/application/ApplicationTagDialog.h>
#include <utils/BaseDialog.h>
#include <utils/DateTimeUtils.h>
#include <utils/IconUtils.h>
#include <utils/PrefixFilterModel.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class ApplicationEditDialog;
}

QT_END_NAMESPACE

class ApplicationEditDialog final : public BaseDialog {
    Q_OBJECT

public:
    explicit ApplicationEditDialog(const QString &name, QWidget *parent = nullptr);

    ~ApplicationEditDialog() override;

    void LoadContent() override {
    };

private slots:
    void HandleAccept();

    void HandleReject();

private:
    void UpdateApplication(const ApplicationGetResponse &applicationGetResponse);

    void SetupDockerfileTab();

    void SetupEnvironmentTab();

    void SetupTagsTab();

    void SetupDependenciesTab();

    void ShowEnvironmentContextMenu(const QPoint &pos);

    void ShowTagsContextMenu(const QPoint &pos);

    void ShowDependenciesContextMenu(const QPoint &pos);

    /**
     * @brief UI components
     */
    Ui::ApplicationEditDialog *_ui;

    /**
     * @brief Application REST service
     */
    ApplicationService *_applicationService;

    /**
     * @brief Application
     */
    Application _application;

    /**
     * @brief Changed flag
     */
    bool _changed = false;

    /**
     * @brief Set to true once the dialog has been populated for the first time.
     *
     * Start/Stop/Refresh re-fetch the application from the server to update status
     * information. That response must not clobber edits the user already made in the
     * editable fields (Dockerfile, description, version, ports, environment, tags,
     * dependencies), so those are only populated on the initial load.
     */
    bool _initialized = false;

    /**
     * @brief Application logs dialog
     */
    ApplicationLogsDialog *_applicationLogsDialog;

    /**
     *  @brief Table data model
     */
    QStandardItemModel *_envDataModel{};

    /**
     * @brief Data proxy model
     */
    PrefixFilterProxyModel *_envProxyModel{};

    /**
     * @brief Sort column index environment table
     */
    int _envSortColumn = 0;

    /**
     * @brief Sort order environment table
     */
    Qt::SortOrder _envSortOrder = Qt::AscendingOrder;

    /**
     * @brief Sort column index tags table
     */
    int _tagSortColumn = 0;

    /**
     * @brief Sort order tag table
     */
    Qt::SortOrder _tagSortOrder = Qt::AscendingOrder;

    /**
     * @brief Container ID
     */
    QString _containerId;
};


#endif // AWSMOCK_QT_UI_APPLICATION_EDIT_DIALOG_H
