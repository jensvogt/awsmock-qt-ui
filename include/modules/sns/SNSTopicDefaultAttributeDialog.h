//
// Created by jensv on 19/02/2026.
//

#ifndef AWSMOCK_QT_UI_SNS_TOPIC_DEFAULT_ATTRIBUTE_DIALOG_H
#define AWSMOCK_QT_UI_SNS_TOPIC_DEFAULT_ATTRIBUTE_DIALOG_H

// Qt includes
#include <QDialog>
#include <QMessageBox>
#include <QDialogButtonBox>


QT_BEGIN_NAMESPACE

namespace Ui {
    class SNSTopicDefaultAttributeDialog;
}

QT_END_NAMESPACE

class SNSTopicDefaultAttributeDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Adds a new default attribute
     *
     * @param parent parent widget
     */
    explicit SNSTopicDefaultAttributeDialog(QWidget *parent = nullptr);

    /**
     * @brief Edit an existing default attribute
     *
     * @param parent parent widget
     */
    explicit SNSTopicDefaultAttributeDialog(const QString &key, const QString &value, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~SNSTopicDefaultAttributeDialog() override;

    /**
     * @brief UI component initialization
     */
    void Initialize();

    /**
     * @brief Returns the default attribute key
     *
     * @return default attribute key
     */
    QString GetKey() {
        return _key;
    }

    /**
     * @brief Returns the default attribute value
     *
     * @return default attribute value
     */
    QString GetValue() {
        return _value;
    }

    /**
     * @brief Handle OK button
     */
    void HandleAccept();

    /**
     * @brief Handle CANCEL button
     */
    void HandleReject();

private:
    /**
     * @brief UI components
     */
    Ui::SNSTopicDefaultAttributeDialog *_ui;

    /**
     * @brief Attribute key
     */
    QString _key{};

    /**
     * @brief Attribute value
     */
    QString _value{};
};


#endif // AWSMOCK_QT_UI_SNS_TOPIC_DEFAULT_ATTRIBUTE_DIALOG_H
