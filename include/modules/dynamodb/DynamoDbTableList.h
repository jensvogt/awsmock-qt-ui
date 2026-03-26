#ifndef AWSMOCK_QT_UI_DYNAMODB_TABLE_LIST_H
#define AWSMOCK_QT_UI_DYNAMODB_TABLE_LIST_H

// C++ includes
#include <ranges>
#include <algorithm>

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

#include <utils/BasePage.h>
#include <utils/IconUtils.h>
#include <utils/StringUtils.h>
#include <utils/DateTimeUtils.h>
#include <utils/PrefixFilterModel.h>
#include <components/PageableTable.h>
#include <dto/dynamodb/DynamoDbListTableResponse.h>
#include <modules/dynamodb/DynamoDbService.h>
#include <modules/dynamodb/DynamoDbAddTableDialog.h>
#include <modules/dynamodb/DynamoDbEditTableDialog.h>

class DynamoDbTableList final : public BasePage {
    Q_OBJECT

public:
    /**
     * @brief DynamoDB table list
     *
     * @param title widget title
     * @param parent parent widget
     */
    explicit DynamoDbTableList(const QString &title, QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~DynamoDbTableList() override;

    /**
     * @brief Clear the page content
     */
    void ClearContent() override {
    }

    /**
     * @brief Load page content
     */
    void LoadContent() override;

    /**
     * @brief Load page content
     *
     * @param listTableResponse
     */
    void HandleListTableSignal(const DynamoDbListTableResponse &listTableResponse);

signals:
    /**
     * @brief Show items signal
     *
     * @param tableName name of the table
     */
    void ShowItemsSignal(const QString &tableName);

private slots:
    /**
     * @brief Context menu callback
     */
    void ShowContextMenu(const QPoint &pos) const;

private:
    /**
     * @brief AWS region
     */
    QString _region;

    /**
     * @brief Qt network manager
     */
    PageableTable *_tableView;

    /**
     * @brief Topic prefix search
     */
    QString _prefixValue = "";

    /**
     * @brief REST service handler
     */
    DynamoDbService *_dynamoDbService;
    QList<DynamoDbTableCounter> _internalData;
};

#endif // AWSMOCK_QT_UI_DYNAMODB_TABLE_LIST_H
