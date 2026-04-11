//
// Created by vogje01 on 4/4/26.
//

#ifndef AWSMOCK_QT_UI_DYNAMO_DB_ITEM_DIALOG_H
#define AWSMOCK_QT_UI_DYNAMO_DB_ITEM_DIALOG_H

// Qt dialog
#include <QDialog>
#include <QDialogButtonBox>

// Awsmock includes
#include <utils/BaseDialog.h>
#include <modules/dynamodb/DynamoDbService.h>
#include <dto/dynamodb/DynamoDbGetItemResponse.h>

QT_BEGIN_NAMESPACE
namespace Ui {
    class DynamoDbItemDialog;
}

QT_END_NAMESPACE

class DynamoDbItemDialog : public BaseDialog {
    Q_OBJECT

public:
    explicit DynamoDbItemDialog(QString tableName, QString partitionKey, QString sortKey, QWidget *parent = nullptr);

    ~DynamoDbItemDialog() override;

    void HandleAccept();

    void HandleReject();

    void LoadContent() override;

    void UpdateItem(const DynamoDbGetItemResponse &response) const;

private:
    /**
     * @brief UI components
     */
    Ui::DynamoDbItemDialog *_ui;

    /**
     * @brief REST service
     */
    DynamoDbService *_dynamoDbService;

    /**
     * @brief Name of the table
     */
    QString _tableName;

    /**
     * @brief Partition key
     */
    QString _partitionKey;

    /**
     * @brief Sort key
     */
    QString _sortKey;

    /**
     * @brief Sort key
     */
    DynamoDbItem _item;
};


#endif // AWSMOCK_QT_UI_DYNAMO_DB_ITEM_DIALOG_H
