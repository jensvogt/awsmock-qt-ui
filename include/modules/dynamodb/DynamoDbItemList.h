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
#include <QFileDialog>

// AwsMock includes
#include <utils/Logging.h>
#include <utils/BasePage.h>
#include <utils/IconUtils.h>
#include <utils/StringUtils.h>
#include <utils/DateTimeUtils.h>
#include <utils/PrefixFilterModel.h>
#include <components/PageableList.h>
#include <modules/module/ModuleService.h>
#include <modules/dynamodb/DynamoDbService.h>

class DynamoDbItemList final : public BasePage {
    Q_OBJECT

public:
    /**
     * @brief DynamoDB item list
     *
     * @param title widget title
     * @param parent parent widget
     */
    explicit DynamoDbItemList(const QString &title, QWidget *parent);

    /**
     * Destructor
     */
    ~DynamoDbItemList() override;

    /**
     * @brief Clear the page content
     */
    void ClearContent() override {
    }

    /**
     * @brief Load page content
     */
    void LoadContent() override;

    void ExportItems() const;

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
    PageableList *_itemView;

    /**
     * @brief DynamoDB REST service handler
     */
    DynamoDbService *_dynamoDbService;

    /**
     * @brief Module REST service handler
     */
    ModuleService *_moduleService;

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
};

#endif // AWSMOCK_QT_UI_DYNAMODB_ITEM_LIST_H
