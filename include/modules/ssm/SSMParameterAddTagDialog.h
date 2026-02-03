//
// Created by vogje01 on 2/2/26.
//

#ifndef AWSMOCK_QT_UI_SSM_PARAMETER_ADD_TAG_DIALOG_H
#define AWSMOCK_QT_UI_SSM_PARAMETER_ADD_TAG_DIALOG_H

// Qt includes
#include <QDialog>
#include <QLineEdit>
#include <QDialogButtonBox>

// AwsMOck includes
#include <utils/BaseDialog.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class SSMParameterAddTagDialog;
}

QT_END_NAMESPACE

class SSMParameterAddTagDialog : public BaseDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructure
     *
     * @param parent Parent widget
     */
    explicit SSMParameterAddTagDialog(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~SSMParameterAddTagDialog() override;

    /**
     * @brief Handle OK button
     */
    void HandleAccept();

    /**
     * @brief Handle CANCEL button
     */
    void HandleReject();

    /**
     * @brief Load dialog content
     */
    void LoadContent() override {
    }

    /**
     * @brief Return key
     *
     * @return the current key
     */
    QString GetKey() { return _key; }

    /**
     * @brief Return value
     *
     * @return the current value
     */
    QString GetValue() { return _value; }

private:
    /**
     * @brief UI components
     */
    Ui::SSMParameterAddTagDialog *_ui;

    /**
     * @brief Key
     */
    QString _key;

    /**
     * @brief Value
     */
    QString _value;

    /**
     * @brief changed flag
     */
    bool _changed = false;
};


#endif // AWSMOCK_QT_UI_SSM_PARAMETER_ADD_TAG_DIALOG_H
