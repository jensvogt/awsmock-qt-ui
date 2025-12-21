//
// Created by vogje01 on 12/21/25.
//

#ifndef AWSMOCK_QT_UI_DYNAMODB_ADD_TABLE_DIALOG_H
#define AWSMOCK_QT_UI_DYNAMODB_ADD_TABLE_DIALOG_H

// Qt includes
#include <QDialog>
#include <QStandardItemModel>

// AwsMock includes
#include <utils/IconUtils.h>
#include <utils/BaseDialog.h>
#include <utils/PrefixFilterModel.h>
#include <modules/dynamodb/DynamoDbService.h>
#include <modules/dynamodb/DynamoDbAddAttributeDialog.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class DynamoDbAddTableDialog;
}

QT_END_NAMESPACE

class DynamoDbAddTableDialog : public BaseDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param parent parent widget
     */
    explicit DynamoDbAddTableDialog(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~DynamoDbAddTableDialog() override;

    /**
     * @brief Load dialog content
     */
    void LoadContent() override;

    /**
     * @brief Setup attribute tab
     */
    void SetupAttributeTab();

    /**
     * @brief Setup key schema tab
     */
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
     * @brief UI  components
     */
    Ui::DynamoDbAddTableDialog *_ui;

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


#endif //AWSMOCK_QT_UI_DYNAMODB_ADD_TABLE_DIALOG_H
