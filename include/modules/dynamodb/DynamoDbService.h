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

class DynamoDbService final : public BaseService {
    Q_OBJECT

public:
    /**
     * @brief DynamoDB service
     */
    DynamoDbService() = default;

    /**
     * @brief List DynamoDB tables
     *
     * @param prefix table name prefix
     */
    void ListTables(const QString &prefix);

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
     * @brief Signaled when a bucket list needs to be reloaded
     *
     */
    void ReloadTableListSignal();

private:
    /**
     * @brief HTTP REST manager
     */
    RestManager _restManager;
};


#endif // AWSMOCK_QT_UI_DYNAMODB_SERVICE_H
