//
// Created by vogje01 on 11/27/25.
//

#ifndef AWSMOCK_QT_UI_LAMBDA_ENVIRONMENT_DETAIL_DIALOG_H
#define AWSMOCK_QT_UI_LAMBDA_ENVIRONMENT_DETAIL_DIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE

namespace Ui {
    class LambdaEnvironmentDetailDialog;
}

QT_END_NAMESPACE

class LambdaEnvironmentDetailDialog final : public QDialog {
    Q_OBJECT

public:
    explicit LambdaEnvironmentDetailDialog(const QString &key, const QString &value, QWidget *parent = nullptr);

    ~LambdaEnvironmentDetailDialog() override;

    void HandleAccept();

    void HandleReject();

    QString GetKey();

    QString GetValue();

private:
    Ui::LambdaEnvironmentDetailDialog *_ui;
    QString _key;
    QString _value;
};


#endif //AWSMOCK_QT_UI_LAMBDA_ENVIRONMENT_DETAIL_DIALOG_H
