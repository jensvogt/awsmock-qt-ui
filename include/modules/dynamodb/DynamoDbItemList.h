#ifndef AWSMOCK_QT_UI_DYNAMODB_ITEM_LIST_H
#define AWSMOCK_QT_UI_DYNAMODB_ITEM_LIST_H

// Qt includes
#include <QLabel>
#include <QMenu>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QListWidget>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QLineEdit>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QToolBar>
#include <QPushButton>
#include <QInputDialog>
#include <QFormLayout>
#include <QTreeWidget>
#include <QTimer>

// AwsMock includes
#include <utils/BasePage.h>
#include <utils/IconUtils.h>
#include <utils/DateTimeUtils.h>
#include <utils/PrefixFilterModel.h>
//#include <dto/dynamodb/DynamoDbListItemResponse.h>
#include <modules/dynamodb/DynamoDbService.h>

class DynamoDbItemList final : public BasePage {
    Q_OBJECT

public:
    /**
     * @brief DynamoDB item list
     *
     * @param title widget title
     * @param tableName name of the parent table
     * @param parent parent widget
     */
    explicit DynamoDbItemList(const QString &title, const QString &tableName, QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~DynamoDbItemList() override;

    /**
     * @brief Load page content
     */
    void LoadContent() override;

    void HandleListItemSignal(const DynamoDbListItemResponse &listItemResponse);

    /**
     * @brief Load page content
     *
     * @param listItemResponse
     */
    //void HandleListItemSignal(const DynamoDbListItemResponse &listItemResponse);

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
     * @brief Item list view
     */
    QListView *_itemView;

    /**
     * @brief Topic prefix search
     */
    QString _prefixValue = "";

    /**
     * @brief REST service handler
     */
    DynamoDbService *_dynamoDbService;

    /**
     *  @brief Item data model
     */
    QStandardItemModel *_dataModel;

    /**
     * @brief Data proxy model
     */
    PrefixFilterProxyModel *_proxyModel{};

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
     * @brief Prefix clear button
     */
    QPushButton *_prefixClear;
};

#endif // AWSMOCK_QT_UI_DYNAMODB_ITEM_LIST_H