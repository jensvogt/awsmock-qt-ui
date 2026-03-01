//
// Created by vogje01 on 1/17/26.
//

#ifndef AWSMOCK_QT_UI_SHOW_INFRASTRUCTURE_H
#define AWSMOCK_QT_UI_SHOW_INFRASTRUCTURE_H

// Qt includes
#include <QFileDialog>
#include <QMessageBox>
#include <QShortcut>
#include <QKeySequence>
#include <QInputDialog>

// AwsMock includes
#include <utils/BaseDialog.h>
#include <utils/IconUtils.h>
#include <utils/DateTimeUtils.h>
#include <modules/module/ModuleService.h>
#include <components/ReplaceWordDialog.h>

#include "components/PlainTextEditor.h"

QT_BEGIN_NAMESPACE

namespace Ui {
    class ShowInfrastructure;
}

QT_END_NAMESPACE

class ShowInfrastructure : public BaseDialog {
    Q_OBJECT

public:
    /**
     * @brief Show the infrastructure file
     *
     * @param parent parent widget
     */
    explicit ShowInfrastructure(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~ShowInfrastructure() override;

    /**
     * @brief Callback for the asynchronous REST call.
     *
     * @param infrastructureJson infrastructure JSON as string
     */
    void HandleGetInfrastructure(const QString &infrastructureJson) const;

    /**
     * @brief Handle OK button
     */
    void HandleAccept();

    /**
     * @brief Handle CANCEL button
     */
    void HandleReject();

    void SearchFile();

    void ReadData() const;

    void ImportData() const;

    void SaveData();

    /**
     * @brief Load content
     */
    void LoadContent() override {
    }

private:
    /**
     * @brief UI components
     */
    Ui::ShowInfrastructure *_ui;

    /**
     * @brief Current file
     */
    QFile *_currentFile{};

    /**
     * @brief Module service
     */
    ModuleService *_moduleService{};

    /**
     * @brief Plaintext edit
     */
    Awsmock::Components::PlainTextEditor *_plainTextEdit{};
};


#endif //AWSMOCK_QT_UI_SHOW_INFRASTRUCTURE_H
