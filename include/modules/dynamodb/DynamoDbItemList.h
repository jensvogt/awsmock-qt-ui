#ifndef AWSMOCK_QT_UI_DYNAMODB_ITEM_LIST_H
#define AWSMOCK_QT_UI_DYNAMODB_ITEM_LIST_H

// Qt includes
#include <QLabel>
#include <QMenu>
#include <QHBoxLayout>
#include <QPushButton>

// AwsMock includes
#include <utils/BasePage.h>
#include <utils/IconUtils.h>
#include <utils/DateTimeUtils.h>
#include <utils/PrefixFilterModel.h>
#include <modules/dynamodb/DynamoDbService.h>
#include <components/PageableTable.h>

class DynamoDbItemList : public BasePage {
    Q_OBJECT

public:
    /**
     * @brief DynamoDB item list
     *
     * @param title widget title
     * @param parent parent widget
     */
    explicit DynamoDbItemList(const QString &title, QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~DynamoDbItemList() override;

    /**
     * @brief Load page content
     */
    void LoadContent() override;

    /**
     * @brief Clear content
     */
    void ClearContent() override {
    }

    /**
     * @brief Load page content
     *
     * @param listItemResponse
     */
    void HandleListItemSignal(const DynamoDbListItemResponse &listItemResponse) const;

signals:
    /**
     * @brief Show items signal
     *
     * @param itemName name of the item
     */
    void ShowItemsSignal(const QString &itemName);

    /**
     * @brief Sends a back navigation to the main window
     */
    void BackNavigationSignal();

private
slots:
    /**
     * @brief Context menu callback
     */
    void ShowContextMenu(const QPoint &pos) const;

private:
    /**
     * @brief DynamoDb table name
     */
    QString _tableName;

    /**
     * @brief Page title
     */
    QLabel *_titleLabel;

    /**
     * @brief Item list view
     */
    PageableTable *_itemView;

    /**
     * @brief REST service handler
     */
    DynamoDbService *_dynamoDbService;

    /**
     * @brief Sort column index
     *
     * @par Default sort column is 'messages', index=1
     */
    int _sortColumn = 1;

    /**
     * @brief Sort order
     */
    Qt::SortOrder _sortOrder = Qt::DescendingOrder;

    /**
     * @brief Save latest list
     */
    DynamoDbListItemResponse _internal;
};

#endif // AWSMOCK_QT_UI_DYNAMODB_ITEM_LIST_H
