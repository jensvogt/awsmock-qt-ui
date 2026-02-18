//
// Created by vogje01 on 11/7/25.
//

#ifndef AWSMOCK_QT_UI_SECRET_LIST_H
#define AWSMOCK_QT_UI_SECRET_LIST_H

// QT includes
#include <QMenu>
#include <QLabel>
#include <QTableView>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QPushButton>
#include <QHeaderView>

// Awsmock includes
#include <utils/BasePage.h>
#include <utils/IconUtils.h>
#include <utils/PrefixFilterModel.h>
#include <modules/secretsmanager/SecretsManagerService.h>
#include <modules/secretsmanager/SecretsDetailsDialog.h>
#include <modules/secretsmanager/SecretAddDialog.h>

/**
 * @brief Secrets list widget. The widget will be placed in the content pane.
 */
class SecretList final : public BasePage {
    Q_OBJECT

public:
    /**
     * @brief Secretsmanager secret list
     *
     * @param title widget title
     * @param parent parent widget
     */
    explicit SecretList(const QString &title, QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~SecretList() override;

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
     * @param secretsListResponse secrets counter list
     */
    void HandleListSecretsSignal(const SecretsListResponse &secretsListResponse) const;

private slots:
    /**
     * @brief Show context menu
     *
     * @param pos table position
     */
    void ShowContextMenu(const QPoint &pos);

private:
    /**
     * @brief Service
     */
    SecretsManagerService *_secretsManagerService;

    /**
     * @brief Sort column index
     */
    int _sortColumn = 0;

    /**
     * @brief Sort order
     */
    Qt::SortOrder _sortOrder = Qt::AscendingOrder;

    /**
     * @brief Secrets table view
     */
    QTableView *_tableView;

    /**
     * @brief Data model
     */
    QStandardItemModel *_dataModel{};

    /**
     * @brief Data proxy model
     */
    PrefixFilterProxyModel *_proxyModel{};
};

#endif // AWSMOCK_QT_UI_SECRET_LIST_H
