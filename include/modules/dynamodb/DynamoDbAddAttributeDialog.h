//
// Created by vogje01 on 12/21/25.
//

#ifndef AWSMOCK_QT_UI_DYNAMODB_ADD_ATTRIBUTE_DIALOG_H
#define AWSMOCK_QT_UI_DYNAMODB_ADD_ATTRIBUTE_DIALOG_H

// Qt includes
#include <QDialog>

// AwsMock includes
#include <dto/dynamodb/DynamoDbAttribute.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class DynamoDbAddAttributeDialog;
}

QT_END_NAMESPACE

class DynamoDbAddAttributeDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor.
     *
     * @param parent parent widget
     */
    explicit DynamoDbAddAttributeDialog(QWidget *parent = nullptr);

    /**
     * @brief Deconstructor.
     */
    ~DynamoDbAddAttributeDialog() override;

    /**
     * @brief Return the attribute
     *
     * @return attribute
     */
    DynamoDbAttribute GetAttribute() { return _attribute; }

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
    Ui::DynamoDbAddAttributeDialog *_ui;

    /**
     * @brief Attribute
     */
    DynamoDbAttribute _attribute;
};


#endif //AWSMOCK_QT_UI_DYNAMODB_ADD_ATTRIBUTE_DIALOG_H
