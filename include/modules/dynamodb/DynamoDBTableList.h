#ifndef AWSMOCK_QT_UI_DYNAMODB_TABLE_LIST_H
#define AWSMOCK_QT_UI_DYNAMODB_TABLE_LIST_H

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
#include <utils/DateTimeUtils.h>
#include <utils/PrefixFilterModel.h>
#include <dto/dynamodb/DynamoDbListTableResponse.h>
#include <modules/dynamodb/DynamoDbService.h>
#include <modules/dynamodb/DynamoDbAddTableDialog.h>

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
     * @brief Load page content
     */
    void LoadContent() override;

    /**
     * @brief Load page content
     *
     * @param listTableResponse
     * @param listTableResponse
     */
    void HandleListTableSignal(const DynamoDbListTableResponse &listTableResponse);

signals:
    void ShowS3Objects(const QString &bucketName);

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
    QTableView *_tableView;

    /**
     * @brief Topic prefix search
     */
    QString _prefixValue = "";

    /**
     * @brief REST service handler
     */
    DynamoDbService *_dynamoDbService;

    /**
     *  @brief Table data model
     */
    QStandardItemModel *_dataModel;

    /**
     * @brief Data proxy model
     */
    PrefixFilterProxyModel *_proxyModel;

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

#endif // AWSMOCK_QT_UI_DYNAMODB_TABLE_LIST_H
