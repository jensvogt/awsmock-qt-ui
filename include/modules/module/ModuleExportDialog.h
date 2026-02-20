//
// Created by jensv on 18/02/2026.
//

#ifndef AWSMOCK_QT_UI_MODULE_EXPORT_DIALOG_H
#define AWSMOCK_QT_UI_MODULE_EXPORT_DIALOG_H

// Qt includes
#include <QDialog>
#include <QFileDialog>
#include <QDialogButtonBox>

// Awsmock in
#include <utils/IconUtils.h>
#include <utils/BaseDialog.h>
#include <modules/module/ModuleService.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class ModuleExportDialog;
}

QT_END_NAMESPACE

class ModuleExportDialog : public BaseDialog {
    Q_OBJECT

public:
    explicit ModuleExportDialog(QWidget *parent = nullptr);

    ~ModuleExportDialog() override;

    void LoadContent() override;

    void LoadAvailableModules(const ListModuleNamesResponse &response) const;

    void SetupListButtons();

    void HandleBrowseButton();

    void WriteInfrastructureFile(const QString &infrastructure) const;

    QString GetFilePath();

    QStringList GetModules();

    void HandleAccept();

    void HandleReject();

private:
    /**
     * @brief UI components
     */
    Ui::ModuleExportDialog *_ui;

    /**
     * @brief module service
     */
    ModuleService *_moduleService;

    /**
     * @brief Data model
     */
    QStandardItemModel *_leftDataModel;

    /**
     * @brief Data model
     */
    QStandardItemModel *_rightDataModel;

    /**
     * @brief Export filename
     */
    QString _exportFilePath;

    /**
     * @brief Selected modules
     */
    QStringList _selectedModules;

    /**
     * @brief Pre
     */
    bool _prettyPrint = true;

    /**
     * @brief Pretty print flag
     */
    bool _includeObjects = false;

    /**
     * @brief Excluded modules
     */
    static QStringList _excluded;
};


#endif //AWSMOCK_QT_UI_MODULE_EXPORT_DIALOG_H
