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
    /**
     * @brief Environment add dialog constructor
     *
     * @param parent parent widget
     */
    explicit LambdaEnvironmentDetailDialog(QWidget *parent = nullptr);

    /**
     * @brief Environment edit dialog constructor
     *
     * @param key environment variable key
     * @param value environment variable value
     * @param parent  parent widget
     */
    LambdaEnvironmentDetailDialog(const QString &key, const QString &value, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~LambdaEnvironmentDetailDialog() override;

    void Initialize();

    /**
     * @brief Accept dialog
     */
    void HandleAccept();

    /**
     * @brief Reject dialog
     */
    void HandleReject();

    /**
     * @brief Environment key name
     */
    QString GetKey();

    /**
     * @brief Environment key value
     */
    QString GetValue();

private:
    /**
     * @brief UI component
     */
    Ui::LambdaEnvironmentDetailDialog *_ui;

    /**
     * @brief Environment key
     */
    QString _key;

    /**
     * @brief Environment value
     */
    QString _value;
};


#endif //AWSMOCK_QT_UI_LAMBDA_ENVIRONMENT_DETAIL_DIALOG_H
