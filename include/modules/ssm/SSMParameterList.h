#ifndef AWSMOCK_QT_UI_SSM_PARAMETER_LIST_H
#define AWSMOCK_QT_UI_SSM_PARAMETER_LIST_H

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
#include <utils/PrefixFilterModel.h>
#include <components/PageableTable.h>
#include <modules/ssm/SSMService.h>
#include <modules/ssm/SSMParameterAddDialog.h>
#include <modules/ssm/SSMParameterEditDialog.h>

class SSMParameterList final : public BasePage {
    Q_OBJECT

public:
    /**
     * @brief SSM parameter list
     *
     * @param title widget title
     * @param parent parent widget
     */
    explicit SSMParameterList(const QString &title, QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~SSMParameterList() override;

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
     * @brief Handle parameter list signal
     *
     * @param listParameterResponse parameter list
     */
    void HandleParameterListSignal(const SSMParameterListResponse &listParameterResponse) const;

signals:
    /**
     * @brief Reload SSM parameter list
     *
     * @param bucketName name of the bucket
     */
    void ShowS3Objects(const QString &bucketName);

private slots:
    /**
     * @brief Context menu callback
     */
    void ShowContextMenu(const QPoint &pos);

private:
    /**
     * @brief AWS region
     */
    QString _region;

    /**
     * @brief SSM parameter table view
     */
    PageableTable *_tableView;

    /**
     * @brief Topic prefix search
     */
    QString prefixValue = "";

    /**
     * @brief REST service handler
     */
    SSMService *_ssmService;

    /**
     * @brief Sort column index
     *
     * @par Default sort column is 'messages', index=1
     */
    int _sortColumn = 0;

    /**
     * @brief Sort order
     */
    Qt::SortOrder _sortOrder = Qt::DescendingOrder;

    /**
     * @brief Prefix clear button
     */
    QPushButton *prefixClear;

    /**
     *  @brief Table data model
     */
    QStandardItemModel *_dataModel;

    /**
     * @brief Data proxy model
     */
    PrefixFilterProxyModel *_proxyModel;
};

#endif // AWSMOCK_QT_UI_SSM_PARAMETER_LIST_H
