//
// Created by vogje01 on 11/7/25.
//

#ifndef AWSMOCK_QT_UI_APPLICATION_EDIT_DIALOG_H
#define AWSMOCK_QT_UI_APPLICATION_EDIT_DIALOG_H

#include <QDialog>

#include <utils/IconUtils.h>
#include <utils/BaseDialog.h>
#include <modules/application/ApplicationService.h>
#include <modules/application/ApplicationEnvironmentEditDialog.h>

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

private slots:
    void HandleAccept();

    void HandleReject();

private:
    void UpdateApplication(const ApplicationGetResponse &applicationGetResponse);

    void SetupEnvironmentTab();

    void ShowEnvironmentContextMenu(const QPoint &pos);

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
     * @brief Sort column index
     */
    int _sortColumn = 1;

    /**
     * @brief Sort order
     */
    Qt::SortOrder _sortOrder = Qt::AscendingOrder;
};


#endif // AWSMOCK_QT_UI_APPLICATION_EDIT_DIALOG_H
