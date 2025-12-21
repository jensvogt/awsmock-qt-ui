//
// Created by vogje01 on 12/21/25.
//

#ifndef AWSMOCK_QT_UI_DYNAMODB_ADD_KEY_SCHEMA_DIALOG_H
#define AWSMOCK_QT_UI_DYNAMODB_ADD_KEY_SCHEMA_DIALOG_H

// Qt includes
#include <QDialog>

// AwsMock includes
#include <dto/dynamodb/DynamoDbKeySchema.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class DynamoDbAddKeySchemaDialog;
}

QT_END_NAMESPACE

class DynamoDbAddKeySchemaDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor.
     *
     * @param parent parent widget
     */
    explicit DynamoDbAddKeySchemaDialog(QWidget *parent = nullptr);

    /**
     * @brief Deconstructor.
     */
    ~DynamoDbAddKeySchemaDialog() override;

    /**
     * @brief Return the key schema
     *
     * @return key schema
     */
    DynamoDbKeySchema GetKeySchema() { return _keySchema; }

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
    Ui::DynamoDbAddKeySchemaDialog *_ui;

    /**
     * @brief Attribute
     */
    DynamoDbKeySchema _keySchema;
};


#endif //AWSMOCK_QT_UI_DYNAMODB_ADD_KEY_SCHEMA_DIALOG_H
