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

// AwsMock includes
#include <utils/BaseDialog.h>
#include <utils/IconUtils.h>

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
     * @brief Handle OK button
     */
    void HandleAccept();

    /**
     * @brief Handle CANCEL button
     */
    void HandleReject();

    void SearchFile();

    void ReadData() const;

    void SaveData() const;

    void FindNext() const;

    void FindPrevious() const;

    void ClearSearch() const;

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
    QFile *_currentFile;
};


#endif //AWSMOCK_QT_UI_SHOW_INFRASTRUCTURE_H
