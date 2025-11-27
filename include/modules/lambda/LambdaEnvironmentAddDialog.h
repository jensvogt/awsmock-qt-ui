//
// Created by vogje01 on 11/27/25.
//

#ifndef AWSMOCK_QT_UI_LAMBDA_ENVIRONMENT_ADD_DIALOG_H
#define AWSMOCK_QT_UI_LAMBDA_ENVIRONMENT_ADD_DIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE

namespace Ui {
    class LambdaEnvironmentAddDialog;
}

QT_END_NAMESPACE

class LambdaEnvironmentAddDialog final : public QDialog {
    Q_OBJECT

public:
    explicit LambdaEnvironmentAddDialog(QWidget *parent = nullptr);

    ~LambdaEnvironmentAddDialog() override;

private:
    Ui::LambdaEnvironmentAddDialog *ui;
};


#endif //AWSMOCK_QT_UI_LAMBDA_ENVIRONMENT_ADD_DIALOG_H
