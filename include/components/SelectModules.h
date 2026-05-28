//
// Created by vogje01 on 4/3/26.
//

#pragma once

// QT includes
#include <QButtonGroup>
#include <QGroupBox>
#include <QStandardItemModel>
#include <QWidget>

// Awsmock includes
#include <modules/module/ModuleService.h>
#include <utils/IconUtils.h>

//namespace Awsmock::Components {

QT_BEGIN_NAMESPACE
namespace Ui {
    class SelectModules;
}

QT_END_NAMESPACE

class SelectModules : public QWidget {
    Q_OBJECT

public:
    explicit SelectModules(QWidget *parent = nullptr);

    ~SelectModules() override;

    void SetupListButtons();

    void LoadAvailableModules(const ListModuleNamesResponse &response) const;

    QStringList GetModules();

    ExportType GetExportType() const;

    bool GetPrettyPrint() const;

signals:
    /**
     * @brief Signaled when the module selection changed
     *
     * @param modules module list
     * @param exportType export type
     * @param prettyPrint pretty print flag
     */
    void ModulesSelectSignal(const QStringList &modules, const ExportType &exportType, bool prettyPrint);

    /**
     * @brief Signaled when the pretty print changed
     *
     * @param prettyPrint pretty print state
     */
    void PrettyPrintChangedSignal(bool prettyPrint);

    /**
     * @brief Signaled when the export type changed
     *
     * @param exportType export type
     */
    void ExportTypeChangedSignal(const ExportType &exportType);

private:
    /**
     * @brief UI components
     */
    Ui::SelectModules *_ui;

    /**
     * @brief module service
     */
    ModuleService *_moduleService;

    /**
     * @brief Selected modules
     */
    QStringList _selectedModules;

    /**
     * @brief Data model
     */
    QStandardItemModel *_leftDataModel;

    /**
     * @brief Data model
     */
    QStandardItemModel *_rightDataModel;

    /**
     * @brief Pre
     */
    bool _prettyPrint = true;

    /**
     * @brief Pretty print flag
     */
    bool _includeObjects = false;

    /**
     * @brief export type, either INFRA_STRUCTURE, OBJECTS, or BOTH
     */
    ExportType _exportType = INFRA_STRUCTURE;

    /**
     * @brief Excluded modules
     */
    static QStringList _excluded;

    /**
     * @brief Export type radio button group
     */
    QButtonGroup *_exportTypeGroup;

};

//} // Awsmock::Components

