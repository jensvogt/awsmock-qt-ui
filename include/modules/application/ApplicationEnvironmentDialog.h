//
// Created by vogje01 on 11/9/25.
//

#ifndef AWSMOCK_QT_UI_APPLICATION_ENVIRONMENT_DIALOG_H
#define AWSMOCK_QT_UI_APPLICATION_ENVIRONMENT_DIALOG_H

// Qt includes
#include <QDialog>

QT_BEGIN_NAMESPACE

namespace Ui {
    class ApplicationEnvironmentDialog;
}

QT_END_NAMESPACE

class ApplicationEnvironmentDialog final : public QDialog {
    Q_OBJECT

public:
    explicit ApplicationEnvironmentDialog(QWidget *parent = nullptr);

    ApplicationEnvironmentDialog(const QString &key, const QString &value, QWidget *parent = nullptr);

    ~ApplicationEnvironmentDialog() override;

    void Initialize();

    void HandleAccept();

    void HandleReject();

    QString GetValue();

    QString GetKey();

private:
    Ui::ApplicationEnvironmentDialog *_ui;
    QString _key{};
    QString _value{};
};


#endif //AWSMOCK_QT_UI_APPLICATION_ENVIRONMENT_DIALOG_H
