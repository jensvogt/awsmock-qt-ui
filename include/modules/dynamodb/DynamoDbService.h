#ifndef AWSMOCK_QT_UI_DYNAMODB_SERVICE_H
#define AWSMOCK_QT_UI_DYNAMODB_SERVICE_H

// Qt includes
#include <QMessageBox>
#include <QObject>
#include <QUrlQuery>
#include <QElapsedTimer>

// AwsMock includes
#include <utils/Configuration.h>
#include <utils/RestManager.h>
#include <utils/BaseService.h>
#include <utils/EventBus.h>
#include <utils/Logging.h>
#include <dto/dynamodb/DynamoDbListTableResponse.h>
#include <dto/dynamodb/DynamoDbCreateTableRequest.h>
#include <dto/dynamodb/DynamoDbCreateTableResponse.h>
#include <dto/dynamodb/DynamoDbDescribeTableResponse.h>
#include <dto/dynamodb/DynamoDbListItemResponse.h>
#include <dto/dynamodb/DynamoDbExportItemsResponse.h>
#include <dto/dynamodb/DynamoDbGetItemResponse.h>

class DynamoDbService final : public BaseService {
    Q_OBJECT

public:
    /**
     * @brief DynamoDB service
     */
    DynamoDbService() = default;

    /**
     * @brief Create a new DynamoDB table
     *
     * @param request create table request
     */
    void CreateTable(const DynamoDbCreateTableRequest &request);

    /**
     * @brief List DynamoDB tables
     *
     * @param prefix table name prefix
     */
    void ListTables(const QString &prefix);

    /**
     * @brief Describe a DynamoDB table
     *
     * @param tableName name of the table
     */
    void DescribeTable(const QString &tableName);

    /**
     * @brief export the table items
     *
     * @param tableName name of the table
     */
    void ExportItems(const QString &tableName);

    /**
     * @brief Delete a DynamoDB table
     *
     * @param tableName name of the table
     */
    void DeleteTable(const QString &tableName);

    /**
     * @brief List DynamoDB table items
     *
     * @param tableName table name
     * @param prefix
     * @param pageSize
     * @param pageIndex
     * @param sortColumn
     * @param sortDirection
     */
    void ListItems(const QString &tableName, const QString &prefix, long pageSize, long pageIndex, const QString &sortColumn, int sortDirection);

    /**
     * @brief Purge a table
     *
     * @param tableName table name
     */
    void PurgeTable(const QString &tableName);

    /**
     * @brief Sends a recalculate item counter request
     */
    void ResetItemCounters();

    /**
     * @brief Get an item
     *
     * @param tableName name of the table
     * @param partitionKey partition key
     * @param sortKey sort key
     */
    void GetItem(const QString &tableName, const QString &partitionKey, const QString &sortKey);

signals:
    /**
     * @brief Signaled when a table list is available
     *
     * @param listTablesResponse list of tables
     */
    void ListTablesSignal(const DynamoDbListTableResponse &listTablesResponse);

    /**
     * @brief Signaled when the table list needs to be reloaded
     */
    void ReloadTableListSignal();

    /**
     * @brief Signaled when new table got created
     *
     * @param dynamodbResponse create table response
     */
    void CreateTableSignal(DynamoDbCreateTableResponse dynamodbResponse);

    /**
     * @brief Signaled when new table got created
     *
     * @param dynamodbResponse create table response
     */
    void DescribeTableSignal(DynamoDbDescribeTableResponse dynamodbResponse);

    /**
     * @brief Signaled when a table items list is available
     *
     * @param listItemsResponse list of table items
     */
    void ListItemsSignal(const DynamoDbListItemResponse &listItemsResponse);

    /**
     * @brief Signaled when a table export items list is available
     *
     * @param exportitemsResponse list of table items
     */
    void ExportItemsSignal(const QString &exportitemsResponse);

    /**
     * @brief Signaled when an item is available
     *
     * @param getItemResponse item response
     */
    void GetItemSignal(const DynamoDbGetItemResponse &getItemResponse);

private:
    /**
     * @brief HTTP REST manager
     */
    RestManager _restManager;
};


#endif // AWSMOCK_QT_UI_DYNAMODB_SERVICE_H
