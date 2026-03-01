//
// Created by vogje01 on 2/15/26.
//

#ifndef AWSMOCK_QT_UI_COGNITO_ADD_USERPOOL_DIALOG_H
#define AWSMOCK_QT_UI_COGNITO_ADD_USERPOOL_DIALOG_H

// Qt includes
#include <QDialog>

QT_BEGIN_NAMESPACE

namespace Ui {
    class CognitoAddUserpoolDialog;
}

QT_END_NAMESPACE

class CognitoAddUserpoolDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param parent parent widget
     */
    explicit CognitoAddUserpoolDialog(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~CognitoAddUserpoolDialog() override;

    /**
     * @brief Handle OK button
     */
    void HandleAccept();

    /**
     * @brief Handle CANCEL button
     */
    void HandleReject();

    /**
     * @brief Returns the user pool name
     */
    QString GetUserpoolName() {
        return _userpoolName;
    }

private:
    /**
     * @brief UI components
     */
    Ui::CognitoAddUserpoolDialog *_ui;

    /**
     * @brief Userpool name
     */
    QString _userpoolName;
};


#endif //AWSMOCK_QT_UI_COGNITO_ADD_USERPOOL_DIALOG_H
