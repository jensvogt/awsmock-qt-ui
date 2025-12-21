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
#include <dto/dynamodb/DynamoDbListTableResponse.h>
#include <dto/dynamodb/DynamoDbCreateTableRequest.h>
#include <dto/dynamodb/DynamoDbCreateTableResponse.h>
#include <dto/dynamodb/DynamoDbDescribeTableResponse.h>

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
     * @brief Delete a DynamoDB table
     *
     * @param tableName name of the table
     */
    void DeleteTable(const QString &tableName);

signals:
    /**
     * @brief Signaled when a table list is available
     *
     * @param listTablesResponse list of buckets
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

private:
    /**
     * @brief HTTP REST manager
     */
    RestManager _restManager;
};


#endif // AWSMOCK_QT_UI_DYNAMODB_SERVICE_H
