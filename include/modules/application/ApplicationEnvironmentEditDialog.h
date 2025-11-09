//
// Created by vogje01 on 11/9/25.
//

#ifndef AWSMOCK_QT_UI_APPLICATION_ENVIRONMENT_EDIT_DIALOG_H
#define AWSMOCK_QT_UI_APPLICATION_ENVIRONMENT_EDIT_DIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE

namespace Ui {
    class ApplicationEnvironmentEditDialog;
}

QT_END_NAMESPACE

class ApplicationEnvironmentEditDialog final : public QDialog {
    Q_OBJECT

public:
    explicit ApplicationEnvironmentEditDialog(const QString &key, const QString &value, bool add, QWidget *parent = nullptr);

    ~ApplicationEnvironmentEditDialog() override;

    void HandleAccept();

    void HandleReject();

    QString GetValue();

    QString GetKey();

private:
    Ui::ApplicationEnvironmentEditDialog *_ui;
    QString _key;
    QString _value;
};


#endif //AWSMOCK_QT_UI_APPLICATION_ENVIRONMENT_EDIT_DIALOG_H
