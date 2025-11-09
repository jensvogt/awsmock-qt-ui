//
// Created by vogje01 on 11/9/25.
//

#ifndef AWSMOCK_QT_UI_APPLICATION_DEPENDENCY_DIALOG_H
#define AWSMOCK_QT_UI_APPLICATION_DEPENDENCY_DIALOG_H

#include <QDialog>

#include <modules/application/ApplicationService.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class ApplicationDependencyDialog;
}

QT_END_NAMESPACE

class ApplicationDependencyDialog : public QDialog {
    Q_OBJECT

public:
    explicit ApplicationDependencyDialog(bool add, QWidget *parent = nullptr);

    ~ApplicationDependencyDialog() override;

    QString GetName();

private slots:
    void HandleAccept();

    void HandleReject();

private:
    /**
     * @brief UI Components
     */
    Ui::ApplicationDependencyDialog *_ui;

    /**
     * @brief Application REST service
     */
    ApplicationService *_applicationService;

    /**
     * @brief Application name
     */
    QString _name;
};


#endif //AWSMOCK_QT_UI_APPLICATION_DEPENDENCY_DIALOG_H
