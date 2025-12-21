//
// Created by vogje01 on 12/21/25.
//

#ifndef AWSMOCK_QT_UI_DYNAMODB_EDIT_TABLE_DIALOG_H
#define AWSMOCK_QT_UI_DYNAMODB_EDIT_TABLE_DIALOG_H

// Qt includes
#include <QDialog>

// AwsMock includes
#include <utils/BaseDialog.h>
#include <utils/IconUtils.h>
#include <utils/PrefixFilterModel.h>
#include <modules/dynamodb/DynamoDbService.h>
#include <modules/dynamodb/DynamoDbAddAttributeDialog.h>
#include <modules/dynamodb/DynamoDbAddTableDialog.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class DynamoDbEditTableDialog;
}

QT_END_NAMESPACE

class DynamoDbEditTableDialog : public BaseDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param tableName name of the table
     * @param parent parent widget
     */
    explicit DynamoDbEditTableDialog(const QString &tableName, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~DynamoDbEditTableDialog() override;

    /**
     * @brief Load dialog content
     */
    void LoadContent() override;

    void UpdateTable(const DynamoDbDescribeTableResponse &response) const;

    void SetupAttributeTab();

    void SetupKeySchemaTab();

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
    Ui::DynamoDbEditTableDialog *_ui;

    /**
     * @brief table name
     */
    QString _tableName;

    /**
     * @brief REST service handler
     */
    DynamoDbService *_dynamoDbService;

    /**
     *  @brief Attribute table data model
     */
    QStandardItemModel *_attributeDataModel{};

    /**
     * @brief Attribute data proxy model
     */
    PrefixFilterProxyModel *_attributeProxyModel{};

    /**
     *  @brief KeySchema table data model
     */
    QStandardItemModel *_keySchemaDataModel{};

    /**
     * @brief KeySchema data proxy model
     */
    PrefixFilterProxyModel *_keySchemaProxyModel{};
};


#endif //AWSMOCK_QT_UI_DYNAMODB_EDIT_TABLE_DIALOG_H
