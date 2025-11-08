//
// Created by vogje01 on 11/7/25.
//

#ifndef AWSMOCK_QT_UI_APPLICATION_EDIT_DIALOG_H
#define AWSMOCK_QT_UI_APPLICATION_EDIT_DIALOG_H

#include <QDialog>

#include <modules/application/ApplicationService.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class ApplicationEditDialog;
}

QT_END_NAMESPACE

class ApplicationEditDialog final : public QDialog {
    Q_OBJECT

public:
    explicit ApplicationEditDialog(const QString &name,QWidget *parent = nullptr);

    ~ApplicationEditDialog() override;

private slots:
    void HandleAccept();
    void HandleReject();

private:

    void UpdateApplication(const ApplicationGetResponse &applicationGetResponse);

    Ui::ApplicationEditDialog *_ui;
    ApplicationService *_applicationService;
    bool _changed = false;
};


#endif // AWSMOCK_QT_UI_APPLICATION_EDIT_DIALOG_H