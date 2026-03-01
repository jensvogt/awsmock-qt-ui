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
    class SSMParameterTagDialog;
}

QT_END_NAMESPACE

class SSMParameterTagDialog : public BaseDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor for a new tag
     *
     * @param parent Parent widget
     */
    explicit SSMParameterTagDialog(QWidget *parent = nullptr);

    /**
     * @brief Constructor for a existing tag
     *
     * @param key tag key
     * @param value tag value
     * @param parent parent widget
     */
    SSMParameterTagDialog(const QString &key, const QString &value, QWidget *parent = nullptr);

    /**
     * @brief Initialize dialog
     */
    void Initialize();

    /**
     * @brief Destructor
     */
    ~SSMParameterTagDialog() override;

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
    Ui::SSMParameterTagDialog *_ui;

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
